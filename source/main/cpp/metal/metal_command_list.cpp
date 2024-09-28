#include "cgfx/metal/metal_command_list.h"
#include "cgfx/metal/metal_device.h"
#include "cgfx/metal/metal_swapchain.h"
#include "cgfx/metal/metal_fence.h"
#include "cgfx/metal/metal_texture.h"
#include "cgfx/metal/metal_pipeline_state.h"
#include "cgfx/metal/metal_descriptor.h"
#include "cgfx/metal/metal_rt_blas.h"
#include "cgfx/metal/metal_rt_tlas.h"
#include "cgfx/gfx.h"

#include "cmacos/Metal/Metal.hpp"
#include "cmacos/MetalShaderConverter/IRConverterRuntime/ir_converter_runtime.h"

namespace ncore
{
    namespace ngfx
    {
        static bool nearly_equal(float a, float b, float epsilon = 0.0001f) { return fabs(a - b) < epsilon; }

        MetalCommandList::MetalCommandList(MetalDevice* pDevice, GfxCommandQueue queue_type, const eastl::string& name)
        {
            m_pDevice   = pDevice;
            m_name      = name;
            m_queueType = queue_type;
        }

        MetalCommandList::~MetalCommandList() { m_pFence->release(); }

        bool MetalCommandList::Create()
        {
            MTL::Device* device = (MTL::Device*)m_pDevice->GetHandle();
            m_pFence            = device->newFence();

            return true;
        }

        void MetalCommandList::ResetAllocator() {}

        void MetalCommandList::Begin()
        {
            MTL::CommandQueue* queue = ((MetalDevice*)m_pDevice)->GetQueue();

            m_pCommandBuffer = queue->commandBuffer();
        }

        void MetalCommandList::End()
        {
            EndBlitEncoder();
            EndComputeEncoder();
            EndRenderPass();
        }

        void MetalCommandList::Wait(IGfxFence* fence, u64 value) { m_pCommandBuffer->encodeWait((const MTL::Event*)fence->GetHandle(), value); }

        void MetalCommandList::Signal(IGfxFence* fence, u64 value) { m_pCommandBuffer->encodeSignalEvent((const MTL::Event*)fence->GetHandle(), value); }

        void MetalCommandList::Present(IGfxSwapchain* swapchain) { m_pCommandBuffer->presentDrawable(((MetalSwapchain*)swapchain)->GetDrawable()); }

        void MetalCommandList::Submit()
        {
            m_pCommandBuffer->commit();
            m_pCommandBuffer = nullptr;
        }

        void MetalCommandList::ResetState()
        {
            m_pBlitCommandEncoder    = nullptr;
            m_pRenderCommandEncoder  = nullptr;
            m_pComputeCommandEncoder = nullptr;
            m_pASEncoder             = nullptr;

            m_pCurrentPSO = nullptr;

            m_pIndexBuffer      = nullptr;
            m_indexBufferOffset = 0;
            m_indexType         = MTL::IndexTypeUInt16;
            m_primitiveType     = MTL::PrimitiveTypeTriangle;
            m_cullMode          = MTL::CullModeNone;
            m_frontFaceWinding  = MTL::WindingClockwise;
            m_fillMode          = MTL::TriangleFillModeFill;
            m_clipMode          = MTL::DepthClipModeClip;
            m_depthBias         = 0.0f;
            m_depthBiasClamp    = 0.0f;
            m_depthSlopeScale   = 0.0f;
        }

        void MetalCommandList::BeginProfiling() {}

        void MetalCommandList::EndProfiling() {}

        void MetalCommandList::BeginEvent(const eastl::string& event_name)
        {
            NS::String* label = NS::String::alloc()->init(event_name.c_str(), NS::StringEncoding::UTF8StringEncoding);
            m_pCommandBuffer->pushDebugGroup(label);
            label->release();
        }

        void MetalCommandList::EndEvent() { m_pCommandBuffer->popDebugGroup(); }

        void MetalCommandList::CopyBufferToTexture(IGfxTexture* dst_texture, u32 mip_level, u32 array_slice, IGfxBuffer* src_buffer, u32 offset)
        {
            BeginBlitEncoder();

            const GfxTextureDesc& desc        = dst_texture->GetDesc();
            MTL::Size             textureSize = MTL::Size::Make(eastl::max(desc.width >> mip_level, 1u), eastl::max(desc.height >> mip_level, 1u), eastl::max(desc.depth >> mip_level, 1u));

            u32 bytesPerRow = ((MetalTexture*)dst_texture)->GetRowPitch(mip_level);

            u32 block_height  = GetFormatBlockHeight(desc.format);
            u32 height        = eastl::max(desc.height >> mip_level, block_height);
            u32 row_num       = height / block_height;
            u32 bytesPerImage = bytesPerRow * row_num;

            m_pBlitCommandEncoder->copyFromBuffer((MTL::Buffer*)src_buffer->GetHandle(), offset, bytesPerRow, desc.type == GfxTextureType::Texture3D ? bytesPerImage : 0, textureSize, (MTL::Texture*)dst_texture->GetHandle(), array_slice, mip_level,
                                                  MTL::Origin::Make(0, 0, 0));
        }

        void MetalCommandList::CopyTextureToBuffer(IGfxBuffer* dst_buffer, u32 offset, IGfxTexture* src_texture, u32 mip_level, u32 array_slice)
        {
            BeginBlitEncoder();

            const GfxTextureDesc& desc        = src_texture->GetDesc();
            MTL::Size             textureSize = MTL::Size::Make(eastl::max(desc.width >> mip_level, 1u), eastl::max(desc.height >> mip_level, 1u), eastl::max(desc.depth >> mip_level, 1u));

            u32 bytesPerRow = ((MetalTexture*)src_texture)->GetRowPitch(mip_level);

            u32 block_height  = GetFormatBlockHeight(desc.format);
            u32 height        = eastl::max(desc.height >> mip_level, block_height);
            u32 row_num       = height / block_height;
            u32 bytesPerImage = bytesPerRow * row_num;

            m_pBlitCommandEncoder->copyFromTexture((MTL::Texture*)src_texture->GetHandle(), array_slice, mip_level, MTL::Origin(0, 0, 0), textureSize, (MTL::Buffer*)dst_buffer->GetHandle(), offset, bytesPerRow,
                                                   desc.type == GfxTextureType::Texture3D ? bytesPerImage : 0);
        }

        void MetalCommandList::CopyBuffer(IGfxBuffer* dst, u32 dst_offset, IGfxBuffer* src, u32 src_offset, u32 size)
        {
            BeginBlitEncoder();

            m_pBlitCommandEncoder->copyFromBuffer((MTL::Buffer*)src->GetHandle(), src_offset, (MTL::Buffer*)dst->GetHandle(), dst_offset, size);
        }

        void MetalCommandList::CopyTexture(IGfxTexture* dst, u32 dst_mip, u32 dst_array, IGfxTexture* src, u32 src_mip, u32 src_array)
        {
            BeginBlitEncoder();

            const GfxTextureDesc& desc     = src->GetDesc();
            MTL::Size             src_size = MTL::Size::Make(eastl::max(desc.width >> src_mip, 1u), eastl::max(desc.height >> src_mip, 1u), eastl::max(desc.depth >> src_mip, 1u));

            m_pBlitCommandEncoder->copyFromTexture((MTL::Texture*)src->GetHandle(), src_array, src_mip, MTL::Origin(0, 0, 0), src_size, (MTL::Texture*)dst->GetHandle(), dst_array, dst_mip, MTL::Origin(0, 0, 0));
        }

        void MetalCommandList::ClearUAV(IGfxResource* resource, IGfxDescriptor* uav, const float* clear_value, IGfxClearUavApi* clear_api)
        {
            const GfxUnorderedAccessViewDesc& desc = ((MetalUnorderedAccessView*)uav)->GetDesc();
            clear_api->ClearUAV(this, resource, uav, desc, clear_value);
        }

        void MetalCommandList::ClearUAV(IGfxResource* resource, IGfxDescriptor* uav, const u32* clear_value, IGfxClearUavApi* clear_api)
        {
            const GfxUnorderedAccessViewDesc& desc = ((MetalUnorderedAccessView*)uav)->GetDesc();
            clear_api->ClearUAV(this, resource, uav, desc, clear_value);
        }

        void MetalCommandList::WriteBuffer(IGfxBuffer* buffer, u32 offset, u32 data)
        {
            BeginBlitEncoder();

            // TODO handle events
            //BeginEvent("IGfxCommandList::WriteBuffer");

            MTL::Buffer* mtlBuffer = (MTL::Buffer*)buffer->GetHandle();
            m_pBlitCommandEncoder->fillBuffer(mtlBuffer, NS::Range::Make(offset + 0, sizeof(u8)), u8(data >> 0));
            m_pBlitCommandEncoder->fillBuffer(mtlBuffer, NS::Range::Make(offset + 1, sizeof(u8)), u8(data >> 8));
            m_pBlitCommandEncoder->fillBuffer(mtlBuffer, NS::Range::Make(offset + 2, sizeof(u8)), u8(data >> 16));
            m_pBlitCommandEncoder->fillBuffer(mtlBuffer, NS::Range::Make(offset + 3, sizeof(u8)), u8(data >> 24));

            //EndEvent();
        }

        void MetalCommandList::UpdateTileMappings(IGfxTexture* texture, IGfxHeap* heap, u32 mapping_count, const GfxTileMapping* mappings)
        {
            // todo
        }

        void MetalCommandList::TextureBarrier(IGfxTexture* texture, u32 sub_resource, GfxAccessFlags access_before, GfxAccessFlags access_after) {}

        void MetalCommandList::BufferBarrier(IGfxBuffer* buffer, GfxAccessFlags access_before, GfxAccessFlags access_after) {}

        void MetalCommandList::GlobalBarrier(GfxAccessFlags access_before, GfxAccessFlags access_after) {}

        void MetalCommandList::FlushBarriers() {}

        void MetalCommandList::BeginRenderPass(const GfxRenderPassDesc& render_pass)
        {
            EndBlitEncoder();
            EndComputeEncoder();
            EndASEncoder();

            u32 width  = 0;
            u32 height = 0;

            MTL::RenderPassDescriptor*                     descriptor        = MTL::RenderPassDescriptor::alloc()->init();
            MTL::RenderPassColorAttachmentDescriptorArray* colorAttachements = descriptor->colorAttachments();

            for (u32 i = 0; i < 8; ++i)
            {
                if (render_pass.color[i].texture == nullptr)
                {
                    continue;
                }

                if (width == 0)
                {
                    width = render_pass.color[i].texture->GetDesc().width;
                }

                if (height == 0)
                {
                    height = render_pass.color[i].texture->GetDesc().height;
                }

                ASSERT(width == render_pass.color[i].texture->GetDesc().width);
                ASSERT(height == render_pass.color[i].texture->GetDesc().height);

                colorAttachements->object(i)->setTexture((MTL::Texture*)render_pass.color[i].texture->GetHandle());
                colorAttachements->object(i)->setLevel(render_pass.color[i].mip_slice);
                colorAttachements->object(i)->setSlice(render_pass.color[i].array_slice);
                colorAttachements->object(i)->setLoadAction(ToLoadAction(render_pass.color[i].load_op));
                colorAttachements->object(i)->setStoreAction(ToStoreAction(render_pass.color[i].store_op));
                colorAttachements->object(i)->setClearColor(MTL::ClearColor::Make(render_pass.color[i].clear_color[0], render_pass.color[i].clear_color[1], render_pass.color[i].clear_color[2], render_pass.color[i].clear_color[3]));
            }

            if (render_pass.depth.texture != nullptr)
            {
                if (width == 0)
                {
                    width = render_pass.depth.texture->GetDesc().width;
                }

                if (height == 0)
                {
                    height = render_pass.depth.texture->GetDesc().height;
                }

                ASSERT(width == render_pass.depth.texture->GetDesc().width);
                ASSERT(height == render_pass.depth.texture->GetDesc().height);

                MTL::RenderPassDepthAttachmentDescriptor* depthAttachment = descriptor->depthAttachment();
                depthAttachment->setTexture((MTL::Texture*)render_pass.depth.texture->GetHandle());
                depthAttachment->setLevel(render_pass.depth.mip_slice);
                depthAttachment->setSlice(render_pass.depth.array_slice);
                depthAttachment->setLoadAction(ToLoadAction(render_pass.depth.load_op));
                depthAttachment->setStoreAction(ToStoreAction(render_pass.depth.store_op));
                depthAttachment->setClearDepth(render_pass.depth.clear_depth);

                if (IsStencilFormat(render_pass.depth.texture->GetDesc().format))
                {
                    MTL::RenderPassStencilAttachmentDescriptor* stencilAttachment = descriptor->stencilAttachment();
                    stencilAttachment->setTexture((MTL::Texture*)render_pass.depth.texture->GetHandle());
                    stencilAttachment->setLevel(render_pass.depth.mip_slice);
                    stencilAttachment->setSlice(render_pass.depth.array_slice);
                    stencilAttachment->setLoadAction(ToLoadAction(render_pass.depth.stencil_load_op));
                    stencilAttachment->setStoreAction(ToStoreAction(render_pass.depth.stencil_store_op));
                    stencilAttachment->setClearStencil(render_pass.depth.clear_stencil);
                }
            }

            ASSERT(m_pRenderCommandEncoder == nullptr);
            m_pRenderCommandEncoder = m_pCommandBuffer->renderCommandEncoder(descriptor);
            m_pRenderCommandEncoder->waitForFence(m_pFence, MTL::RenderStageVertex | MTL::RenderStageObject);
            descriptor->release();

            MetalDevice* pDevice = (MetalDevice*)m_pDevice;
            m_pRenderCommandEncoder->setVertexBuffer(pDevice->GetResourceDescriptorBuffer(), 0, kIRDescriptorHeapBindPoint);
            m_pRenderCommandEncoder->setVertexBuffer(pDevice->GetSamplerDescriptorBuffer(), 0, kIRSamplerHeapBindPoint);
            m_pRenderCommandEncoder->setFragmentBuffer(pDevice->GetResourceDescriptorBuffer(), 0, kIRDescriptorHeapBindPoint);
            m_pRenderCommandEncoder->setFragmentBuffer(pDevice->GetSamplerDescriptorBuffer(), 0, kIRSamplerHeapBindPoint);

            SetViewport(0, 0, width, height);
        }

        void MetalCommandList::EndRenderPass()
        {
            if (m_pRenderCommandEncoder)
            {
                m_pRenderCommandEncoder->updateFence(m_pFence, MTL::RenderStageFragment);
                m_pRenderCommandEncoder->endEncoding();
                m_pRenderCommandEncoder = nullptr;

                ResetState();
            }
        }

        void MetalCommandList::SetPipelineState(IGfxPipelineState* state)
        {
            if (m_pCurrentPSO != state)
            {
                m_pCurrentPSO = state;

                if (state->GetType() != GfxPipelineType::Compute)
                {
                    ASSERT(m_pRenderCommandEncoder != nullptr);

                    m_pRenderCommandEncoder->setRenderPipelineState((MTL::RenderPipelineState*)state->GetHandle());

                    if (state->GetType() == GfxPipelineType::Graphics)
                    {
                        MetalGraphicsPipelineState*    graphicsPSO = (MetalGraphicsPipelineState*)state;
                        const GfxGraphicsPipelineDesc& desc        = graphicsPSO->GetDesc();

                        m_pRenderCommandEncoder->setDepthStencilState(graphicsPSO->GetDepthStencilState());
                        SetRasterizerState(desc.rasterizer_state);
                        m_primitiveType = ToPrimitiveType(desc.primitive_type);
                    }
                    else
                    {
                        MetalMeshShadingPipelineState*    meshShadingPSO = (MetalMeshShadingPipelineState*)state;
                        const GfxMeshShadingPipelineDesc& desc           = meshShadingPSO->GetDesc();

                        m_pRenderCommandEncoder->setDepthStencilState(meshShadingPSO->GetDepthStencilState());
                        SetRasterizerState(desc.rasterizer_state);
                    }
                }
            }
        }

        void MetalCommandList::SetStencilReference(u8 stencil)
        {
            ASSERT(m_pRenderCommandEncoder != nullptr);
            m_pRenderCommandEncoder->setStencilReferenceValue(stencil);
        }

        void MetalCommandList::SetBlendFactor(const float* blend_factor)
        {
            ASSERT(m_pRenderCommandEncoder != nullptr);
            m_pRenderCommandEncoder->setBlendColor(blend_factor[0], blend_factor[1], blend_factor[2], blend_factor[3]);
        }

        void MetalCommandList::SetIndexBuffer(IGfxBuffer* buffer, u32 offset, GfxFormat format)
        {
            ASSERT(m_pRenderCommandEncoder != nullptr);

            m_pIndexBuffer      = (MTL::Buffer*)buffer->GetHandle();
            m_indexBufferOffset = offset;
            m_indexType         = format == GfxFormat::R16UI ? MTL::IndexTypeUInt16 : MTL::IndexTypeUInt32;
        }

        void MetalCommandList::SetViewport(u32 x, u32 y, u32 width, u32 height)
        {
            ASSERT(m_pRenderCommandEncoder != nullptr);

            MTL::Viewport viewport; // = {x, y, width, height, 0.0, 1.0};
            viewport.originX = x;
            viewport.originY = y;
            viewport.width   = width;
            viewport.height  = height;
            viewport.znear   = 0.0;
            viewport.zfar    = 1.0;
            m_pRenderCommandEncoder->setViewport(viewport);

            SetScissorRect(x, y, width, height);
        }

        void MetalCommandList::SetScissorRect(u32 x, u32 y, u32 width, u32 height)
        {
            ASSERT(m_pRenderCommandEncoder != nullptr);

            MTL::ScissorRect scissorRect = {x, y, width, height};
            m_pRenderCommandEncoder->setScissorRect(scissorRect);
        }

        void MetalCommandList::SetGraphicsConstants(u32 slot, const void* data, s64 data_size)
        {
            if (slot == 0)
            {
                ASSERT(data_size <= GFX_MAX_ROOT_CONSTANTS * sizeof(u32));
                memcpy(m_graphicsArgumentBuffer.cbv0, data, data_size);
            }
            else
            {
                ASSERT(slot < GFX_MAX_CBV_BINDINGS);
                u64 gpuAddress = ((MetalDevice*)m_pDevice)->AllocateConstantBuffer(data, data_size);

                if (slot == 1)
                {
                    m_graphicsArgumentBuffer.cbv1 = gpuAddress;
                }
                else
                {
                    m_graphicsArgumentBuffer.cbv2 = gpuAddress;
                }
            }
        }

        void MetalCommandList::SetComputeConstants(u32 slot, const void* data, s64 data_size)
        {
            if (slot == 0)
            {
                ASSERT(data_size <= GFX_MAX_ROOT_CONSTANTS * sizeof(u32));
                memcpy(m_computeArgumentBuffer.cbv0, data, data_size);
            }
            else
            {
                ASSERT(slot < GFX_MAX_CBV_BINDINGS);
                u64 gpuAddress = ((MetalDevice*)m_pDevice)->AllocateConstantBuffer(data, data_size);

                if (slot == 1)
                {
                    m_computeArgumentBuffer.cbv1 = gpuAddress;
                }
                else
                {
                    m_computeArgumentBuffer.cbv2 = gpuAddress;
                }
            }
        }

        void MetalCommandList::Draw(u32 vertex_count, u32 instance_count)
        {
            ASSERT(m_pRenderCommandEncoder != nullptr);

            m_pRenderCommandEncoder->setVertexBytes(&m_graphicsArgumentBuffer, sizeof(TopLevelArgumentBuffer), kIRArgumentBufferBindPoint);
            m_pRenderCommandEncoder->setFragmentBytes(&m_graphicsArgumentBuffer, sizeof(TopLevelArgumentBuffer), kIRArgumentBufferBindPoint);

            IRRuntimeDrawPrimitives(m_pRenderCommandEncoder, m_primitiveType, 0, vertex_count, instance_count);
        }

        void MetalCommandList::DrawIndexed(u32 index_count, u32 instance_count, u32 index_offset)
        {
            ASSERT(m_pRenderCommandEncoder != nullptr);

            m_pRenderCommandEncoder->setVertexBytes(&m_graphicsArgumentBuffer, sizeof(TopLevelArgumentBuffer), kIRArgumentBufferBindPoint);
            m_pRenderCommandEncoder->setFragmentBytes(&m_graphicsArgumentBuffer, sizeof(TopLevelArgumentBuffer), kIRArgumentBufferBindPoint);

            u64 indexBufferOffset = m_indexBufferOffset;
            if (m_indexType == MTL::IndexTypeUInt16)
            {
                indexBufferOffset += sizeof(uint16_t) * index_offset;
            }
            else
            {
                indexBufferOffset += sizeof(u32) * index_offset;
            }

            IRRuntimeDrawIndexedPrimitives(m_pRenderCommandEncoder, m_primitiveType, index_count, m_indexType, m_pIndexBuffer, indexBufferOffset, instance_count, 0, 0);
        }

        void MetalCommandList::Dispatch(u32 group_count_x, u32 group_count_y, u32 group_count_z)
        {
            BeginComputeEncoder();

            m_pComputeCommandEncoder->setComputePipelineState((MTL::ComputePipelineState*)m_pCurrentPSO->GetHandle());
            m_pComputeCommandEncoder->setBytes(&m_computeArgumentBuffer, sizeof(TopLevelArgumentBuffer), kIRArgumentBufferBindPoint);

            MTL::Size threadgroupsPerGrid   = MTL::Size::Make(group_count_x, group_count_y, group_count_z);
            MTL::Size threadsPerThreadgroup = ((MetalComputePipelineState*)m_pCurrentPSO)->GetThreadsPerThreadgroup();
            m_pComputeCommandEncoder->dispatchThreadgroups(threadgroupsPerGrid, threadsPerThreadgroup);

            EndComputeEncoder();
        }

        void MetalCommandList::DispatchMesh(u32 group_count_x, u32 group_count_y, u32 group_count_z)
        {
            ASSERT(m_pRenderCommandEncoder != nullptr);

            MetalDevice* pDevice = (MetalDevice*)m_pDevice;
            m_pRenderCommandEncoder->setObjectBuffer(pDevice->GetResourceDescriptorBuffer(), 0, kIRDescriptorHeapBindPoint);
            m_pRenderCommandEncoder->setObjectBuffer(pDevice->GetSamplerDescriptorBuffer(), 0, kIRSamplerHeapBindPoint);
            m_pRenderCommandEncoder->setMeshBuffer(pDevice->GetResourceDescriptorBuffer(), 0, kIRDescriptorHeapBindPoint);
            m_pRenderCommandEncoder->setMeshBuffer(pDevice->GetSamplerDescriptorBuffer(), 0, kIRSamplerHeapBindPoint);

            m_pRenderCommandEncoder->setObjectBytes(&m_graphicsArgumentBuffer, sizeof(TopLevelArgumentBuffer), kIRArgumentBufferBindPoint);
            m_pRenderCommandEncoder->setMeshBytes(&m_graphicsArgumentBuffer, sizeof(TopLevelArgumentBuffer), kIRArgumentBufferBindPoint);
            m_pRenderCommandEncoder->setFragmentBytes(&m_graphicsArgumentBuffer, sizeof(TopLevelArgumentBuffer), kIRArgumentBufferBindPoint);

            MTL::Size threadgroupsPerGrid         = MTL::Size::Make(group_count_x, group_count_y, group_count_z);
            MTL::Size threadsPerObjectThreadgroup = ((MetalMeshShadingPipelineState*)m_pCurrentPSO)->GetThreadsPerObjectThreadgroup();
            MTL::Size threadsPerMeshThreadgroup   = ((MetalMeshShadingPipelineState*)m_pCurrentPSO)->GetThreadsPerMeshThreadgroup();

            m_pRenderCommandEncoder->drawMeshThreadgroups(threadgroupsPerGrid, threadsPerObjectThreadgroup, threadsPerMeshThreadgroup);
        }

        void MetalCommandList::DrawIndirect(IGfxBuffer* buffer, u32 offset)
        {
            ASSERT(m_pRenderCommandEncoder != nullptr);

            m_pRenderCommandEncoder->setVertexBytes(&m_graphicsArgumentBuffer, sizeof(TopLevelArgumentBuffer), kIRArgumentBufferBindPoint);
            m_pRenderCommandEncoder->setFragmentBytes(&m_graphicsArgumentBuffer, sizeof(TopLevelArgumentBuffer), kIRArgumentBufferBindPoint);

            IRRuntimeDrawPrimitives(m_pRenderCommandEncoder, m_primitiveType, (MTL::Buffer*)buffer->GetHandle(), offset);
        }

        void MetalCommandList::DrawIndexedIndirect(IGfxBuffer* buffer, u32 offset)
        {
            ASSERT(m_pRenderCommandEncoder != nullptr);

            m_pRenderCommandEncoder->setVertexBytes(&m_graphicsArgumentBuffer, sizeof(TopLevelArgumentBuffer), kIRArgumentBufferBindPoint);
            m_pRenderCommandEncoder->setFragmentBytes(&m_graphicsArgumentBuffer, sizeof(TopLevelArgumentBuffer), kIRArgumentBufferBindPoint);

            IRRuntimeDrawIndexedPrimitives(m_pRenderCommandEncoder, m_primitiveType, m_indexType, m_pIndexBuffer, m_indexBufferOffset, (MTL::Buffer*)buffer->GetHandle(), offset);
        }

        void MetalCommandList::DispatchIndirect(IGfxBuffer* buffer, u32 offset)
        {
            BeginComputeEncoder();

            m_pComputeCommandEncoder->setComputePipelineState((MTL::ComputePipelineState*)m_pCurrentPSO->GetHandle());
            m_pComputeCommandEncoder->setBytes(&m_computeArgumentBuffer, sizeof(TopLevelArgumentBuffer), kIRArgumentBufferBindPoint);

            MTL::Size threadsPerThreadgroup = ((MetalComputePipelineState*)m_pCurrentPSO)->GetThreadsPerThreadgroup();
            m_pComputeCommandEncoder->dispatchThreadgroups((MTL::Buffer*)buffer->GetHandle(), offset, threadsPerThreadgroup);

            EndComputeEncoder();
        }

        void MetalCommandList::DispatchMeshIndirect(IGfxBuffer* buffer, u32 offset)
        {
            ASSERT(m_pRenderCommandEncoder != nullptr);

            MetalDevice* pDevice = (MetalDevice*)m_pDevice;
            m_pRenderCommandEncoder->setObjectBuffer(pDevice->GetResourceDescriptorBuffer(), 0, kIRDescriptorHeapBindPoint);
            m_pRenderCommandEncoder->setObjectBuffer(pDevice->GetSamplerDescriptorBuffer(), 0, kIRSamplerHeapBindPoint);
            m_pRenderCommandEncoder->setMeshBuffer(pDevice->GetResourceDescriptorBuffer(), 0, kIRDescriptorHeapBindPoint);
            m_pRenderCommandEncoder->setMeshBuffer(pDevice->GetSamplerDescriptorBuffer(), 0, kIRSamplerHeapBindPoint);

            m_pRenderCommandEncoder->setObjectBytes(&m_graphicsArgumentBuffer, sizeof(TopLevelArgumentBuffer), kIRArgumentBufferBindPoint);
            m_pRenderCommandEncoder->setMeshBytes(&m_graphicsArgumentBuffer, sizeof(TopLevelArgumentBuffer), kIRArgumentBufferBindPoint);
            m_pRenderCommandEncoder->setFragmentBytes(&m_graphicsArgumentBuffer, sizeof(TopLevelArgumentBuffer), kIRArgumentBufferBindPoint);

            MTL::Size threadsPerObjectThreadgroup = ((MetalMeshShadingPipelineState*)m_pCurrentPSO)->GetThreadsPerObjectThreadgroup();
            MTL::Size threadsPerMeshThreadgroup   = ((MetalMeshShadingPipelineState*)m_pCurrentPSO)->GetThreadsPerMeshThreadgroup();

            m_pRenderCommandEncoder->drawMeshThreadgroups((MTL::Buffer*)buffer->GetHandle(), offset, threadsPerObjectThreadgroup, threadsPerMeshThreadgroup);
        }

        void MetalCommandList::MultiDrawIndirect(u32 max_count, IGfxBuffer* args_buffer, u32 args_buffer_offset, IGfxBuffer* count_buffer, u32 count_buffer_offset)
        {
            ASSERT(m_pRenderCommandEncoder != nullptr);

            // todo
        }

        void MetalCommandList::MultiDrawIndexedIndirect(u32 max_count, IGfxBuffer* args_buffer, u32 args_buffer_offset, IGfxBuffer* count_buffer, u32 count_buffer_offset)
        {
            ASSERT(m_pRenderCommandEncoder != nullptr);

            // todo
        }

        void MetalCommandList::MultiDispatchIndirect(u32 max_count, IGfxBuffer* args_buffer, u32 args_buffer_offset, IGfxBuffer* count_buffer, u32 count_buffer_offset)
        {
            // todo
        }

        void MetalCommandList::MultiDispatchMeshIndirect(u32 max_count, IGfxBuffer* args_buffer, u32 args_buffer_offset, IGfxBuffer* count_buffer, u32 count_buffer_offset)
        {
            ASSERT(m_pRenderCommandEncoder != nullptr);

            // todo
        }

        void MetalCommandList::BuildRayTracingBLAS(IGfxRayTracingBLAS* blas)
        {
            BeginASEncoder();

            MetalRayTracingBLAS* metalBLAS = (MetalRayTracingBLAS*)blas;
            m_pASEncoder->buildAccelerationStructure(metalBLAS->GetAccelerationStructure(), metalBLAS->GetDescriptor(), metalBLAS->GetScratchBuffer(), 0);
        }

        void MetalCommandList::UpdateRayTracingBLAS(IGfxRayTracingBLAS* blas, IGfxBuffer* vertex_buffer, u32 vertex_buffer_offset)
        {
            BeginASEncoder();

            MetalRayTracingBLAS* metalBLAS = (MetalRayTracingBLAS*)blas;
            metalBLAS->UpdateVertexBuffer(vertex_buffer, vertex_buffer_offset);

            m_pASEncoder->refitAccelerationStructure(metalBLAS->GetAccelerationStructure(), metalBLAS->GetDescriptor(), metalBLAS->GetAccelerationStructure(), metalBLAS->GetScratchBuffer(), 0);
        }

        void MetalCommandList::BuildRayTracingTLAS(IGfxRayTracingTLAS* tlas, const GfxRayTracingInstance* instances, u32 instance_count)
        {
            BeginASEncoder();

            MetalRayTracingTLAS* metalTLAS = (MetalRayTracingTLAS*)tlas;
            metalTLAS->UpdateInstance(instances, instance_count);

            m_pASEncoder->buildAccelerationStructure(metalTLAS->GetAccelerationStructure(), metalTLAS->GetDescriptor(), metalTLAS->GetScratchBuffer(), 0);
        }

#if MICROPROFILE_GPU_TIMERS
        MicroProfileThreadLogGpu* MetalCommandList::GetProfileLog() const { return nullptr; }
#endif

        void MetalCommandList::BeginBlitEncoder()
        {
            EndRenderPass();
            EndComputeEncoder();
            EndASEncoder();

            if (m_pBlitCommandEncoder == nullptr)
            {
                m_pBlitCommandEncoder = m_pCommandBuffer->blitCommandEncoder();
                m_pBlitCommandEncoder->waitForFence(m_pFence);
            }
        }

        void MetalCommandList::EndBlitEncoder()
        {
            if (m_pBlitCommandEncoder)
            {
                m_pBlitCommandEncoder->updateFence(m_pFence);
                m_pBlitCommandEncoder->endEncoding();
                m_pBlitCommandEncoder = nullptr;
            }
        }

        void MetalCommandList::BeginComputeEncoder()
        {
            EndRenderPass();
            EndBlitEncoder();
            EndASEncoder();

            if (m_pComputeCommandEncoder == nullptr)
            {
                m_pComputeCommandEncoder = m_pCommandBuffer->computeCommandEncoder();
                m_pComputeCommandEncoder->waitForFence(m_pFence);

                m_pComputeCommandEncoder->setBuffer(((MetalDevice*)m_pDevice)->GetResourceDescriptorBuffer(), 0, kIRDescriptorHeapBindPoint);
                m_pComputeCommandEncoder->setBuffer(((MetalDevice*)m_pDevice)->GetSamplerDescriptorBuffer(), 0, kIRSamplerHeapBindPoint);
            }
        }

        void MetalCommandList::EndComputeEncoder()
        {
            if (m_pComputeCommandEncoder)
            {
                m_pComputeCommandEncoder->updateFence(m_pFence);
                m_pComputeCommandEncoder->endEncoding();
                m_pComputeCommandEncoder = nullptr;
            }
        }

        void MetalCommandList::BeginASEncoder()
        {
            EndRenderPass();
            EndBlitEncoder();
            EndComputeEncoder();

            if (m_pASEncoder == nullptr)
            {
                m_pASEncoder = m_pCommandBuffer->accelerationStructureCommandEncoder();
                m_pASEncoder->waitForFence(m_pFence);
            }
        }

        void MetalCommandList::EndASEncoder()
        {
            if (m_pASEncoder)
            {
                m_pASEncoder->updateFence(m_pFence);
                m_pASEncoder->endEncoding();
                m_pASEncoder = nullptr;
            }
        }

        void MetalCommandList::SetRasterizerState(const GfxRasterizerState& state)
        {
            MTL::CullMode cullMode = ToCullMode(state.cull_mode);
            if (m_cullMode != cullMode)
            {
                m_pRenderCommandEncoder->setCullMode(cullMode);
                m_cullMode = cullMode;
            }

            MTL::Winding frontFaceWinding = state.front_ccw ? MTL::WindingCounterClockwise : MTL::WindingClockwise;
            if (m_frontFaceWinding != frontFaceWinding)
            {
                m_pRenderCommandEncoder->setFrontFacingWinding(frontFaceWinding);
                m_frontFaceWinding = frontFaceWinding;
            }

            MTL::TriangleFillMode fillMode = state.wireframe ? MTL::TriangleFillModeLines : MTL::TriangleFillModeFill;
            if (m_fillMode != fillMode)
            {
                m_pRenderCommandEncoder->setTriangleFillMode(fillMode);
                m_fillMode = fillMode;
            }

            MTL::DepthClipMode clipMode = state.depth_clip ? MTL::DepthClipModeClip : MTL::DepthClipModeClamp;
            if (m_clipMode != clipMode)
            {
                m_pRenderCommandEncoder->setDepthClipMode(clipMode);
                m_clipMode = clipMode;
            }

            if (!nearly_equal(state.depth_bias, m_depthBias) || !nearly_equal(state.depth_bias_clamp, m_depthBiasClamp) || !nearly_equal(state.depth_slope_scale, m_depthSlopeScale))
            {
                m_pRenderCommandEncoder->setDepthBias(state.depth_bias, state.depth_slope_scale, state.depth_bias_clamp);

                m_depthBias       = state.depth_bias;
                m_depthBiasClamp  = state.depth_bias_clamp;
                m_depthSlopeScale = state.depth_slope_scale;
            }
        }

    }  // namespace ngfx
}  // namespace ncore
