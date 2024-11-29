#include "cbase/c_integer.h"
#include "cgfx/gfx_defines.h"
#include "cgfx/metal/metal_buffer.h"
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
        namespace nmetal
        {
            static bool s_nearly_equal(float a, float b, float epsilon = 0.0001f) { return fabs(a - b) < epsilon; }

            void SetRasterizerState(nmetal::command_list_t* cmdList, const rasterizer_state_t& state)
            {
                MTL::CullMode cullMode = ToCullMode(state.cull_mode);
                if (cmdList->m_cullMode != cullMode)
                {
                    cmdList->m_pRenderCommandEncoder->setCullMode(cullMode);
                    cmdList->m_cullMode = cullMode;
                }

                MTL::Winding frontFaceWinding = state.front_ccw ? MTL::WindingCounterClockwise : MTL::WindingClockwise;
                if (cmdList->m_frontFaceWinding != frontFaceWinding)
                {
                    cmdList->m_pRenderCommandEncoder->setFrontFacingWinding(frontFaceWinding);
                    cmdList->m_frontFaceWinding = frontFaceWinding;
                }

                MTL::TriangleFillMode fillMode = state.wireframe ? MTL::TriangleFillModeLines : MTL::TriangleFillModeFill;
                if (cmdList->m_fillMode != fillMode)
                {
                    cmdList->m_pRenderCommandEncoder->setTriangleFillMode(fillMode);
                    cmdList->m_fillMode = fillMode;
                }

                MTL::DepthClipMode clipMode = state.depth_clip ? MTL::DepthClipModeClip : MTL::DepthClipModeClamp;
                if (cmdList->m_clipMode != clipMode)
                {
                    cmdList->m_pRenderCommandEncoder->setDepthClipMode(clipMode);
                    cmdList->m_clipMode = clipMode;
                }

                if (!s_nearly_equal(state.depth_bias, cmdList->m_depthBias) || !s_nearly_equal(state.depth_bias_clamp, cmdList->m_depthBiasClamp) || !s_nearly_equal(state.depth_slope_scale, cmdList->m_depthSlopeScale))
                {
                    cmdList->m_pRenderCommandEncoder->setDepthBias(state.depth_bias, state.depth_slope_scale, state.depth_bias_clamp);

                    cmdList->m_depthBias       = state.depth_bias;
                    cmdList->m_depthBiasClamp  = state.depth_bias_clamp;
                    cmdList->m_depthSlopeScale = state.depth_slope_scale;
                }
            }

            void CreateCommandList(ngfx::command_list_t* cmdList)
            {
                // ...
                nmetal::command_list_t* mcmdList = CreateComponent<ngfx::command_list_t, nmetal::command_list_t>(cmdList->m_device, cmdList);
            }

            bool Create(ngfx::command_list_t* cmdList)
            {
                nmetal::command_list_t* mcmdList  = GetComponent<ngfx::command_list_t, nmetal::command_list_t>(cmdList->m_device, cmdList);
                nmetal::device_t*       mdevice   = GetComponent<ngfx::device_t, nmetal::device_t>(cmdList->m_device, cmdList->m_device);
                MTL::Device*            mtlDevice = mdevice->m_pDevice;
                mcmdList->m_pFence                = mtlDevice->newFence();
                return true;
            }

            void Destroy(ngfx::command_list_t* cmdList)
            {
                nmetal::command_list_t* mcmdList = GetComponent<ngfx::command_list_t, nmetal::command_list_t>(cmdList->m_device, cmdList);
                if (mcmdList->m_pFence)
                {
                    mcmdList->m_pFence->release();
                    mcmdList->m_pFence = nullptr;
                }
            }

            MTL::CommandBuffer* GetHandle(ngfx::command_list_t* cmdList)
            {
                nmetal::command_list_t* mcmdList = GetComponent<ngfx::command_list_t, nmetal::command_list_t>(cmdList->m_device, cmdList);
                return mcmdList->m_pCommandBuffer;
            }

            void ResetAllocator(ngfx::command_list_t* cmdList) {}

            void Begin(ngfx::command_list_t* cmdList)
            {
                nmetal::device_t*       mdevice  = GetComponent<ngfx::device_t, nmetal::device_t>(cmdList->m_device, cmdList->m_device);
                MTL::CommandQueue*      queue    = mdevice->m_pQueue;
                nmetal::command_list_t* mcmdList = GetComponent<ngfx::command_list_t, nmetal::command_list_t>(cmdList->m_device, cmdList);
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

            void EndASEncoder(nmetal::command_list_t* cmdList)
            {
                if (cmdList->m_pASEncoder)
                {
                    cmdList->m_pASEncoder->updateFence(cmdList->m_pFence);
                    cmdList->m_pASEncoder->endEncoding();
                    cmdList->m_pASEncoder = nullptr;
                }
            }

            void End(ngfx::command_list_t* cmdList)
            {
                nmetal::command_list_t* mcmdList = GetComponent<ngfx::command_list_t, nmetal::command_list_t>(cmdList->m_device, cmdList);
                EndBlitEncoder(mcmdList);
                EndComputeEncoder(mcmdList);
                EndRenderPass(mcmdList);
            }

            void BeginASEncoder(ngfx::command_list_t* cmdList)
            {
                nmetal::command_list_t* mcmdList = GetComponent<ngfx::command_list_t, nmetal::command_list_t>(cmdList->m_device, cmdList);
                EndRenderPass(mcmdList);
                EndBlitEncoder(mcmdList);
                EndComputeEncoder(mcmdList);

                if (mcmdList->m_pASEncoder == nullptr)
                {
                    mcmdList->m_pASEncoder = mcmdList->m_pCommandBuffer->accelerationStructureCommandEncoder();
                    mcmdList->m_pASEncoder->waitForFence(mcmdList->m_pFence);
                }
            }

            void BeginComputeEncoder(ngfx::command_list_t* cmdList)
            {
                nmetal::command_list_t* mcmdList = GetComponent<ngfx::command_list_t, nmetal::command_list_t>(cmdList->m_device, cmdList);
                EndRenderPass(mcmdList);
                EndBlitEncoder(mcmdList);
                EndASEncoder(mcmdList);

                if (mcmdList->m_pComputeCommandEncoder == nullptr)
                {
                    mcmdList->m_pComputeCommandEncoder = mcmdList->m_pCommandBuffer->computeCommandEncoder();
                    mcmdList->m_pComputeCommandEncoder->waitForFence(mcmdList->m_pFence);

                    nmetal::device_t* mdevice           = GetComponent<ngfx::device_t, nmetal::device_t>(cmdList->m_device, cmdList->m_device);
                    MTL::Buffer*      mtlResDescrBuffer = nmetal::GetResourceDescriptorBuffer(mdevice);
                    MTL::Buffer*      mtlSamDescrBuffer = nmetal::GetSamplerDescriptorBuffer(mdevice);

                    mcmdList->m_pComputeCommandEncoder->setBuffer(mtlResDescrBuffer, 0, kIRDescriptorHeapBindPoint);
                    mcmdList->m_pComputeCommandEncoder->setBuffer(mtlSamDescrBuffer, 0, kIRSamplerHeapBindPoint);
                }
            }

            void BeginBlitEncoder(ngfx::command_list_t* cmdList)
            {
                nmetal::command_list_t* mcmdList = GetComponent<ngfx::command_list_t, nmetal::command_list_t>(cmdList->m_device, cmdList);
                EndRenderPass(mcmdList);
                EndComputeEncoder(mcmdList);
                EndASEncoder(mcmdList);

                if (mcmdList->m_pBlitCommandEncoder == nullptr)
                {
                    mcmdList->m_pBlitCommandEncoder = mcmdList->m_pCommandBuffer->blitCommandEncoder();
                    mcmdList->m_pBlitCommandEncoder->waitForFence(mcmdList->m_pFence);
                }
            }

            void Wait(ngfx::command_list_t* cmdList, ngfx::fence_t* fence, u64 value)
            {
                nmetal::command_list_t* mcmdList = GetComponent<ngfx::command_list_t, nmetal::command_list_t>(cmdList->m_device, cmdList);
                nmetal::fence_t*        mfence   = GetComponent<ngfx::fence_t, nmetal::fence_t>(cmdList->m_device, fence);
                mcmdList->m_pCommandBuffer->encodeWait(mfence->m_pEvent, value);
            }

            void Signal(ngfx::command_list_t* cmdList, ngfx::fence_t* fence, u64 value)
            {
                nmetal::command_list_t* mcmdList = GetComponent<ngfx::command_list_t, nmetal::command_list_t>(cmdList->m_device, cmdList);
                nmetal::fence_t*        mfence   = GetComponent<ngfx::fence_t, nmetal::fence_t>(cmdList->m_device, fence);
                mcmdList->m_pCommandBuffer->encodeSignalEvent(mfence->m_pEvent, value);
            }

            void Present(ngfx::command_list_t* cmdList, ngfx::swapchain_t* swapchain)
            {
                nmetal::command_list_t* mcmdList   = GetComponent<ngfx::command_list_t, nmetal::command_list_t>(cmdList->m_device, cmdList);
                nmetal::swapchain_t*    mswapchain = GetComponent<ngfx::swapchain_t, nmetal::swapchain_t>(cmdList->m_device, swapchain);
                // TODO, no reason to use GetDrawable, we could directly use the view
                mcmdList->m_pCommandBuffer->presentDrawable(nmetal::GetDrawable(cmdList->m_device, swapchain));
            }

            void Submit(ngfx::command_list_t* cmdList)
            {
                nmetal::command_list_t* mcmdList = GetComponent<ngfx::command_list_t, nmetal::command_list_t>(cmdList->m_device, cmdList);
                mcmdList->m_pCommandBuffer->commit();
                mcmdList->m_pCommandBuffer = nullptr;
            }

            void ResetState(ngfx::command_list_t* cmdList)
            {
                nmetal::command_list_t* mcmdList = GetComponent<ngfx::command_list_t, nmetal::command_list_t>(cmdList->m_device, cmdList);
                ResetState(mcmdList);
            }

            void BeginProfiling(ngfx::command_list_t* cmdList) {}
            void EndProfiling(ngfx::command_list_t* cmdList) {}

            void BeginEvent(ngfx::command_list_t* cmdList, const char* event_name)
            {
                nmetal::command_list_t* mcmdList = GetComponent<ngfx::command_list_t, nmetal::command_list_t>(cmdList->m_device, cmdList);
                NS::String*             label    = NS::String::alloc()->init(event_name, NS::StringEncoding::UTF8StringEncoding);
                mcmdList->m_pCommandBuffer->pushDebugGroup(label);
                label->release();
            }

            void EndEvent(ngfx::command_list_t* cmdList)
            {
                nmetal::command_list_t* mcmdList = GetComponent<ngfx::command_list_t, nmetal::command_list_t>(cmdList->m_device, cmdList);
                mcmdList->m_pCommandBuffer->popDebugGroup();
            }

            inline static u32 sMaxSize(u32 size, u32 minSize) { return (size < minSize) ? minSize : size; }
            inline static u32 sMaxMipSize(u32 size, u32 mip_level, u32 minSize)
            {
                size = size >> mip_level;
                return (size < minSize) ? minSize : size;
            }

            void CopyBufferToTexture(ngfx::command_list_t* cmdList, texture_t* dst_texture, u32 mip_level, u32 array_slice, buffer_t* src_buffer, u32 offset)
            {
                BeginBlitEncoder(cmdList);

                const texture_desc_t& desc        = dst_texture->m_desc;
                MTL::Size             textureSize = MTL::Size::Make(sMaxMipSize(desc.width, mip_level, 1), sMaxMipSize(desc.height, mip_level, 1), sMaxMipSize(desc.depth, mip_level, 1));

                u32 const bytesPerRow   = nmetal::GetRowPitch(cmdList->m_device, dst_texture, mip_level);
                u32 const blockHeight   = enums::GetFormatBlockHeight(desc.format);
                u32 const height        = sMaxMipSize(desc.height, mip_level, blockHeight);
                u32 const rowNum        = height / blockHeight;
                u32 const bytesPerImage = bytesPerRow * rowNum;

                MTL::Texture* dstMtlTexture = nmetal::GetHandle(cmdList->m_device, dst_texture);
                MTL::Buffer*  srcMtlBuffer  = nmetal::GetHandle(cmdList->m_device, src_buffer);

                nmetal::command_list_t* mcmdList = GetComponent<ngfx::command_list_t, nmetal::command_list_t>(cmdList->m_device, cmdList);
                mcmdList->m_pBlitCommandEncoder->copyFromBuffer(srcMtlBuffer, offset, bytesPerRow, desc.type == enums::TextureType3D ? bytesPerImage : 0, textureSize, dstMtlTexture, array_slice, mip_level, MTL::Origin::Make(0, 0, 0));
            }

            void CopyTextureToBuffer(ngfx::command_list_t* cmdList, buffer_t* dst_buffer, u32 offset, texture_t* src_texture, u32 mip_level, u32 array_slice)
            {
                BeginBlitEncoder(cmdList);

                const texture_desc_t& desc        = src_texture->m_desc;
                MTL::Size             textureSize = MTL::Size::Make(sMaxMipSize(desc.width, mip_level, 1), sMaxMipSize(desc.height, mip_level, 1), sMaxMipSize(desc.depth, mip_level, 1));

                u32 const bytesPerRow   = nmetal::GetRowPitch(cmdList->m_device, src_texture, mip_level);
                u32 const blockHeight   = enums::GetFormatBlockHeight(desc.format);
                u32 const height        = sMaxMipSize(desc.height, mip_level, blockHeight);
                u32 const rowNum        = height / blockHeight;
                u32 const bytesPerImage = bytesPerRow * rowNum;

                MTL::Buffer*  dstMtlBuffer  = nmetal::GetHandle(cmdList->m_device, dst_buffer);
                MTL::Texture* srcMtlTexture = nmetal::GetHandle(cmdList->m_device, src_texture);

                nmetal::command_list_t* mcmdList = GetComponent<ngfx::command_list_t, nmetal::command_list_t>(cmdList->m_device, cmdList);
                mcmdList->m_pBlitCommandEncoder->copyFromTexture(srcMtlTexture, array_slice, mip_level, MTL::Origin::Make(0, 0, 0), textureSize, dstMtlBuffer, offset, bytesPerRow, desc.type == enums::TextureType3D ? bytesPerImage : 0);
            }

            void CopyBuffer(ngfx::command_list_t* cmdList, buffer_t* dst, u32 dst_offset, buffer_t* src, u32 src_offset, u32 size)
            {
                BeginBlitEncoder(cmdList);
                nmetal::command_list_t* mcmdList = GetComponent<ngfx::command_list_t, nmetal::command_list_t>(cmdList->m_device, cmdList);

                MTL::Buffer* srcMetalBuffer = nmetal::GetHandle(cmdList->m_device, src);
                MTL::Buffer* dstMetalBuffer = nmetal::GetHandle(cmdList->m_device, dst);
                mcmdList->m_pBlitCommandEncoder->copyFromBuffer(srcMetalBuffer, src_offset, dstMetalBuffer, dst_offset, size);
            }

            void CopyTexture(ngfx::command_list_t* cmdList, texture_t* dst, u32 dst_mip, u32 dst_array, texture_t* src, u32 src_mip, u32 src_array)
            {
                BeginBlitEncoder(cmdList);
                nmetal::command_list_t* mcmdList = GetComponent<ngfx::command_list_t, nmetal::command_list_t>(cmdList->m_device, cmdList);

                const texture_desc_t& srcDesc = src->m_desc;
                MTL::Size             srcSize = MTL::Size::Make(sMaxMipSize(srcDesc.width, src_mip, 1), sMaxMipSize(srcDesc.height, src_mip, 1), sMaxMipSize(srcDesc.depth, src_mip, 1));
                MTL::Origin           origin  = MTL::Origin::Make(0, 0, 0);

                MTL::Texture* srcMetalTexture = nmetal::GetHandle(cmdList->m_device, src);
                MTL::Texture* dstMetalTexture = nmetal::GetHandle(cmdList->m_device, dst);
                mcmdList->m_pBlitCommandEncoder->copyFromTexture(srcMetalTexture, src_array, src_mip, origin, srcSize, dstMetalTexture, dst_array, dst_mip, origin);
            }

            void ClearUAV(ngfx::command_list_t* cmdList, resource_t* resource, ngfx::descriptor_t* uav, const float* clear_value)
            {
                if (uav->m_type == enums::DescriptorTypeUavBuffer)
                {
                    nmetal::uav_buffer_t* muav = GetComponent<ngfx::descriptor_t, nmetal::uav_buffer_t>(cmdList->m_device, uav);
                    const uav_desc_t&     desc = muav->m_base.m_desc;
                    // TODO, use callback_t ??
                    //::ClearUAV(cmdList, resource, uav, desc, clear_value);
                }
                else if (uav->m_type == enums::DescriptorTypeUavTexture)
                {
                    nmetal::uav_texture_t* muav = GetComponent<ngfx::descriptor_t, nmetal::uav_texture_t>(cmdList->m_device, uav);
                    const uav_desc_t&      desc = muav->m_base.m_desc;
                    // TODO, use callback_t ??
                    //::ClearUAV(cmdList, resource, uav, desc, clear_value);
                }
            }

            void ClearUAV(ngfx::command_list_t* cmdList, resource_t* resource, ngfx::descriptor_t* uav, const u32* clear_value)
            {
                if (uav->m_type == enums::DescriptorTypeUavBuffer)
                {
                    nmetal::uav_buffer_t* muav = GetComponent<ngfx::descriptor_t, nmetal::uav_buffer_t>(cmdList->m_device, uav);
                    const uav_desc_t&     desc = muav->m_base.m_desc;
                    // TODO, use callback_t ??
                    //::ClearUAV(cmdList, resource, uav, desc, clear_value);
                }
                else if (uav->m_type == enums::DescriptorTypeUavTexture)
                {
                    nmetal::uav_texture_t* muav = GetComponent<ngfx::descriptor_t, nmetal::uav_texture_t>(cmdList->m_device, uav);
                    const uav_desc_t&      desc = muav->m_base.m_desc;
                    // TODO, use callback_t ??
                    //::ClearUAV(cmdList, resource, uav, desc, clear_value);
                }
            }

            void WriteBuffer(ngfx::command_list_t* cmdList, buffer_t* buffer, u32 offset, u32 data)
            {
                BeginBlitEncoder(cmdList);

                nmetal::BeginEvent(cmdList, "CommandList::WriteBuffer");

                MTL::Buffer* mtlBuffer = nmetal::GetHandle(cmdList->m_device, buffer);

                nmetal::command_list_t* mcmdList = GetComponent<ngfx::command_list_t, nmetal::command_list_t>(cmdList->m_device, cmdList);
                mcmdList->m_pBlitCommandEncoder->fillBuffer(mtlBuffer, NS::Range::Make(offset + 0, sizeof(u8)), u8(data >> 0));
                mcmdList->m_pBlitCommandEncoder->fillBuffer(mtlBuffer, NS::Range::Make(offset + 1, sizeof(u8)), u8(data >> 8));
                mcmdList->m_pBlitCommandEncoder->fillBuffer(mtlBuffer, NS::Range::Make(offset + 2, sizeof(u8)), u8(data >> 16));
                mcmdList->m_pBlitCommandEncoder->fillBuffer(mtlBuffer, NS::Range::Make(offset + 3, sizeof(u8)), u8(data >> 24));

                nmetal::EndEvent(cmdList);
            }

            void UpdateTileMappings(ngfx::command_list_t* cmdList, texture_t* texture, heap_t* heap, u32 mapping_count, const tile_mapping_t* mappings)
            {
                // TODO
            }

            void TextureBarrier(ngfx::command_list_t* cmdList, texture_t* texture, u32 sub_resource, enums::access_flags access_before, enums::access_flags access_after)
            {
                // ...
            }

            void BufferBarrier(ngfx::command_list_t* cmdList, buffer_t* buffer, enums::access_flags access_before, enums::access_flags access_after)
            {
                // ...
            }

            void GlobalBarrier(ngfx::command_list_t* cmdList, enums::access_flags access_before, enums::access_flags access_after)
            {
                // ...
            }

            void FlushBarriers(ngfx::command_list_t* cmdList)
            {
                // ...
            }

            void BeginRenderPass(ngfx::command_list_t* cmdList, const renderpass_desc_t& render_pass)
            {
                nmetal::command_list_t* mcmdList = GetComponent<ngfx::command_list_t, nmetal::command_list_t>(cmdList->m_device, cmdList);
                EndBlitEncoder(mcmdList);
                EndComputeEncoder(mcmdList);
                EndASEncoder(mcmdList);

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
                        width = render_pass.color[i].texture->m_desc.width;
                    }

                    if (height == 0)
                    {
                        height = render_pass.color[i].texture->m_desc.height;
                    }

                    ASSERT(width == render_pass.color[i].texture->m_desc.width);
                    ASSERT(height == render_pass.color[i].texture->m_desc.height);

                    MTL::Texture* renderPassMtlTexture = nmetal::GetHandle(cmdList->m_device, render_pass.color[i].texture);
                    colorAttachements->object(i)->setTexture(renderPassMtlTexture);
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
                        width = render_pass.depth.texture->m_desc.width;
                    }

                    if (height == 0)
                    {
                        height = render_pass.depth.texture->m_desc.height;
                    }

                    ASSERT(width == render_pass.depth.texture->m_desc.width);
                    ASSERT(height == render_pass.depth.texture->m_desc.height);

                    MTL::Texture* renderPassMtlDepthTexture = nmetal::GetHandle(cmdList->m_device, render_pass.depth.texture);

                    MTL::RenderPassDepthAttachmentDescriptor* depthAttachment = descriptor->depthAttachment();
                    depthAttachment->setTexture(renderPassMtlDepthTexture);
                    depthAttachment->setLevel(render_pass.depth.mip_slice);
                    depthAttachment->setSlice(render_pass.depth.array_slice);
                    depthAttachment->setLoadAction(ToLoadAction(render_pass.depth.load_op));
                    depthAttachment->setStoreAction(ToStoreAction(render_pass.depth.store_op));
                    depthAttachment->setClearDepth(render_pass.depth.clear_depth);

                    if (enums::IsStencilFormat(render_pass.depth.texture->m_desc.format))
                    {
                        MTL::RenderPassStencilAttachmentDescriptor* stencilAttachment = descriptor->stencilAttachment();
                        stencilAttachment->setTexture(renderPassMtlDepthTexture);
                        stencilAttachment->setLevel(render_pass.depth.mip_slice);
                        stencilAttachment->setSlice(render_pass.depth.array_slice);
                        stencilAttachment->setLoadAction(ToLoadAction(render_pass.depth.stencil_load_op));
                        stencilAttachment->setStoreAction(ToStoreAction(render_pass.depth.stencil_store_op));
                        stencilAttachment->setClearStencil(render_pass.depth.clear_stencil);
                    }
                }

                ASSERT(mcmdList->m_pRenderCommandEncoder == nullptr);
                mcmdList->m_pRenderCommandEncoder = mcmdList->m_pCommandBuffer->renderCommandEncoder(descriptor);
                mcmdList->m_pRenderCommandEncoder->waitForFence(mcmdList->m_pFence, MTL::RenderStageVertex | MTL::RenderStageObject);
                descriptor->release();

                nmetal::device_t* mdevice = GetComponent<ngfx::device_t, nmetal::device_t>(cmdList->m_device, cmdList->m_device);
                mcmdList->m_pRenderCommandEncoder->setVertexBuffer(nmetal::GetResourceDescriptorBuffer(mdevice), 0, kIRDescriptorHeapBindPoint);
                mcmdList->m_pRenderCommandEncoder->setVertexBuffer(nmetal::GetSamplerDescriptorBuffer(mdevice), 0, kIRSamplerHeapBindPoint);
                mcmdList->m_pRenderCommandEncoder->setFragmentBuffer(nmetal::GetResourceDescriptorBuffer(mdevice), 0, kIRDescriptorHeapBindPoint);
                mcmdList->m_pRenderCommandEncoder->setFragmentBuffer(nmetal::GetSamplerDescriptorBuffer(mdevice), 0, kIRSamplerHeapBindPoint);

                mcmdList->m_pRenderCommandEncoder->setObjectBuffer(nmetal::GetResourceDescriptorBuffer(mdevice), 0, kIRDescriptorHeapBindPoint);
                mcmdList->m_pRenderCommandEncoder->setObjectBuffer(nmetal::GetSamplerDescriptorBuffer(mdevice), 0, kIRSamplerHeapBindPoint);
                mcmdList->m_pRenderCommandEncoder->setMeshBuffer(nmetal::GetResourceDescriptorBuffer(mdevice), 0, kIRDescriptorHeapBindPoint);
                mcmdList->m_pRenderCommandEncoder->setMeshBuffer(nmetal::GetSamplerDescriptorBuffer(mdevice), 0, kIRSamplerHeapBindPoint);

                nmetal::SetViewport(cmdList, 0, 0, width, height);
            }

            void EndRenderPass(ngfx::command_list_t* cmdList)
            {
                nmetal::command_list_t* mcmdList = GetComponent<ngfx::command_list_t, nmetal::command_list_t>(cmdList->m_device, cmdList);
                if (mcmdList->m_pRenderCommandEncoder)
                {
                    mcmdList->m_pRenderCommandEncoder->updateFence(mcmdList->m_pFence, MTL::RenderStageFragment);
                    mcmdList->m_pRenderCommandEncoder->endEncoding();
                    mcmdList->m_pRenderCommandEncoder = nullptr;

                    ResetState(mcmdList);
                }
            }

            void SetPipelineState(ngfx::command_list_t* cmdList, pipeline_state_t* state)
            {
                nmetal::command_list_t* mcmdList = GetComponent<ngfx::command_list_t, nmetal::command_list_t>(cmdList->m_device, cmdList);
                if (mcmdList->m_pCurrentPSO != state)
                {
                    mcmdList->m_pCurrentPSO = state;

                    if (state->m_type != enums::PipelineCompute)
                    {
                        ASSERT(mcmdList->m_pRenderCommandEncoder != nullptr);

                        if (state->m_type == enums::PipelineGraphics)
                        {
                            nmetal::graphics_pipeline_state_t* graphicsPipeline = GetComponent<ngfx::pipeline_state_t, nmetal::graphics_pipeline_state_t>(cmdList->m_device, state);
                            mcmdList->m_pRenderCommandEncoder->setRenderPipelineState(graphicsPipeline->m_pPSO);
                            const graphics_pipeline_desc_t& desc = graphicsPipeline->m_desc;

                            mcmdList->m_pRenderCommandEncoder->setDepthStencilState(graphicsPipeline->m_pDepthStencilState);
                            SetRasterizerState(mcmdList, desc.rasterizer_state);
                            mcmdList->m_primitiveType = ToPrimitiveType(desc.primitive_type);
                        }
                        else
                        {
                            nmetal::meshshading_pipeline_state_t* meshShadingPipeline = GetComponent<ngfx::pipeline_state_t, nmetal::meshshading_pipeline_state_t>(cmdList->m_device, state);
                            mcmdList->m_pRenderCommandEncoder->setRenderPipelineState(meshShadingPipeline->m_pPSO);
                            const mesh_shading_pipeline_desc_t& desc = meshShadingPipeline->m_desc;

                            mcmdList->m_pRenderCommandEncoder->setDepthStencilState(meshShadingPipeline->m_pDepthStencilState);
                            SetRasterizerState(mcmdList, desc.rasterizer_state);
                        }
                    }
                }
            }

            void SetStencilReference(ngfx::command_list_t* cmdList, u8 stencil)
            {
                nmetal::command_list_t* mcmdList = GetComponent<ngfx::command_list_t, nmetal::command_list_t>(cmdList->m_device, cmdList);
                ASSERT(mcmdList->m_pRenderCommandEncoder != nullptr);
                mcmdList->m_pRenderCommandEncoder->setStencilReferenceValue(stencil);
            }

            void SetBlendFactor(ngfx::command_list_t* cmdList, const float* blend_factor)
            {
                nmetal::command_list_t* mcmdList = GetComponent<ngfx::command_list_t, nmetal::command_list_t>(cmdList->m_device, cmdList);
                ASSERT(mcmdList->m_pRenderCommandEncoder != nullptr);
                mcmdList->m_pRenderCommandEncoder->setBlendColor(blend_factor[0], blend_factor[1], blend_factor[2], blend_factor[3]);
            }

            void SetIndexBuffer(ngfx::command_list_t* cmdList, buffer_t* buffer, u32 offset, enums::format format)
            {
                nmetal::command_list_t* mcmdList = GetComponent<ngfx::command_list_t, nmetal::command_list_t>(cmdList->m_device, cmdList);
                ASSERT(mcmdList->m_pRenderCommandEncoder != nullptr);
                mcmdList->m_pIndexBuffer      = nmetal::GetHandle(cmdList->m_device, buffer);
                mcmdList->m_indexBufferOffset = offset;
                mcmdList->m_indexType         = (format == enums::FORMAT_R16UI) ? MTL::IndexTypeUInt16 : MTL::IndexTypeUInt32;
            }

            void SetViewport(ngfx::command_list_t* cmdList, u32 x, u32 y, u32 width, u32 height)
            {
                nmetal::command_list_t* mcmdList = GetComponent<ngfx::command_list_t, nmetal::command_list_t>(cmdList->m_device, cmdList);
                ASSERT(mcmdList->m_pRenderCommandEncoder != nullptr);
                MTL::Viewport viewport = {(f64)x, (f64)y, (f64)width, (f64)height, 0.0f, 1.0f};
                mcmdList->m_pRenderCommandEncoder->setViewport(viewport);

                MTL::ScissorRect scissorRect = {x, y, width, height};
                mcmdList->m_pRenderCommandEncoder->setScissorRect(scissorRect);
            }

            void SetScissorRect(ngfx::command_list_t* cmdList, u32 x, u32 y, u32 width, u32 height)
            {
                nmetal::command_list_t* mcmdList = GetComponent<ngfx::command_list_t, nmetal::command_list_t>(cmdList->m_device, cmdList);
                ASSERT(mcmdList->m_pRenderCommandEncoder != nullptr);
                MTL::ScissorRect scissorRect = {x, y, width, height};
                mcmdList->m_pRenderCommandEncoder->setScissorRect(scissorRect);
            }

            void SetGraphicsConstants(ngfx::command_list_t* cmdList, u32 slot, const void* data, s64 data_size)
            {
                nmetal::command_list_t* mcmdList = GetComponent<ngfx::command_list_t, nmetal::command_list_t>(cmdList->m_device, cmdList);
                if (slot == 0)
                {
                    ASSERT(data_size <= GFX_MAX_ROOT_CONSTANTS * sizeof(u32));
                    memcpy(mcmdList->m_graphicsArgumentBuffer.cbv0, data, data_size);
                }
                else
                {
                    ASSERT(slot < GFX_MAX_CBV_BINDINGS);
                    u64 gpuAddress = nmetal::AllocateConstantBuffer(cmdList->m_device, data, data_size);
                    if (slot == 1)
                    {
                        mcmdList->m_graphicsArgumentBuffer.cbv1 = gpuAddress;
                    }
                    else
                    {
                        mcmdList->m_graphicsArgumentBuffer.cbv2 = gpuAddress;
                    }
                }
            }

            void SetComputeConstants(ngfx::command_list_t* cmdList, u32 slot, const void* data, s64 data_size)
            {
                nmetal::command_list_t* mcmdList = GetComponent<ngfx::command_list_t, nmetal::command_list_t>(cmdList->m_device, cmdList);
                if (slot == 0)
                {
                    ASSERT(data_size <= GFX_MAX_ROOT_CONSTANTS * sizeof(u32));
                    memcpy(mcmdList->m_computeArgumentBuffer.cbv0, data, data_size);
                }
                else
                {
                    ASSERT(slot < GFX_MAX_CBV_BINDINGS);
                    uint64_t gpuAddress = nmetal::AllocateConstantBuffer(cmdList->m_device, data, data_size);

                    if (slot == 1)
                    {
                        mcmdList->m_computeArgumentBuffer.cbv1 = gpuAddress;
                    }
                    else
                    {
                        mcmdList->m_computeArgumentBuffer.cbv2 = gpuAddress;
                    }
                }
            }

            void Draw(ngfx::command_list_t* cmdList, u32 vertex_count, u32 instance_count)
            {
                nmetal::command_list_t* mcmdList = GetComponent<ngfx::command_list_t, nmetal::command_list_t>(cmdList->m_device, cmdList);
                ASSERT(mcmdList->m_pRenderCommandEncoder != nullptr);

                mcmdList->m_pRenderCommandEncoder->setVertexBytes(&mcmdList->m_graphicsArgumentBuffer, sizeof(top_level_argument_buffer_t), kIRArgumentBufferBindPoint);
                mcmdList->m_pRenderCommandEncoder->setFragmentBytes(&mcmdList->m_graphicsArgumentBuffer, sizeof(top_level_argument_buffer_t), kIRArgumentBufferBindPoint);

                IRRuntimeDrawPrimitives(mcmdList->m_pRenderCommandEncoder, mcmdList->m_primitiveType, 0, vertex_count, instance_count);
            }

            void DrawIndexed(ngfx::command_list_t* cmdList, u32 index_count, u32 instance_count, u32 index_offset)
            {
                nmetal::command_list_t* mcmdList = GetComponent<ngfx::command_list_t, nmetal::command_list_t>(cmdList->m_device, cmdList);
                ASSERT(mcmdList->m_pRenderCommandEncoder != nullptr);

                mcmdList->m_pRenderCommandEncoder->setVertexBytes(&mcmdList->m_graphicsArgumentBuffer, sizeof(top_level_argument_buffer_t), kIRArgumentBufferBindPoint);
                mcmdList->m_pRenderCommandEncoder->setFragmentBytes(&mcmdList->m_graphicsArgumentBuffer, sizeof(top_level_argument_buffer_t), kIRArgumentBufferBindPoint);

                u64 indexBufferOffset = mcmdList->m_indexBufferOffset;
                if (mcmdList->m_indexType == MTL::IndexTypeUInt16)
                {
                    indexBufferOffset += sizeof(u16) * index_offset;
                }
                else
                {
                    indexBufferOffset += sizeof(u32) * index_offset;
                }

                IRRuntimeDrawIndexedPrimitives(mcmdList->m_pRenderCommandEncoder, mcmdList->m_primitiveType, index_count, mcmdList->m_indexType, mcmdList->m_pIndexBuffer, indexBufferOffset, instance_count, 0, 0);
            }

            void Dispatch(ngfx::command_list_t* cmdList, u32 group_count_x, u32 group_count_y, u32 group_count_z)
            {
                nmetal::command_list_t* mcmdList = GetComponent<ngfx::command_list_t, nmetal::command_list_t>(cmdList->m_device, cmdList);
                if (mcmdList->m_pCurrentPSO->m_type == enums::PipelineCompute)
                {
                    BeginComputeEncoder(cmdList);

                    nmetal::compute_pipeline_state_t* pipelineState = GetComponent<ngfx::pipeline_state_t, nmetal::compute_pipeline_state_t>(cmdList->m_device, mcmdList->m_pCurrentPSO);
                    mcmdList->m_pComputeCommandEncoder->setComputePipelineState(pipelineState->m_pPSO);
                    mcmdList->m_pComputeCommandEncoder->setBytes(&mcmdList->m_computeArgumentBuffer, sizeof(top_level_argument_buffer_t), kIRArgumentBufferBindPoint);

                    MTL::Size const threadgroupsPerGrid   = MTL::Size::Make(group_count_x, group_count_y, group_count_z);
                    MTL::Size const threadsPerThreadgroup = pipelineState->m_threadsPerThreadgroup;
                    mcmdList->m_pComputeCommandEncoder->dispatchThreadgroups(threadgroupsPerGrid, threadsPerThreadgroup);

                    EndComputeEncoder(mcmdList);
                }
            }

            void DispatchMesh(ngfx::command_list_t* cmdList, u32 group_count_x, u32 group_count_y, u32 group_count_z)
            {
                nmetal::command_list_t* mcmdList = GetComponent<ngfx::command_list_t, nmetal::command_list_t>(cmdList->m_device, cmdList);
                ASSERT(mcmdList->m_pRenderCommandEncoder != nullptr);

                if (mcmdList->m_pCurrentPSO->m_type == enums::PipelineMeshShading)
                {
                    mcmdList->m_pRenderCommandEncoder->setObjectBytes(&mcmdList->m_graphicsArgumentBuffer, sizeof(top_level_argument_buffer_t), kIRArgumentBufferBindPoint);
                    mcmdList->m_pRenderCommandEncoder->setMeshBytes(&mcmdList->m_graphicsArgumentBuffer, sizeof(top_level_argument_buffer_t), kIRArgumentBufferBindPoint);
                    mcmdList->m_pRenderCommandEncoder->setFragmentBytes(&mcmdList->m_graphicsArgumentBuffer, sizeof(top_level_argument_buffer_t), kIRArgumentBufferBindPoint);

                    meshshading_pipeline_state_t* pipelineState = GetComponent<ngfx::pipeline_state_t, nmetal::meshshading_pipeline_state_t>(cmdList->m_device, mcmdList->m_pCurrentPSO);

                    MTL::Size const threadgroupsPerGrid         = MTL::Size::Make(group_count_x, group_count_y, group_count_z);
                    MTL::Size const threadsPerObjectThreadgroup = pipelineState->m_threadsPerObjectThreadgroup;
                    MTL::Size const threadsPerMeshThreadgroup   = pipelineState->m_threadsPerMeshThreadgroup;
                    mcmdList->m_pRenderCommandEncoder->drawMeshThreadgroups(threadgroupsPerGrid, threadsPerObjectThreadgroup, threadsPerMeshThreadgroup);
                }
            }

            void DrawIndirect(ngfx::command_list_t* cmdList, buffer_t* buffer, u32 offset)
            {
                nmetal::command_list_t* mcmdList = GetComponent<ngfx::command_list_t, nmetal::command_list_t>(cmdList->m_device, cmdList);
                ASSERT(mcmdList->m_pRenderCommandEncoder != nullptr);

                mcmdList->m_pRenderCommandEncoder->setVertexBytes(&mcmdList->m_graphicsArgumentBuffer, sizeof(top_level_argument_buffer_t), kIRArgumentBufferBindPoint);
                mcmdList->m_pRenderCommandEncoder->setFragmentBytes(&mcmdList->m_graphicsArgumentBuffer, sizeof(top_level_argument_buffer_t), kIRArgumentBufferBindPoint);

                MTL::Buffer* mtlBuffer = nmetal::GetHandle(cmdList->m_device, buffer);
                IRRuntimeDrawPrimitives(mcmdList->m_pRenderCommandEncoder, mcmdList->m_primitiveType, mtlBuffer, offset);
            }

            void DrawIndexedIndirect(ngfx::command_list_t* cmdList, buffer_t* buffer, u32 offset)
            {
                nmetal::command_list_t* mcmdList = GetComponent<ngfx::command_list_t, nmetal::command_list_t>(cmdList->m_device, cmdList);
                ASSERT(mcmdList->m_pRenderCommandEncoder != nullptr);

                mcmdList->m_pRenderCommandEncoder->setVertexBytes(&mcmdList->m_graphicsArgumentBuffer, sizeof(top_level_argument_buffer_t), kIRArgumentBufferBindPoint);
                mcmdList->m_pRenderCommandEncoder->setFragmentBytes(&mcmdList->m_graphicsArgumentBuffer, sizeof(top_level_argument_buffer_t), kIRArgumentBufferBindPoint);

                MTL::Buffer* mtlBuffer = nmetal::GetHandle(cmdList->m_device, buffer);
                IRRuntimeDrawIndexedPrimitives(mcmdList->m_pRenderCommandEncoder, mcmdList->m_primitiveType, mcmdList->m_indexType, mcmdList->m_pIndexBuffer, mcmdList->m_indexBufferOffset, mtlBuffer, offset);
            }

            void DispatchIndirect(ngfx::command_list_t* cmdList, buffer_t* buffer, u32 offset)
            {
                nmetal::command_list_t* mcmdList = GetComponent<ngfx::command_list_t, nmetal::command_list_t>(cmdList->m_device, cmdList);
                if (mcmdList->m_pCurrentPSO->m_type == enums::PipelineCompute)
                {
                    BeginComputeEncoder(cmdList);

                    nmetal::compute_pipeline_state_t* pipelineState = GetComponent<ngfx::pipeline_state_t, nmetal::compute_pipeline_state_t>(cmdList->m_device, mcmdList->m_pCurrentPSO);
                    mcmdList->m_pComputeCommandEncoder->setComputePipelineState(pipelineState->m_pPSO);
                    mcmdList->m_pComputeCommandEncoder->setBytes(&mcmdList->m_computeArgumentBuffer, sizeof(top_level_argument_buffer_t), kIRArgumentBufferBindPoint);

                    MTL::Size const threadsPerThreadgroup = pipelineState->m_threadsPerThreadgroup;
                    MTL::Buffer*    mtlBuffer             = nmetal::GetHandle(cmdList->m_device, buffer);
                    mcmdList->m_pComputeCommandEncoder->dispatchThreadgroups(mtlBuffer, offset, threadsPerThreadgroup);

                    EndComputeEncoder(mcmdList);
                }
            }

            void DispatchMeshIndirect(ngfx::command_list_t* cmdList, buffer_t* buffer, u32 offset)
            {
                nmetal::command_list_t* mcmdList = GetComponent<ngfx::command_list_t, nmetal::command_list_t>(cmdList->m_device, cmdList);
                ASSERT(mcmdList->m_pRenderCommandEncoder != nullptr);

                mcmdList->m_pRenderCommandEncoder->setObjectBytes(&mcmdList->m_graphicsArgumentBuffer, sizeof(top_level_argument_buffer_t), kIRArgumentBufferBindPoint);
                mcmdList->m_pRenderCommandEncoder->setMeshBytes(&mcmdList->m_graphicsArgumentBuffer, sizeof(top_level_argument_buffer_t), kIRArgumentBufferBindPoint);
                mcmdList->m_pRenderCommandEncoder->setFragmentBytes(&mcmdList->m_graphicsArgumentBuffer, sizeof(top_level_argument_buffer_t), kIRArgumentBufferBindPoint);

                MTL::Size threadsPerObjectThreadgroup = MTL::Size::Make(1, 1, 1);
                MTL::Size threadsPerMeshThreadgroup   = MTL::Size::Make(1, 1, 1);
                if (mcmdList->m_pCurrentPSO->m_type == enums::PipelineMeshShading)
                {
                    meshshading_pipeline_state_t* pipelineState = GetComponent<ngfx::pipeline_state_t, nmetal::meshshading_pipeline_state_t>(cmdList->m_device, mcmdList->m_pCurrentPSO);
                    threadsPerObjectThreadgroup                 = pipelineState->m_threadsPerObjectThreadgroup;
                    threadsPerMeshThreadgroup                   = pipelineState->m_threadsPerMeshThreadgroup;
                }

                nmetal::mbuffer_t* mbuffer = GetComponent<ngfx::buffer_t, nmetal::mbuffer_t>(cmdList->m_device, buffer);
                mcmdList->m_pRenderCommandEncoder->drawMeshThreadgroups(mbuffer->m_pBuffer, offset, threadsPerObjectThreadgroup, threadsPerMeshThreadgroup);
            }

            void MultiDrawIndirect(ngfx::command_list_t* cmdList, u32 max_count, buffer_t* args_buffer, u32 args_buffer_offset, buffer_t* count_buffer, u32 count_buffer_offset)
            {
                nmetal::command_list_t* mcmdList = GetComponent<ngfx::command_list_t, nmetal::command_list_t>(cmdList->m_device, cmdList);
                ASSERT(mcmdList->m_pRenderCommandEncoder != nullptr);
                // todo
            }

            void MultiDrawIndexedIndirect(ngfx::command_list_t* cmdList, u32 max_count, buffer_t* args_buffer, u32 args_buffer_offset, buffer_t* count_buffer, u32 count_buffer_offset)
            {
                nmetal::command_list_t* mcmdList = GetComponent<ngfx::command_list_t, nmetal::command_list_t>(cmdList->m_device, cmdList);
                ASSERT(mcmdList->m_pRenderCommandEncoder != nullptr);
                // todo
            }

            void MultiDispatchIndirect(ngfx::command_list_t* cmdList, u32 max_count, buffer_t* args_buffer, u32 args_buffer_offset, buffer_t* count_buffer, u32 count_buffer_offset)
            {
                nmetal::command_list_t* mcmdList = GetComponent<ngfx::command_list_t, nmetal::command_list_t>(cmdList->m_device, cmdList);
                ASSERT(mcmdList->m_pRenderCommandEncoder != nullptr);
                // todo
            }

            void MultiDispatchMeshIndirect(ngfx::command_list_t* cmdList, u32 max_count, buffer_t* args_buffer, u32 args_buffer_offset, buffer_t* count_buffer, u32 count_buffer_offset)
            {
                nmetal::command_list_t* mcmdList = GetComponent<ngfx::command_list_t, nmetal::command_list_t>(cmdList->m_device, cmdList);
                ASSERT(mcmdList->m_pRenderCommandEncoder != nullptr);
                // todo
            }

            void BuildRayTracingBLAS(ngfx::command_list_t* cmdList, ngfx::blas_t* blas)
            {
                BeginASEncoder(cmdList);

                nmetal::command_list_t* mcmdList  = GetComponent<ngfx::command_list_t, nmetal::command_list_t>(cmdList->m_device, cmdList);
                nmetal::mblas_t*        metalBLAS = GetComponent<ngfx::blas_t, nmetal::mblas_t>(cmdList->m_device, blas);
                mcmdList->m_pASEncoder->buildAccelerationStructure(metalBLAS->m_pAccelerationStructure, metalBLAS->m_pDescriptor, metalBLAS->m_pScratchBuffer, 0);
            }

            void UpdateRayTracingBLAS(ngfx::command_list_t* cmdList, ngfx::blas_t* blas, ngfx::buffer_t* vertex_buffer, u32 vertex_buffer_offset)
            {
                BeginASEncoder(cmdList);

                nmetal::UpdateVertexBuffer(cmdList->m_device, blas, vertex_buffer, vertex_buffer_offset);

                nmetal::command_list_t* mcmdList  = GetComponent<ngfx::command_list_t, nmetal::command_list_t>(cmdList->m_device, cmdList);
                nmetal::mblas_t*        metalBLAS = GetComponent<ngfx::blas_t, nmetal::mblas_t>(cmdList->m_device, blas);
                mcmdList->m_pASEncoder->refitAccelerationStructure(metalBLAS->m_pAccelerationStructure, metalBLAS->m_pDescriptor, metalBLAS->m_pAccelerationStructure, metalBLAS->m_pScratchBuffer, 0);
            }

            void BuildRayTracingTLAS(ngfx::command_list_t* cmdList, ngfx::tlas_t* tlas, const rt_instance_t* instances, u32 instance_count)
            {
                BeginASEncoder(cmdList);

                nmetal::UpdateInstance(cmdList->m_device, tlas, instances, instance_count);

                nmetal::command_list_t* mcmdList  = GetComponent<ngfx::command_list_t, nmetal::command_list_t>(cmdList->m_device, cmdList);
                nmetal::mtlas_t*        metalTLAS = GetComponent<ngfx::tlas_t, nmetal::mtlas_t>(cmdList->m_device, tlas);
                mcmdList->m_pASEncoder->buildAccelerationStructure(metalTLAS->m_pAccelerationStructure, metalTLAS->m_pDescriptor, metalTLAS->m_pScratchBuffer, 0);
            }

        }  // namespace nmetal
    }  // namespace ngfx
}  // namespace ncore
