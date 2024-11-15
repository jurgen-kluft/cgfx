#include "cgfx/mock/mock_command_list.h"
#include "cgfx/mock/mock_device.h"
#include "cgfx/mock/mock_swapchain.h"

namespace ncore
{
    namespace ngfx
    {
        namespace nmock
        {
            bool  Create(ngfx::command_list_t* cl) { return true; }
            void* GetHandle(ngfx::command_list_t* cl) { return nullptr; }
            void  ResetAllocator(ngfx::command_list_t* cl) {}
            void  Begin(ngfx::command_list_t* cl) {}
            void  End(ngfx::command_list_t* cl) {}
            void  Wait(ngfx::command_list_t* cl, fence_t* fence, u64 value) {}
            void  Signal(ngfx::command_list_t* cl, fence_t* fence, u64 value) {}
            void  Present(ngfx::command_list_t* cl, swapchain_t* swapchain) {}
            void  Submit(ngfx::command_list_t* cl) {}
            void  ResetState(ngfx::command_list_t* cl) {}
            void  BeginProfiling(ngfx::command_list_t* cl) {}
            void  EndProfiling(ngfx::command_list_t* cl) {}
            void  BeginEvent(ngfx::command_list_t* cl, const char* event_name) {}
            void  EndEvent(ngfx::command_list_t* cl) {}
            void  CopyBufferToTexture(ngfx::command_list_t* cl, texture_t* dst_texture, u32 mip_level, u32 array_slice, buffer_t* src_buffer, u32 offset) {}
            void  CopyTextureToBuffer(ngfx::command_list_t* cl, buffer_t* dst_buffer, u32 offset, texture_t* src_texture, u32 mip_level, u32 array_slice) {}
            void  CopyBuffer(ngfx::command_list_t* cl, buffer_t* dst, u32 dst_offset, buffer_t* src, u32 src_offset, u32 size) {}
            void  CopyTexture(ngfx::command_list_t* cl, texture_t* dst, u32 dst_mip, u32 dst_array, texture_t* src, u32 src_mip, u32 src_array) {}
            void  ClearUAV(ngfx::command_list_t* cl, resource_t* resource, descriptor_t* uav, const float* clear_value)
            {
                uav_desc_t desc;
                ClearUAV(cl, resource, uav, desc, clear_value);
            }

            void ClearUAV(ngfx::command_list_t* cl, resource_t* resource, descriptor_t* uav, const u32* clear_value)
            {
                uav_desc_t desc;
                ClearUAV(cl, resource, uav, desc, clear_value);
            }

            void WriteBuffer(ngfx::command_list_t* cl, buffer_t* buffer, u32 offset, u32 data) {}
            void UpdateTileMappings(ngfx::command_list_t* cl, texture_t* texture, heap_t* heap, u32 mapping_count, const GfxTileMapping* mappings) {}
            void TextureBarrier(ngfx::command_list_t* cl, texture_t* texture, u32 sub_resource, enums::access_flags access_before, enums::access_flags access_after) {}
            void BufferBarrier(ngfx::command_list_t* cl, buffer_t* buffer, enums::access_flags access_before, enums::access_flags access_after) {}
            void GlobalBarrier(ngfx::command_list_t* cl, enums::access_flags access_before, enums::access_flags access_after) {}
            void FlushBarriers(ngfx::command_list_t* cl) {}
            void BeginRenderPass(ngfx::command_list_t* cl, const GfxRenderPassDesc& render_pass) {}
            void EndRenderPass(ngfx::command_list_t* cl) {}
            void SetPipelineState(ngfx::command_list_t* cl, pipeline_state_t* state) {}
            void SetStencilReference(ngfx::command_list_t* cl, u8 stencil) {}
            void SetBlendFactor(ngfx::command_list_t* cl, const float* blend_factor) {}
            void SetIndexBuffer(ngfx::command_list_t* cl, buffer_t* buffer, u32 offset, enums::format format) {}
            void SetViewport(ngfx::command_list_t* cl, u32 x, u32 y, u32 width, u32 height) {}
            void SetScissorRect(ngfx::command_list_t* cl, u32 x, u32 y, u32 width, u32 height) {}
            void SetGraphicsConstants(ngfx::command_list_t* cl, u32 slot, const void* data, s64 data_size) {}
            void SetComputeConstants(ngfx::command_list_t* cl, u32 slot, const void* data, s64 data_size) {}
            void Draw(ngfx::command_list_t* cl, u32 vertex_count, u32 instance_count) {}
            void DrawIndexed(ngfx::command_list_t* cl, u32 index_count, u32 instance_count, u32 index_offset) {}
            void Dispatch(ngfx::command_list_t* cl, u32 group_count_x, u32 group_count_y, u32 group_count_z) {}
            void DispatchMesh(ngfx::command_list_t* cl, u32 group_count_x, u32 group_count_y, u32 group_count_z) {}
            void DrawIndirect(ngfx::command_list_t* cl, buffer_t* buffer, u32 offset) {}
            void DrawIndexedIndirect(ngfx::command_list_t* cl, buffer_t* buffer, u32 offset) {}
            void DispatchIndirect(ngfx::command_list_t* cl, buffer_t* buffer, u32 offset) {}
            void DispatchMeshIndirect(ngfx::command_list_t* cl, buffer_t* buffer, u32 offset) {}
            void MultiDrawIndirect(ngfx::command_list_t* cl, u32 max_count, buffer_t* args_buffer, u32 args_buffer_offset, buffer_t* count_buffer, u32 count_buffer_offset) {}
            void MultiDrawIndexedIndirect(ngfx::command_list_t* cl, u32 max_count, buffer_t* args_buffer, u32 args_buffer_offset, buffer_t* count_buffer, u32 count_buffer_offset) {}
            void MultiDispatchIndirect(ngfx::command_list_t* cl, u32 max_count, buffer_t* args_buffer, u32 args_buffer_offset, buffer_t* count_buffer, u32 count_buffer_offset) {}
            void MultiDispatchMeshIndirect(ngfx::command_list_t* cl, u32 max_count, buffer_t* args_buffer, u32 args_buffer_offset, buffer_t* count_buffer, u32 count_buffer_offset) {}
            void BuildRayTracingBLAS(ngfx::command_list_t* cl, blas_t* blas) {}
            void UpdateRayTracingBLAS(ngfx::command_list_t* cl, blas_t* blas, buffer_t* vertex_buffer, u32 vertex_buffer_offset) {}
            void BuildRayTracingTLAS(ngfx::command_list_t* cl, tlas_t* tlas, const rt_instance_t* instances, u32 instance_count) {}

#if MICROPROFILE_GPU_TIMERS
            MicroProfileThreadLogGpu* GetProfileLog(ngfx::command_list_t* cl) const { return nullptr; }
#endif
        }  // namespace nmock
    }  // namespace ngfx
}  // namespace ncore
