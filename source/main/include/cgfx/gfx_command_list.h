#ifndef __CGFX_GFX_COMMAND_LIST_H__
#define __CGFX_GFX_COMMAND_LIST_H__

#include "cgfx/gfx_resource.h"

namespace ncore
{
    namespace ngfx
    {
        class IGfxBuffer;
        class IGfxCommandList;
        class IGfxDescriptor;
        class IGfxFence;
        class IGfxHeap;
        class IGfxPipelineState;
        class IGfxRayTracingBLAS;
        class IGfxRayTracingTLAS;
        class IGfxResource;
        class IGfxSwapchain;
        class IGfxTexture;

        class IGfxClearUavApi
        {
        public:
            virtual void ClearUAV(IGfxCommandList* commandList, IGfxResource* resource, IGfxDescriptor* descriptor, const GfxUnorderedAccessViewDesc& uavDesc, const float* value) = 0;
            virtual void ClearUAV(IGfxCommandList* commandList, IGfxResource* resource, IGfxDescriptor* descriptor, const GfxUnorderedAccessViewDesc& uavDesc, const u32* value)   = 0;
        };

        class IGfxCommandList : public IGfxResource
        {
        public:
            virtual ~IGfxCommandList() {}

            GfxCommandQueue GetQueue() const { return m_queueType; }

            virtual void ResetAllocator()                    = 0;
            virtual void Begin()                             = 0;
            virtual void End()                               = 0;
            virtual void Wait(IGfxFence* fence, u64 value)   = 0;
            virtual void Signal(IGfxFence* fence, u64 value) = 0;
            virtual void Present(IGfxSwapchain* swapchain)   = 0;
            virtual void Submit()                            = 0;
            virtual void ResetState()                        = 0;

            virtual void BeginProfiling()                            = 0;
            virtual void EndProfiling()                              = 0;
            virtual void BeginEvent(const char* event_name) = 0;
            virtual void EndEvent()                                  = 0;

            virtual void CopyBufferToTexture(IGfxTexture* dst_texture, u32 mip_level, u32 array_slice, IGfxBuffer* src_buffer, u32 offset) = 0;
            virtual void CopyTextureToBuffer(IGfxBuffer* dst_buffer, u32 offset, IGfxTexture* src_texture, u32 mip_level, u32 array_slice) = 0;
            virtual void CopyBuffer(IGfxBuffer* dst, u32 dst_offset, IGfxBuffer* src, u32 src_offset, u32 size)                            = 0;
            virtual void CopyTexture(IGfxTexture* dst, u32 dst_mip, u32 dst_array, IGfxTexture* src, u32 src_mip, u32 src_array)           = 0;
            virtual void ClearUAV(IGfxResource* resource, IGfxDescriptor* uav, const float* clear_value, IGfxClearUavApi* clear_uav)       = 0;
            virtual void ClearUAV(IGfxResource* resource, IGfxDescriptor* uav, const u32* clear_value, IGfxClearUavApi* clear_uav)         = 0;
            virtual void WriteBuffer(IGfxBuffer* buffer, u32 offset, u32 data)                                                             = 0;
            virtual void UpdateTileMappings(IGfxTexture* texture, IGfxHeap* heap, u32 mapping_count, const GfxTileMapping* mappings)       = 0;

            virtual void TextureBarrier(IGfxTexture* texture, u32 sub_resource, GfxAccessFlags access_before, GfxAccessFlags access_after) = 0;
            virtual void BufferBarrier(IGfxBuffer* buffer, GfxAccessFlags access_before, GfxAccessFlags access_after)                      = 0;
            virtual void GlobalBarrier(GfxAccessFlags access_before, GfxAccessFlags access_after)                                          = 0;
            virtual void FlushBarriers()                                                                                                   = 0;

            virtual void BeginRenderPass(const GfxRenderPassDesc& render_pass)            = 0;
            virtual void EndRenderPass()                                                  = 0;
            virtual void SetPipelineState(IGfxPipelineState* state)                       = 0;
            virtual void SetStencilReference(u8 stencil)                                  = 0;
            virtual void SetBlendFactor(const float* blend_factor)                        = 0;
            virtual void SetIndexBuffer(IGfxBuffer* buffer, u32 offset, GfxFormat format) = 0;
            virtual void SetViewport(u32 x, u32 y, u32 width, u32 height)                 = 0;
            virtual void SetScissorRect(u32 x, u32 y, u32 width, u32 height)              = 0;
            virtual void SetGraphicsConstants(u32 slot, const void* data, s64 data_size)  = 0;
            virtual void SetComputeConstants(u32 slot, const void* data, s64 data_size)   = 0;

            virtual void Draw(u32 vertex_count, u32 instance_count = 1)                             = 0;
            virtual void DrawIndexed(u32 index_count, u32 instance_count = 1, u32 index_offset = 0) = 0;
            virtual void Dispatch(u32 group_count_x, u32 group_count_y, u32 group_count_z)          = 0;
            virtual void DispatchMesh(u32 group_count_x, u32 group_count_y, u32 group_count_z)      = 0;

            virtual void DrawIndirect(IGfxBuffer* buffer, u32 offset)         = 0;
            virtual void DrawIndexedIndirect(IGfxBuffer* buffer, u32 offset)  = 0;
            virtual void DispatchIndirect(IGfxBuffer* buffer, u32 offset)     = 0;
            virtual void DispatchMeshIndirect(IGfxBuffer* buffer, u32 offset) = 0;

            virtual void MultiDrawIndirect(u32 max_count, IGfxBuffer* args_buffer, u32 args_buffer_offset, IGfxBuffer* count_buffer, u32 count_buffer_offset)         = 0;
            virtual void MultiDrawIndexedIndirect(u32 max_count, IGfxBuffer* args_buffer, u32 args_buffer_offset, IGfxBuffer* count_buffer, u32 count_buffer_offset)  = 0;
            virtual void MultiDispatchIndirect(u32 max_count, IGfxBuffer* args_buffer, u32 args_buffer_offset, IGfxBuffer* count_buffer, u32 count_buffer_offset)     = 0;
            virtual void MultiDispatchMeshIndirect(u32 max_count, IGfxBuffer* args_buffer, u32 args_buffer_offset, IGfxBuffer* count_buffer, u32 count_buffer_offset) = 0;

            virtual void BuildRayTracingBLAS(IGfxRayTracingBLAS* blas)                                                             = 0;
            virtual void UpdateRayTracingBLAS(IGfxRayTracingBLAS* blas, IGfxBuffer* vertex_buffer, u32 vertex_buffer_offset)       = 0;
            virtual void BuildRayTracingTLAS(IGfxRayTracingTLAS* tlas, const GfxRayTracing::Instance* instances, u32 instance_count) = 0;

#if MICROPROFILE_GPU_TIMERS
            virtual struct MicroProfileThreadLogGpu* GetProfileLog() const = 0;
#endif

        protected:
            GfxCommandQueue m_queueType;
        };

    }  // namespace ngfx
}  // namespace ncore

#endif
