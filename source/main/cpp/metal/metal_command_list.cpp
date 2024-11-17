#include "cbase/c_integer.h"
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

        //         void MetalCommandList::Wait(fence_t* fence, u64 value) { m_pCommandBuffer->encodeWait((const MTL::Event*)fence->GetHandle(), value); }
        //         void MetalCommandList::Signal(fence_t* fence, u64 value) { m_pCommandBuffer->encodeSignalEvent((const MTL::Event*)fence->GetHandle(), value); }
        //         void MetalCommandList::Present(swapchain_t* swapchain) { m_pCommandBuffer->presentDrawable(((MetalSwapchain*)swapchain)->GetDrawable()); }

        //         void MetalCommandList::Submit()
        //         {
        //             m_pCommandBuffer->commit();
        //             m_pCommandBuffer = nullptr;
        //         }

        //         void MetalCommandList::ResetState()
        //         {
        //             m_pBlitCommandEncoder    = nullptr;
        //             m_pRenderCommandEncoder  = nullptr;
        //             m_pComputeCommandEncoder = nullptr;
        //             m_pASEncoder             = nullptr;
        //             m_pCurrentPSO = nullptr;
        //             m_pIndexBuffer      = nullptr;
        //             m_indexBufferOffset = 0;
        //             m_indexType         = MTL::IndexTypeUInt16;
        //             m_primitiveType     = MTL::PrimitiveTypeTriangle;
        //             m_cullMode          = MTL::CullModeNone;
        //             m_frontFaceWinding  = MTL::WindingClockwise;
        //             m_fillMode          = MTL::TriangleFillModeFill;
        //             m_clipMode          = MTL::DepthClipModeClip;
        //             m_depthBias         = 0.0f;
        //             m_depthBiasClamp    = 0.0f;
        //             m_depthSlopeScale   = 0.0f;
        //         }

        //         void MetalCommandList::BeginProfiling() {}
        //         void MetalCommandList::EndProfiling() {}
        //         void MetalCommandList::BeginEvent(const char* event_name)
        //         {
        //             NS::String* label = NS::String::alloc()->init(event_name, NS::StringEncoding::UTF8StringEncoding);
        //             m_pCommandBuffer->pushDebugGroup(label);
        //             label->release();
        //         }

        //         void MetalCommandList::EndEvent() { m_pCommandBuffer->popDebugGroup(); }

        //         void MetalCommandList::CopyBufferToTexture(texture_t* dst_texture, u32 mip_level, u32 array_slice, buffer_t* src_buffer, u32 offset)
        //         {
        //             BeginBlitEncoder();

        //             const texture_desc_t& desc        = dst_texture->GetDesc();
        //             MTL::Size             textureSize = MTL::Size::Make(math::max(desc.width >> mip_level, 1u), math::max(desc.height >> mip_level, 1u), math::max(desc.depth >> mip_level, 1u));

        //             u32 bytesPerRow = ((MetalTexture*)dst_texture)->GetRowPitch(mip_level);

        //             u32 block_height  = GetFormatBlockHeight(desc.format);
        //             u32 height        = math::max(desc.height >> mip_level, block_height);
        //             u32 row_num       = height / block_height;
        //             u32 bytesPerImage = bytesPerRow * row_num;

        //             m_pBlitCommandEncoder->copyFromBuffer((MTL::Buffer*)src_buffer->GetHandle(), offset, bytesPerRow, desc.type == GfxTextureType::Texture3D ? bytesPerImage : 0, textureSize, (MTL::Texture*)dst_texture->GetHandle(), array_slice,
        //             mip_level,
        //                                                   MTL::Origin::Make(0, 0, 0));
        //         }

        //         void MetalCommandList::CopyTextureToBuffer(buffer_t* dst_buffer, u32 offset, texture_t* src_texture, u32 mip_level, u32 array_slice)
        //         {
        //             BeginBlitEncoder();

        //             const texture_desc_t& desc        = src_texture->GetDesc();
        //             MTL::Size             textureSize = MTL::Size::Make(math::max(desc.width >> mip_level, 1u), math::max(desc.height >> mip_level, 1u), math::max(desc.depth >> mip_level, 1u));

        //             u32 bytesPerRow = ((MetalTexture*)src_texture)->GetRowPitch(mip_level);

        //             u32 block_height  = GetFormatBlockHeight(desc.format);
        //             u32 height        = math::max(desc.height >> mip_level, block_height);
        //             u32 row_num       = height / block_height;
        //             u32 bytesPerImage = bytesPerRow * row_num;

        //             m_pBlitCommandEncoder->copyFromTexture((MTL::Texture*)src_texture->GetHandle(), array_slice, mip_level, MTL::Origin(0, 0, 0), textureSize, (MTL::Buffer*)dst_buffer->GetHandle(), offset, bytesPerRow,
        //                                                    desc.type == GfxTextureType::Texture3D ? bytesPerImage : 0);
        //         }

        //         void MetalCommandList::CopyBuffer(buffer_t* dst, u32 dst_offset, buffer_t* src, u32 src_offset, u32 size)
        //         {
        //             BeginBlitEncoder();

        //             m_pBlitCommandEncoder->copyFromBuffer((MTL::Buffer*)src->GetHandle(), src_offset, (MTL::Buffer*)dst->GetHandle(), dst_offset, size);
        //         }

        //         void MetalCommandList::CopyTexture(texture_t* dst, u32 dst_mip, u32 dst_array, texture_t* src, u32 src_mip, u32 src_array)
        //         {
        //             BeginBlitEncoder();

        //             const texture_desc_t& desc     = src->GetDesc();
        //             MTL::Size             src_size = MTL::Size::Make(math::max(desc.width >> src_mip, 1u), math::max(desc.height >> src_mip, 1u), math::max(desc.depth >> src_mip, 1u));

        //             m_pBlitCommandEncoder->copyFromTexture((MTL::Texture*)src->GetHandle(), src_array, src_mip, MTL::Origin(0, 0, 0), src_size, (MTL::Texture*)dst->GetHandle(), dst_array, dst_mip, MTL::Origin(0, 0, 0));
        //         }

        //         void MetalCommandList::ClearUAV(resource_t* resource, descriptor_t* uav, const float* clear_value, IGfxClearUavApi* clear_api)
        //         {
        //             const uav_desc_t& desc = ((MetalUnorderedAccessView*)uav)->GetDesc();
        //             clear_api->ClearUAV(this, resource, uav, desc, clear_value);
        //         }

        //         void MetalCommandList::ClearUAV(resource_t* resource, descriptor_t* uav, const u32* clear_value, IGfxClearUavApi* clear_api)
        //         {
        //             const uav_desc_t& desc = ((MetalUnorderedAccessView*)uav)->GetDesc();
        //             clear_api->ClearUAV(this, resource, uav, desc, clear_value);
        //         }

        //         void MetalCommandList::WriteBuffer(buffer_t* buffer, u32 offset, u32 data)
        //         {
        //             BeginBlitEncoder();

        //             // TODO handle events
        //             // BeginEvent("command_list_t::WriteBuffer");

        //             MTL::Buffer* mtlBuffer = (MTL::Buffer*)buffer->GetHandle();
        //             m_pBlitCommandEncoder->fillBuffer(mtlBuffer, NS::Range::Make(offset + 0, sizeof(u8)), u8(data >> 0));
        //             m_pBlitCommandEncoder->fillBuffer(mtlBuffer, NS::Range::Make(offset + 1, sizeof(u8)), u8(data >> 8));
        //             m_pBlitCommandEncoder->fillBuffer(mtlBuffer, NS::Range::Make(offset + 2, sizeof(u8)), u8(data >> 16));
        //             m_pBlitCommandEncoder->fillBuffer(mtlBuffer, NS::Range::Make(offset + 3, sizeof(u8)), u8(data >> 24));

        //             // EndEvent();
        //         }

        //         void MetalCommandList::UpdateTileMappings(texture_t* texture, heap_t* heap, u32 mapping_count, const tile_mapping_t* mappings)
        //         {
        //             // todo
        //         }

        //         void MetalCommandList::TextureBarrier(texture_t* texture, u32 sub_resource, GfxAccessFlags access_before, GfxAccessFlags access_after) {}

        //         void MetalCommandList::BufferBarrier(buffer_t* buffer, GfxAccessFlags access_before, GfxAccessFlags access_after) {}

        //         void MetalCommandList::GlobalBarrier(GfxAccessFlags access_before, GfxAccessFlags access_after) {}

        //         void MetalCommandList::FlushBarriers() {}

        //         void MetalCommandList::BeginRenderPass(const renderpass_desc_t& render_pass)
        //         {
        //             EndBlitEncoder();
        //             EndComputeEncoder();
        //             EndASEncoder();

        //             u32 width  = 0;
        //             u32 height = 0;

        //             MTL::RenderPassDescriptor*                     descriptor        = MTL::RenderPassDescriptor::alloc()->init();
        //             MTL::RenderPassColorAttachmentDescriptorArray* colorAttachements = descriptor->colorAttachments();

        //             for (u32 i = 0; i < 8; ++i)
        //             {
        //                 if (render_pass.color[i].texture == nullptr)
        //                 {
        //                     continue;
        //                 }

        //                 if (width == 0)
        //                 {
        //                     width = render_pass.color[i].texture->GetDesc().width;
        //                 }

        //                 if (height == 0)
        //                 {
        //                     height = render_pass.color[i].texture->GetDesc().height;
        //                 }

        //                 ASSERT(width == render_pass.color[i].texture->GetDesc().width);
        //                 ASSERT(height == render_pass.color[i].texture->GetDesc().height);

        //                 colorAttachements->object(i)->setTexture((MTL::Texture*)render_pass.color[i].texture->GetHandle());
        //                 colorAttachements->object(i)->setLevel(render_pass.color[i].mip_slice);
        //                 colorAttachements->object(i)->setSlice(render_pass.color[i].array_slice);
        //                 colorAttachements->object(i)->setLoadAction(ToLoadAction(render_pass.color[i].load_op));
        //                 colorAttachements->object(i)->setStoreAction(ToStoreAction(render_pass.color[i].store_op));
        //                 colorAttachements->object(i)->setClearColor(MTL::ClearColor::Make(render_pass.color[i].clear_color[0], render_pass.color[i].clear_color[1], render_pass.color[i].clear_color[2], render_pass.color[i].clear_color[3]));
        //             }

        //             if (render_pass.depth.texture != nullptr)
        //             {
        //                 if (width == 0)
        //                 {
        //                     width = render_pass.depth.texture->GetDesc().width;
        //                 }

        //                 if (height == 0)
        //                 {
        //                     height = render_pass.depth.texture->GetDesc().height;
        //                 }

        //                 ASSERT(width == render_pass.depth.texture->GetDesc().width);
        //                 ASSERT(height == render_pass.depth.texture->GetDesc().height);

        //                 MTL::RenderPassDepthAttachmentDescriptor* depthAttachment = descriptor->depthAttachment();
        //                 depthAttachment->setTexture((MTL::Texture*)render_pass.depth.texture->GetHandle());
        //                 depthAttachment->setLevel(render_pass.depth.mip_slice);
        //                 depthAttachment->setSlice(render_pass.depth.array_slice);
        //                 depthAttachment->setLoadAction(ToLoadAction(render_pass.depth.load_op));
        //                 depthAttachment->setStoreAction(ToStoreAction(render_pass.depth.store_op));
        //                 depthAttachment->setClearDepth(render_pass.depth.clear_depth);

        //                 if (IsStencilFormat(render_pass.depth.texture->GetDesc().format))
        //                 {
        //                     MTL::RenderPassStencilAttachmentDescriptor* stencilAttachment = descriptor->stencilAttachment();
        //                     stencilAttachment->setTexture((MTL::Texture*)render_pass.depth.texture->GetHandle());
        //                     stencilAttachment->setLevel(render_pass.depth.mip_slice);
        //                     stencilAttachment->setSlice(render_pass.depth.array_slice);
        //                     stencilAttachment->setLoadAction(ToLoadAction(render_pass.depth.stencil_load_op));
        //                     stencilAttachment->setStoreAction(ToStoreAction(render_pass.depth.stencil_store_op));
        //                     stencilAttachment->setClearStencil(render_pass.depth.clear_stencil);
        //                 }
        //             }

        //             ASSERT(m_pRenderCommandEncoder == nullptr);
        //             m_pRenderCommandEncoder = m_pCommandBuffer->renderCommandEncoder(descriptor);
        //             m_pRenderCommandEncoder->waitForFence(m_pFence, MTL::RenderStageVertex | MTL::RenderStageObject);
        //             descriptor->release();

        //             MetalDevice* pDevice = (MetalDevice*)m_pDevice;
        //             m_pRenderCommandEncoder->setVertexBuffer(pDevice->GetResourceDescriptorBuffer(), 0, kIRDescriptorHeapBindPoint);
        //             m_pRenderCommandEncoder->setVertexBuffer(pDevice->GetSamplerDescriptorBuffer(), 0, kIRSamplerHeapBindPoint);
        //             m_pRenderCommandEncoder->setFragmentBuffer(pDevice->GetResourceDescriptorBuffer(), 0, kIRDescriptorHeapBindPoint);
        //             m_pRenderCommandEncoder->setFragmentBuffer(pDevice->GetSamplerDescriptorBuffer(), 0, kIRSamplerHeapBindPoint);

        //             SetViewport(0, 0, width, height);
        //         }

        //         void MetalCommandList::EndRenderPass()
        //         {
        //             if (m_pRenderCommandEncoder)
        //             {
        //                 m_pRenderCommandEncoder->updateFence(m_pFence, MTL::RenderStageFragment);
        //                 m_pRenderCommandEncoder->endEncoding();
        //                 m_pRenderCommandEncoder = nullptr;
        //                 ResetState();
        //             }
        //         }

        //         void MetalCommandList::SetPipelineState(pipeline_state_t* state)
        //         {
        //             if (m_pCurrentPSO != state)
        //             {
        //                 m_pCurrentPSO = state;

        //                 if (state->GetType() != GfxPipelineType::Compute)
        //                 {
        //                     ASSERT(m_pRenderCommandEncoder != nullptr);

        //                     m_pRenderCommandEncoder->setRenderPipelineState((MTL::RenderPipelineState*)state->GetHandle());

        //                     if (state->GetType() == GfxPipelineType::Graphics)
        //                     {
        //                         MetalGraphicsPipelineState*    graphicsPSO = (MetalGraphicsPipelineState*)state;
        //                         const graphics_pipeline_desc_t& desc        = graphicsPSO->GetDesc();

        //                         m_pRenderCommandEncoder->setDepthStencilState(graphicsPSO->GetDepthStencilState());
        //                         SetRasterizerState(desc.rasterizer_state);
        //                         m_primitiveType = ToPrimitiveType(desc.primitive_type);
        //                     }
        //                     else
        //                     {
        //                         MetalMeshShadingPipelineState*    meshShadingPSO = (MetalMeshShadingPipelineState*)state;
        //                         const mesh_shading_pipeline_desc_t& desc           = meshShadingPSO->GetDesc();

        //                         m_pRenderCommandEncoder->setDepthStencilState(meshShadingPSO->GetDepthStencilState());
        //                         SetRasterizerState(desc.rasterizer_state);
        //                     }
        //                 }
        //             }
        //         }

        //         void MetalCommandList::SetStencilReference(u8 stencil)
        //         {
        //             ASSERT(m_pRenderCommandEncoder != nullptr);
        //             m_pRenderCommandEncoder->setStencilReferenceValue(stencil);
        //         }

        //         void MetalCommandList::SetBlendFactor(const float* blend_factor)
        //         {
        //             ASSERT(m_pRenderCommandEncoder != nullptr);
        //             m_pRenderCommandEncoder->setBlendColor(blend_factor[0], blend_factor[1], blend_factor[2], blend_factor[3]);
        //         }

        //         void MetalCommandList::SetIndexBuffer(buffer_t* buffer, u32 offset, GfxFormat format)
        //         {
        //             ASSERT(m_pRenderCommandEncoder != nullptr);

        //             m_pIndexBuffer      = (MTL::Buffer*)buffer->GetHandle();
        //             m_indexBufferOffset = offset;
        //             m_indexType         = format == Gfx::R16UI ? MTL::IndexTypeUInt16 : MTL::IndexTypeUInt32;
        //         }

        //         void MetalCommandList::SetViewport(u32 x, u32 y, u32 width, u32 height)
        //         {
        //             ASSERT(m_pRenderCommandEncoder != nullptr);

        //             MTL::Viewport viewport;  // = {x, y, width, height, 0.0, 1.0};
        //             viewport.originX = x;
        //             viewport.originY = y;
        //             viewport.width   = width;
        //             viewport.height  = height;
        //             viewport.znear   = 0.0;
        //             viewport.zfar    = 1.0;
        //             m_pRenderCommandEncoder->setViewport(viewport);

        //             SetScissorRect(x, y, width, height);
        //         }

        //         void MetalCommandList::SetScissorRect(u32 x, u32 y, u32 width, u32 height)
        //         {
        //             ASSERT(m_pRenderCommandEncoder != nullptr);

        //             MTL::ScissorRect scissorRect = {x, y, width, height};
        //             m_pRenderCommandEncoder->setScissorRect(scissorRect);
        //         }

        //         void MetalCommandList::SetGraphicsConstants(u32 slot, const void* data, s64 data_size)
        //         {
        //             if (slot == 0)
        //             {
        //                 ASSERT(data_size <= GFX_MAX_ROOT_CONSTANTS * sizeof(u32));
        //                 memcpy(m_graphicsArgumentBuffer.cbv0, data, data_size);
        //             }
        //             else
        //             {
        //                 ASSERT(slot < GFX_MAX_CBV_BINDINGS);
        //                 u64 gpuAddress = ((MetalDevice*)m_pDevice)->AllocateConstantBuffer(data, data_size);

        //                 if (slot == 1)
        //                 {
        //                     m_graphicsArgumentBuffer.cbv1 = gpuAddress;
        //                 }
        //                 else
        //                 {
        //                     m_graphicsArgumentBuffer.cbv2 = gpuAddress;
        //                 }
        //             }
        //         }

        //         void MetalCommandList::SetComputeConstants(u32 slot, const void* data, s64 data_size)
        //         {
        //             if (slot == 0)
        //             {
        //                 ASSERT(data_size <= GFX_MAX_ROOT_CONSTANTS * sizeof(u32));
        //                 memcpy(m_computeArgumentBuffer.cbv0, data, data_size);
        //             }
        //             else
        //             {
        //                 ASSERT(slot < GFX_MAX_CBV_BINDINGS);
        //                 u64 gpuAddress = ((MetalDevice*)m_pDevice)->AllocateConstantBuffer(data, data_size);

        //                 if (slot == 1)
        //                 {
        //                     m_computeArgumentBuffer.cbv1 = gpuAddress;
        //                 }
        //                 else
        //                 {
        //                     m_computeArgumentBuffer.cbv2 = gpuAddress;
        //                 }
        //             }
        //         }

        //         void MetalCommandList::Draw(u32 vertex_count, u32 instance_count)
        //         {
        //             ASSERT(m_pRenderCommandEncoder != nullptr);

        //             m_pRenderCommandEncoder->setVertexBytes(&m_graphicsArgumentBuffer, sizeof(TopLevelArgumentBuffer), kIRArgumentBufferBindPoint);
        //             m_pRenderCommandEncoder->setFragmentBytes(&m_graphicsArgumentBuffer, sizeof(TopLevelArgumentBuffer), kIRArgumentBufferBindPoint);

        //             IRRuntimeDrawPrimitives(m_pRenderCommandEncoder, m_primitiveType, 0, vertex_count, instance_count);
        //         }

        //         void MetalCommandList::DrawIndexed(u32 index_count, u32 instance_count, u32 index_offset)
        //         {
        //             ASSERT(m_pRenderCommandEncoder != nullptr);

        //             m_pRenderCommandEncoder->setVertexBytes(&m_graphicsArgumentBuffer, sizeof(TopLevelArgumentBuffer), kIRArgumentBufferBindPoint);
        //             m_pRenderCommandEncoder->setFragmentBytes(&m_graphicsArgumentBuffer, sizeof(TopLevelArgumentBuffer), kIRArgumentBufferBindPoint);

        //             u64 indexBufferOffset = m_indexBufferOffset;
        //             if (m_indexType == MTL::IndexTypeUInt16)
        //             {
        //                 indexBufferOffset += sizeof(uint16_t) * index_offset;
        //             }
        //             else
        //             {
        //                 indexBufferOffset += sizeof(u32) * index_offset;
        //             }

        //             IRRuntimeDrawIndexedPrimitives(m_pRenderCommandEncoder, m_primitiveType, index_count, m_indexType, m_pIndexBuffer, indexBufferOffset, instance_count, 0, 0);
        //         }

        //         void MetalCommandList::Dispatch(u32 group_count_x, u32 group_count_y, u32 group_count_z)
        //         {
        //             BeginComputeEncoder();

        //             m_pComputeCommandEncoder->setComputePipelineState((MTL::ComputePipelineState*)m_pCurrentPSO->GetHandle());
        //             m_pComputeCommandEncoder->setBytes(&m_computeArgumentBuffer, sizeof(TopLevelArgumentBuffer), kIRArgumentBufferBindPoint);

        //             MTL::Size threadgroupsPerGrid   = MTL::Size::Make(group_count_x, group_count_y, group_count_z);
        //             MTL::Size threadsPerThreadgroup = ((MetalComputePipelineState*)m_pCurrentPSO)->GetThreadsPerThreadgroup();
        //             m_pComputeCommandEncoder->dispatchThreadgroups(threadgroupsPerGrid, threadsPerThreadgroup);

        //             EndComputeEncoder();
        //         }

        //         void MetalCommandList::DispatchMesh(u32 group_count_x, u32 group_count_y, u32 group_count_z)
        //         {
        //             ASSERT(m_pRenderCommandEncoder != nullptr);

        //             MetalDevice* pDevice = (MetalDevice*)m_pDevice;
        //             m_pRenderCommandEncoder->setObjectBuffer(pDevice->GetResourceDescriptorBuffer(), 0, kIRDescriptorHeapBindPoint);
        //             m_pRenderCommandEncoder->setObjectBuffer(pDevice->GetSamplerDescriptorBuffer(), 0, kIRSamplerHeapBindPoint);
        //             m_pRenderCommandEncoder->setMeshBuffer(pDevice->GetResourceDescriptorBuffer(), 0, kIRDescriptorHeapBindPoint);
        //             m_pRenderCommandEncoder->setMeshBuffer(pDevice->GetSamplerDescriptorBuffer(), 0, kIRSamplerHeapBindPoint);

        //             m_pRenderCommandEncoder->setObjectBytes(&m_graphicsArgumentBuffer, sizeof(TopLevelArgumentBuffer), kIRArgumentBufferBindPoint);
        //             m_pRenderCommandEncoder->setMeshBytes(&m_graphicsArgumentBuffer, sizeof(TopLevelArgumentBuffer), kIRArgumentBufferBindPoint);
        //             m_pRenderCommandEncoder->setFragmentBytes(&m_graphicsArgumentBuffer, sizeof(TopLevelArgumentBuffer), kIRArgumentBufferBindPoint);

        //             MTL::Size threadgroupsPerGrid         = MTL::Size::Make(group_count_x, group_count_y, group_count_z);
        //             MTL::Size threadsPerObjectThreadgroup = ((MetalMeshShadingPipelineState*)m_pCurrentPSO)->GetThreadsPerObjectThreadgroup();
        //             MTL::Size threadsPerMeshThreadgroup   = ((MetalMeshShadingPipelineState*)m_pCurrentPSO)->GetThreadsPerMeshThreadgroup();

        //             m_pRenderCommandEncoder->drawMeshThreadgroups(threadgroupsPerGrid, threadsPerObjectThreadgroup, threadsPerMeshThreadgroup);
        //         }

        //         void MetalCommandList::DrawIndirect(buffer_t* buffer, u32 offset)
        //         {
        //             ASSERT(m_pRenderCommandEncoder != nullptr);

        //             m_pRenderCommandEncoder->setVertexBytes(&m_graphicsArgumentBuffer, sizeof(TopLevelArgumentBuffer), kIRArgumentBufferBindPoint);
        //             m_pRenderCommandEncoder->setFragmentBytes(&m_graphicsArgumentBuffer, sizeof(TopLevelArgumentBuffer), kIRArgumentBufferBindPoint);

        //             IRRuntimeDrawPrimitives(m_pRenderCommandEncoder, m_primitiveType, (MTL::Buffer*)buffer->GetHandle(), offset);
        //         }

        //         void MetalCommandList::DrawIndexedIndirect(buffer_t* buffer, u32 offset)
        //         {
        //             ASSERT(m_pRenderCommandEncoder != nullptr);

        //             m_pRenderCommandEncoder->setVertexBytes(&m_graphicsArgumentBuffer, sizeof(TopLevelArgumentBuffer), kIRArgumentBufferBindPoint);
        //             m_pRenderCommandEncoder->setFragmentBytes(&m_graphicsArgumentBuffer, sizeof(TopLevelArgumentBuffer), kIRArgumentBufferBindPoint);

        //             IRRuntimeDrawIndexedPrimitives(m_pRenderCommandEncoder, m_primitiveType, m_indexType, m_pIndexBuffer, m_indexBufferOffset, (MTL::Buffer*)buffer->GetHandle(), offset);
        //         }

        //         void MetalCommandList::DispatchIndirect(buffer_t* buffer, u32 offset)
        //         {
        //             BeginComputeEncoder();

        //             m_pComputeCommandEncoder->setComputePipelineState((MTL::ComputePipelineState*)m_pCurrentPSO->GetHandle());
        //             m_pComputeCommandEncoder->setBytes(&m_computeArgumentBuffer, sizeof(TopLevelArgumentBuffer), kIRArgumentBufferBindPoint);

        //             MTL::Size threadsPerThreadgroup = ((MetalComputePipelineState*)m_pCurrentPSO)->GetThreadsPerThreadgroup();
        //             m_pComputeCommandEncoder->dispatchThreadgroups((MTL::Buffer*)buffer->GetHandle(), offset, threadsPerThreadgroup);

        //             EndComputeEncoder();
        //         }

        //         void MetalCommandList::DispatchMeshIndirect(buffer_t* buffer, u32 offset)
        //         {
        //             ASSERT(m_pRenderCommandEncoder != nullptr);

        //             MetalDevice* pDevice = (MetalDevice*)m_pDevice;
        //             m_pRenderCommandEncoder->setObjectBuffer(pDevice->GetResourceDescriptorBuffer(), 0, kIRDescriptorHeapBindPoint);
        //             m_pRenderCommandEncoder->setObjectBuffer(pDevice->GetSamplerDescriptorBuffer(), 0, kIRSamplerHeapBindPoint);
        //             m_pRenderCommandEncoder->setMeshBuffer(pDevice->GetResourceDescriptorBuffer(), 0, kIRDescriptorHeapBindPoint);
        //             m_pRenderCommandEncoder->setMeshBuffer(pDevice->GetSamplerDescriptorBuffer(), 0, kIRSamplerHeapBindPoint);

        //             m_pRenderCommandEncoder->setObjectBytes(&m_graphicsArgumentBuffer, sizeof(TopLevelArgumentBuffer), kIRArgumentBufferBindPoint);
        //             m_pRenderCommandEncoder->setMeshBytes(&m_graphicsArgumentBuffer, sizeof(TopLevelArgumentBuffer), kIRArgumentBufferBindPoint);
        //             m_pRenderCommandEncoder->setFragmentBytes(&m_graphicsArgumentBuffer, sizeof(TopLevelArgumentBuffer), kIRArgumentBufferBindPoint);

        //             MTL::Size threadsPerObjectThreadgroup = ((MetalMeshShadingPipelineState*)m_pCurrentPSO)->GetThreadsPerObjectThreadgroup();
        //             MTL::Size threadsPerMeshThreadgroup   = ((MetalMeshShadingPipelineState*)m_pCurrentPSO)->GetThreadsPerMeshThreadgroup();

        //             m_pRenderCommandEncoder->drawMeshThreadgroups((MTL::Buffer*)buffer->GetHandle(), offset, threadsPerObjectThreadgroup, threadsPerMeshThreadgroup);
        //         }

        //         void MetalCommandList::MultiDrawIndirect(u32 max_count, buffer_t* args_buffer, u32 args_buffer_offset, buffer_t* count_buffer, u32 count_buffer_offset)
        //         {
        //             ASSERT(m_pRenderCommandEncoder != nullptr);

        //             // todo
        //         }

        //         void MetalCommandList::MultiDrawIndexedIndirect(u32 max_count, buffer_t* args_buffer, u32 args_buffer_offset, buffer_t* count_buffer, u32 count_buffer_offset)
        //         {
        //             ASSERT(m_pRenderCommandEncoder != nullptr);

        //             // todo
        //         }

        //         void MetalCommandList::MultiDispatchIndirect(u32 max_count, buffer_t* args_buffer, u32 args_buffer_offset, buffer_t* count_buffer, u32 count_buffer_offset)
        //         {
        //             // todo
        //         }

        //         void MetalCommandList::MultiDispatchMeshIndirect(u32 max_count, buffer_t* args_buffer, u32 args_buffer_offset, buffer_t* count_buffer, u32 count_buffer_offset)
        //         {
        //             ASSERT(m_pRenderCommandEncoder != nullptr);

        //             // todo
        //         }

        //         void MetalCommandList::BuildRayTracingBLAS(blas_t* blas)
        //         {
        //             BeginASEncoder();

        //             MetalRayTracingBLAS* metalBLAS = (MetalRayTracingBLAS*)blas;
        //             m_pASEncoder->buildAccelerationStructure(metalBLAS->GetAccelerationStructure(), metalBLAS->GetDescriptor(), metalBLAS->GetScratchBuffer(), 0);
        //         }

        //         void MetalCommandList::UpdateRayTracingBLAS(blas_t* blas, buffer_t* vertex_buffer, u32 vertex_buffer_offset)
        //         {
        //             BeginASEncoder();

        //             MetalRayTracingBLAS* metalBLAS = (MetalRayTracingBLAS*)blas;
        //             metalBLAS->UpdateVertexBuffer(vertex_buffer, vertex_buffer_offset);

        //             m_pASEncoder->refitAccelerationStructure(metalBLAS->GetAccelerationStructure(), metalBLAS->GetDescriptor(), metalBLAS->GetAccelerationStructure(), metalBLAS->GetScratchBuffer(), 0);
        //         }

        //         void MetalCommandList::BuildRayTracingTLAS(tlas_t* tlas, const GfxRayTracingInstance* instances, u32 instance_count)
        //         {
        //             BeginASEncoder();

        //             MetalRayTracingTLAS* metalTLAS = (MetalRayTracingTLAS*)tlas;
        //             metalTLAS->UpdateInstance(instances, instance_count);

        //             m_pASEncoder->buildAccelerationStructure(metalTLAS->GetAccelerationStructure(), metalTLAS->GetDescriptor(), metalTLAS->GetScratchBuffer(), 0);
        //         }

        // #if MICROPROFILE_GPU_TIMERS
        //         MicroProfileThreadLogGpu* MetalCommandList::GetProfileLog() const { return nullptr; }
        // #endif

        //         void MetalCommandList::BeginBlitEncoder()
        //         {
        //             EndRenderPass();
        //             EndComputeEncoder();
        //             EndASEncoder();

        //             if (m_pBlitCommandEncoder == nullptr)
        //             {
        //                 m_pBlitCommandEncoder = m_pCommandBuffer->blitCommandEncoder();
        //                 m_pBlitCommandEncoder->waitForFence(m_pFence);
        //             }
        //         }

        //         void MetalCommandList::EndBlitEncoder()
        //         {
        //             if (m_pBlitCommandEncoder)
        //             {
        //                 m_pBlitCommandEncoder->updateFence(m_pFence);
        //                 m_pBlitCommandEncoder->endEncoding();
        //                 m_pBlitCommandEncoder = nullptr;
        //             }
        //         }

        //         void MetalCommandList::BeginComputeEncoder()
        //         {
        //             EndRenderPass();
        //             EndBlitEncoder();
        //             EndASEncoder();

        //             if (m_pComputeCommandEncoder == nullptr)
        //             {
        //                 m_pComputeCommandEncoder = m_pCommandBuffer->computeCommandEncoder();
        //                 m_pComputeCommandEncoder->waitForFence(m_pFence);

        //                 m_pComputeCommandEncoder->setBuffer(((MetalDevice*)m_pDevice)->GetResourceDescriptorBuffer(), 0, kIRDescriptorHeapBindPoint);
        //                 m_pComputeCommandEncoder->setBuffer(((MetalDevice*)m_pDevice)->GetSamplerDescriptorBuffer(), 0, kIRSamplerHeapBindPoint);
        //             }
        //         }

        //         void MetalCommandList::EndComputeEncoder()
        //         {
        //             if (m_pComputeCommandEncoder)
        //             {
        //                 m_pComputeCommandEncoder->updateFence(m_pFence);
        //                 m_pComputeCommandEncoder->endEncoding();
        //                 m_pComputeCommandEncoder = nullptr;
        //             }
        //         }

        //         void MetalCommandList::BeginASEncoder()
        //         {
        //             EndRenderPass();
        //             EndBlitEncoder();
        //             EndComputeEncoder();

        //             if (m_pASEncoder == nullptr)
        //             {
        //                 m_pASEncoder = m_pCommandBuffer->accelerationStructureCommandEncoder();
        //                 m_pASEncoder->waitForFence(m_pFence);
        //             }
        //         }

        //         void MetalCommandList::EndASEncoder()
        //         {
        //             if (m_pASEncoder)
        //             {
        //                 m_pASEncoder->updateFence(m_pFence);
        //                 m_pASEncoder->endEncoding();
        //                 m_pASEncoder = nullptr;
        //             }
        //         }

        //         void MetalCommandList::SetRasterizerState(const rasterizer_state_t& state)
        //         {
        //             MTL::CullMode cullMode = ToCullMode(state.cull_mode);
        //             if (m_cullMode != cullMode)
        //             {
        //                 m_pRenderCommandEncoder->setCullMode(cullMode);
        //                 m_cullMode = cullMode;
        //             }

        //             MTL::Winding frontFaceWinding = state.front_ccw ? MTL::WindingCounterClockwise : MTL::WindingClockwise;
        //             if (m_frontFaceWinding != frontFaceWinding)
        //             {
        //                 m_pRenderCommandEncoder->setFrontFacingWinding(frontFaceWinding);
        //                 m_frontFaceWinding = frontFaceWinding;
        //             }

        //             MTL::TriangleFillMode fillMode = state.wireframe ? MTL::TriangleFillModeLines : MTL::TriangleFillModeFill;
        //             if (m_fillMode != fillMode)
        //             {
        //                 m_pRenderCommandEncoder->setTriangleFillMode(fillMode);
        //                 m_fillMode = fillMode;
        //             }

        //             MTL::DepthClipMode clipMode = state.depth_clip ? MTL::DepthClipModeClip : MTL::DepthClipModeClamp;
        //             if (m_clipMode != clipMode)
        //             {
        //                 m_pRenderCommandEncoder->setDepthClipMode(clipMode);
        //                 m_clipMode = clipMode;
        //             }

        //             if (!nearly_equal(state.depth_bias, m_depthBias) || !nearly_equal(state.depth_bias_clamp, m_depthBiasClamp) || !nearly_equal(state.depth_slope_scale, m_depthSlopeScale))
        //             {
        //                 m_pRenderCommandEncoder->setDepthBias(state.depth_bias, state.depth_slope_scale, state.depth_bias_clamp);

        //                 m_depthBias       = state.depth_bias;
        //                 m_depthBiasClamp  = state.depth_bias_clamp;
        //                 m_depthSlopeScale = state.depth_slope_scale;
        //             }
        //         }
        namespace nmetal
        {
            struct command_list_t
            {
                struct TopLevelArgumentBuffer
                {
                    uint32_t cbv0[GFX_MAX_ROOT_CONSTANTS];  // root constants
                    uint64_t cbv1;                          // gpu address
                    uint64_t cbv2;
                };

                MTL::CommandBuffer*                       m_pCommandBuffer         = nullptr;
                MTL::BlitCommandEncoder*                  m_pBlitCommandEncoder    = nullptr;
                MTL::RenderCommandEncoder*                m_pRenderCommandEncoder  = nullptr;
                MTL::ComputeCommandEncoder*               m_pComputeCommandEncoder = nullptr;
                MTL::AccelerationStructureCommandEncoder* m_pASEncoder             = nullptr;
                MTL::Fence*                               m_pFence                 = nullptr;
                pipeline_state_t*                         m_pCurrentPSO            = nullptr;
                MTL::Buffer*                              m_pIndexBuffer           = nullptr;
                NS::UInteger                              m_indexBufferOffset      = 0;
                MTL::IndexType                            m_indexType              = MTL::IndexTypeUInt16;
                MTL::PrimitiveType                        m_primitiveType          = MTL::PrimitiveTypeTriangle;
                MTL::CullMode                             m_cullMode               = MTL::CullModeNone;
                MTL::Winding                              m_frontFaceWinding       = MTL::WindingClockwise;
                MTL::TriangleFillMode                     m_fillMode               = MTL::TriangleFillModeFill;
                MTL::DepthClipMode                        m_clipMode               = MTL::DepthClipModeClip;
                float                                     m_depthBias              = 0.0f;
                float                                     m_depthBiasClamp         = 0.0f;
                float                                     m_depthSlopeScale        = 0.0f;
                TopLevelArgumentBuffer                    m_graphicsArgumentBuffer;
                TopLevelArgumentBuffer                    m_computeArgumentBuffer;
            };

            ngfx::command_list_t* CreateCommandList(ngfx::device_t* device, ngfx::command_list_t* cmdList) { nmetal::command_list_t* mcmdList = AddAnotherComponent<ngfx::command_list_t, nmetal::command_list_t>(device, cmdList); }

            bool Create(ngfx::command_list_t* cmdList)
            {
                nmetal::command_list_t* mcmdList  = GetOtherComponent<ngfx::command_list_t, nmetal::command_list_t>(cmdList->m_device, cmdList);
                nmetal::device_t*       mdevice   = GetOtherComponent<ngfx::device_t, nmetal::device_t>(cmdList->m_device, cmdList->m_device);
                MTL::Device*            mtlDevice = mdevice->m_pDevice;
                mcmdList->m_pFence                = mtlDevice->newFence();
                return true;
            }

            void Destroy(ngfx::command_list_t* cmdList)
            {
                nmetal::command_list_t* mcmdList = GetOtherComponent<ngfx::command_list_t, nmetal::command_list_t>(cmdList->m_device, cmdList);
                if (mcmdList->m_pFence)
                {
                    mcmdList->m_pFence->release();
                    mcmdList->m_pFence = nullptr;
                }
            }

            void* GetHandle(ngfx::command_list_t* cmdList)
            {
                nmetal::command_list_t* mcmdList = GetOtherComponent<ngfx::command_list_t, nmetal::command_list_t>(cmdList->m_device, cmdList);
                return mcmdList->m_pCommandBuffer;
            }

            void ResetAllocator(ngfx::command_list_t* cmdList) {}

            void Begin(ngfx::command_list_t* cmdList)
            {
                nmetal::device_t*       mdevice  = GetOtherComponent<ngfx::device_t, nmetal::device_t>(cmdList->m_device, cmdList->m_device);
                MTL::CommandQueue*      queue    = mdevice->m_pQueue;
                nmetal::command_list_t* mcmdList = GetOtherComponent<ngfx::command_list_t, nmetal::command_list_t>(cmdList->m_device, cmdList);
                mcmdList->m_pCommandBuffer       = queue->commandBuffer();
            }

            void ResetState(nmetal::command_list_t* cmdList)
            {
                cmdList->m_pBlitCommandEncoder    = nullptr;
                cmdList->m_pRenderCommandEncoder  = nullptr;
                cmdList->m_pComputeCommandEncoder = nullptr;
                cmdList->m_pASEncoder             = nullptr;
                cmdList->m_pCurrentPSO            = nullptr;
                cmdList->m_pIndexBuffer           = nullptr;
                cmdList->m_indexBufferOffset      = 0;
                cmdList->m_indexType              = MTL::IndexTypeUInt16;
                cmdList->m_primitiveType          = MTL::PrimitiveTypeTriangle;
                cmdList->m_cullMode               = MTL::CullModeNone;
                cmdList->m_frontFaceWinding       = MTL::WindingClockwise;
                cmdList->m_fillMode               = MTL::TriangleFillModeFill;
                cmdList->m_clipMode               = MTL::DepthClipModeClip;
                cmdList->m_depthBias              = 0.0f;
                cmdList->m_depthBiasClamp         = 0.0f;
                cmdList->m_depthSlopeScale        = 0.0f;
            }

            void EndBlitEncoder(nmetal::command_list_t* cmdList)
            {
                if (cmdList->m_pBlitCommandEncoder)
                {
                    cmdList->m_pBlitCommandEncoder->updateFence(cmdList->m_pFence);
                    cmdList->m_pBlitCommandEncoder->endEncoding();
                    cmdList->m_pBlitCommandEncoder = nullptr;
                }
            }

            void EndComputeEncoder(nmetal::command_list_t* cmdList)
            {
                if (cmdList->m_pComputeCommandEncoder)
                {
                    cmdList->m_pComputeCommandEncoder->updateFence(cmdList->m_pFence);
                    cmdList->m_pComputeCommandEncoder->endEncoding();
                    cmdList->m_pComputeCommandEncoder = nullptr;
                }
            }

            void EndRenderPass(nmetal::command_list_t* cmdList)
            {
                if (cmdList->m_pRenderCommandEncoder)
                {
                    cmdList->m_pRenderCommandEncoder->updateFence(cmdList->m_pFence, MTL::RenderStageFragment);
                    cmdList->m_pRenderCommandEncoder->endEncoding();
                    cmdList->m_pRenderCommandEncoder = nullptr;
                    ResetState(cmdList);
                }
            }

            void End(ngfx::command_list_t* cmdList)
            {
                nmetal::command_list_t* mcmdList = GetOtherComponent<ngfx::command_list_t, nmetal::command_list_t>(cmdList->m_device, cmdList);
                EndBlitEncoder(mcmdList);
                EndComputeEncoder(mcmdList);
                EndRenderPass(mcmdList);
            }

            void Wait(ngfx::command_list_t* cmdList, ngfx::fence_t* fence, u64 value);
            void Signal(ngfx::command_list_t* cmdList, ngfx::fence_t* fence, u64 value);
            void Present(ngfx::command_list_t* cmdList, swapchain_t* swapchain);

            void Wait(ngfx::command_list_t* cmdList, ngfx::fence_t* fence, u64 value)
            {
                nmetal::command_list_t* mcmdList = GetOtherComponent<ngfx::command_list_t, nmetal::command_list_t>(cmdList->m_device, cmdList);
                nmetal::fence_t*        mfence   = GetOtherComponent<ngfx::fence_t, nmetal::fence_t>(cmdList->m_device, fence);
                mcmdList->m_pCommandBuffer->encodeWait(mfence->m_pEvent, value);
            }

            void Signal(ngfx::command_list_t* cmdList, ngfx::fence_t* fence, u64 value)
            {
                nmetal::command_list_t* mcmdList = GetOtherComponent<ngfx::command_list_t, nmetal::command_list_t>(cmdList->m_device, cmdList);
                nmetal::fence_t*        mfence   = GetOtherComponent<ngfx::fence_t, nmetal::fence_t>(cmdList->m_device, fence);
                mcmdList->m_pCommandBuffer->encodeSignalEvent(mfence->m_pEvent, value);
            }

            void Present(ngfx::command_list_t* cmdList, ngfx::swapchain_t* swapchain)
            {
                nmetal::command_list_t* mcmdList   = GetOtherComponent<ngfx::command_list_t, nmetal::command_list_t>(cmdList->m_device, cmdList);
                nmetal::swapchain_t*    mswapchain = GetOtherComponent<ngfx::swapchain_t, nmetal::swapchain_t>(cmdList->m_device, swapchain);
                // mcmdList->m_pCommandBuffer->presentDrawable(((MetalSwapchain*)swapchain)->GetDrawable());
                mcmdList->m_pCommandBuffer->presentDrawable(mswapchain->m_pView->currentDrawable());
            }

            void Submit(ngfx::command_list_t* cmdList)
            {
                nmetal::command_list_t* mcmdList = GetOtherComponent<ngfx::command_list_t, nmetal::command_list_t>(cmdList->m_device, cmdList);
                mcmdList->m_pCommandBuffer->commit();
                mcmdList->m_pCommandBuffer = nullptr;
            }

            void ResetState(ngfx::command_list_t* cmdList)
            {
                nmetal::command_list_t* mcmdList = GetOtherComponent<ngfx::command_list_t, nmetal::command_list_t>(cmdList->m_device, cmdList);
                ResetState(mcmdList);
            }

            void BeginProfiling(ngfx::command_list_t* cmdList);
            void EndProfiling(ngfx::command_list_t* cmdList);
            void BeginEvent(ngfx::command_list_t* cmdList, const char* event_name);
            void EndEvent(ngfx::command_list_t* cmdList);

            void CopyBufferToTexture(ngfx::command_list_t* cmdList, texture_t* dst_texture, u32 mip_level, u32 array_slice, buffer_t* src_buffer, u32 offset);
            void CopyTextureToBuffer(ngfx::command_list_t* cmdList, buffer_t* dst_buffer, u32 offset, texture_t* src_texture, u32 mip_level, u32 array_slice);
            void CopyBuffer(ngfx::command_list_t* cmdList, buffer_t* dst, u32 dst_offset, buffer_t* src, u32 src_offset, u32 size);
            void CopyTexture(ngfx::command_list_t* cmdList, texture_t* dst, u32 dst_mip, u32 dst_array, texture_t* src, u32 src_mip, u32 src_array);
            void ClearUAV(ngfx::command_list_t* cmdList, resource_t* resource, descriptor_t* uav, const float* clear_value);
            void ClearUAV(ngfx::command_list_t* cmdList, resource_t* resource, descriptor_t* uav, const u32* clear_value);
            void WriteBuffer(ngfx::command_list_t* cmdList, buffer_t* buffer, u32 offset, u32 data);
            void UpdateTileMappings(ngfx::command_list_t* cmdList, texture_t* texture, heap_t* heap, u32 mapping_count, const tile_mapping_t* mappings);

            void TextureBarrier(ngfx::command_list_t* cmdList, texture_t* texture, u32 sub_resource, enums::access_flags access_before, enums::access_flags access_after);
            void BufferBarrier(ngfx::command_list_t* cmdList, buffer_t* buffer, enums::access_flags access_before, enums::access_flags access_after);
            void GlobalBarrier(ngfx::command_list_t* cmdList, enums::access_flags access_before, enums::access_flags access_after);
            void FlushBarriers(ngfx::command_list_t* cmdList);

            void BeginRenderPass(ngfx::command_list_t* cmdList, const renderpass_desc_t& render_pass);
            void EndRenderPass(ngfx::command_list_t* cmdList);
            void SetPipelineState(ngfx::command_list_t* cmdList, pipeline_state_t* state);
            void SetStencilReference(ngfx::command_list_t* cmdList, u8 stencil);
            void SetBlendFactor(ngfx::command_list_t* cmdList, const float* blend_factor);
            void SetIndexBuffer(ngfx::command_list_t* cmdList, buffer_t* buffer, u32 offset, enums::format format);
            void SetViewport(ngfx::command_list_t* cmdList, u32 x, u32 y, u32 width, u32 height);
            void SetScissorRect(ngfx::command_list_t* cmdList, u32 x, u32 y, u32 width, u32 height);
            void SetGraphicsConstants(ngfx::command_list_t* cmdList, u32 slot, const void* data, s64 data_size);
            void SetComputeConstants(ngfx::command_list_t* cmdList, u32 slot, const void* data, s64 data_size);

            void Draw(ngfx::command_list_t* cmdList, u32 vertex_count, u32 instance_count = 1);
            void DrawIndexed(ngfx::command_list_t* cmdList, u32 index_count, u32 instance_count = 1, u32 index_offset = 0);
            void Dispatch(ngfx::command_list_t* cmdList, u32 group_count_x, u32 group_count_y, u32 group_count_z);
            void DispatchMesh(ngfx::command_list_t* cmdList, u32 group_count_x, u32 group_count_y, u32 group_count_z);

            void DrawIndirect(ngfx::command_list_t* cmdList, buffer_t* buffer, u32 offset);
            void DrawIndexedIndirect(ngfx::command_list_t* cmdList, buffer_t* buffer, u32 offset);
            void DispatchIndirect(ngfx::command_list_t* cmdList, buffer_t* buffer, u32 offset);
            void DispatchMeshIndirect(ngfx::command_list_t* cmdList, buffer_t* buffer, u32 offset);

            void MultiDrawIndirect(ngfx::command_list_t* cmdList, u32 max_count, buffer_t* args_buffer, u32 args_buffer_offset, buffer_t* count_buffer, u32 count_buffer_offset);
            void MultiDrawIndexedIndirect(ngfx::command_list_t* cmdList, u32 max_count, buffer_t* args_buffer, u32 args_buffer_offset, buffer_t* count_buffer, u32 count_buffer_offset);
            void MultiDispatchIndirect(ngfx::command_list_t* cmdList, u32 max_count, buffer_t* args_buffer, u32 args_buffer_offset, buffer_t* count_buffer, u32 count_buffer_offset);
            void MultiDispatchMeshIndirect(ngfx::command_list_t* cmdList, u32 max_count, buffer_t* args_buffer, u32 args_buffer_offset, buffer_t* count_buffer, u32 count_buffer_offset);

            void BuildRayTracingBLAS(ngfx::command_list_t* cmdList, ngfx::blas_t* blas);
            void UpdateRayTracingBLAS(ngfx::command_list_t* cmdList, ngfx::blas_t* blas, ngfx::buffer_t* vertex_buffer, u32 vertex_buffer_offset);
            void BuildRayTracingTLAS(ngfx::command_list_t* cmdList, ngfx::tlas_t* tlas, const rt_instance_t* instances, u32 instance_count);

        }  // namespace nmetal
    }  // namespace ngfx
}  // namespace ncore
