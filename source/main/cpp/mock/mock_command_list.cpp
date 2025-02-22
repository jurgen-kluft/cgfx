#include "cgfx/mock/mock_command_list.h"
#include "cgfx/mock/mock_device.h"
#include "cgfx/mock/mock_swapchain.h"

namespace ncore
{
    namespace ngfx
    {
        void ClearUAV(command_list_t* cmdList, texture_t* texture, descriptor_t* uav, uav_desc_t const& uav_desc, const float* clear_value);
        void ClearUAV(command_list_t* cmdList, buffer_t* buffer, descriptor_t* uav, uav_desc_t const& uav_desc, const float* clear_value);

        namespace nmock
        {
            command_list_t* CreateCommandList(command_list_t* cmdList)
            {
                // ...
                return cmdList;
            }

            bool  Create(command_list_t* cmdList) { return true; }
            void  ResetAllocator(command_list_t* cmdList) {}
            void  Begin(command_list_t* cmdList) {}
            void  End(command_list_t* cmdList) {}
            void  Wait(command_list_t* cmdList, fence_t* fence, u64 value) {}
            void  Signal(command_list_t* cmdList, fence_t* fence, u64 value) {}
            void  Present(command_list_t* cmdList, swapchain_t* swapchain) {}
            void  Submit(command_list_t* cmdList) {}
            void  ResetState(command_list_t* cmdList) {}
            void  BeginProfiling(command_list_t* cmdList) {}
            void  EndProfiling(command_list_t* cmdList) {}
            void  BeginEvent(command_list_t* cmdList, const char* event_name) {}
            void  EndEvent(command_list_t* cmdList) {}
            void  CopyBufferToTexture(command_list_t* cmdList, texture_t* dst_texture, u32 mip_level, u32 array_slice, buffer_t* src_buffer, u32 offset) {}
            void  CopyTextureToBuffer(command_list_t* cmdList, buffer_t* dst_buffer, u32 offset, texture_t* src_texture, u32 mip_level, u32 array_slice) {}
            void  CopyBuffer(command_list_t* cmdList, buffer_t* dst, u32 dst_offset, buffer_t* src, u32 src_offset, u32 size) {}
            void  CopyTexture(command_list_t* cmdList, texture_t* dst, u32 dst_mip, u32 dst_array, texture_t* src, u32 src_mip, u32 src_array) {}

            void ClearUAV(command_list_t* cmdList, texture_t* texture, descriptor_t* uav, const float* clear_value)
            {
                uav_desc_t desc;
                ngfx::ClearUAV(cmdList, texture, uav, desc, clear_value);
            }

            void ClearUAV(command_list_t* cmdList, buffer_t* buffer, descriptor_t* uav, const float* clear_value)
            {
                uav_desc_t desc;
                ngfx::ClearUAV(cmdList, buffer, uav, desc, clear_value);
            }

            void ClearUAV(command_list_t* cmdList, texture_t* texture, descriptor_t* uav, const u32* value);
            void ClearUAV(command_list_t* cmdList, buffer_t* buffer, descriptor_t* uav, const u32* value);

            void WriteBuffer(command_list_t* cmdList, buffer_t* buffer, u32 offset, u32 data) {}
            void UpdateTileMappings(command_list_t* cmdList, texture_t* texture, heap_t* heap, u32 mapping_count, const tile_mapping_t* mappings) {}
            void TextureBarrier(command_list_t* cmdList, texture_t* texture, u32 sub_resource, enums::access_flags access_before, enums::access_flags access_after) {}
            void BufferBarrier(command_list_t* cmdList, buffer_t* buffer, enums::access_flags access_before, enums::access_flags access_after) {}
            void GlobalBarrier(command_list_t* cmdList, enums::access_flags access_before, enums::access_flags access_after) {}
            void FlushBarriers(command_list_t* cmdList) {}
            void BeginRenderPass(command_list_t* cmdList, const renderpass_desc_t& render_pass) {}
            void EndRenderPass(command_list_t* cmdList) {}
            void SetPipelineState(command_list_t* cmdList, pipeline_state_t* state) {}
            void SetStencilReference(command_list_t* cmdList, u8 stencil) {}
            void SetBlendFactor(command_list_t* cmdList, const float* blend_factor) {}
            void SetIndexBuffer(command_list_t* cmdList, buffer_t* buffer, u32 offset, enums::format format) {}
            void SetViewport(command_list_t* cmdList, u32 x, u32 y, u32 width, u32 height) {}
            void SetScissorRect(command_list_t* cmdList, u32 x, u32 y, u32 width, u32 height) {}
            void SetGraphicsConstants(command_list_t* cmdList, u32 slot, const void* data, s64 data_size) {}
            void SetComputeConstants(command_list_t* cmdList, u32 slot, const void* data, s64 data_size) {}
            void Draw(command_list_t* cmdList, u32 vertex_count, u32 instance_count) {}
            void DrawIndexed(command_list_t* cmdList, u32 index_count, u32 instance_count, u32 index_offset) {}
            void Dispatch(command_list_t* cmdList, u32 group_count_x, u32 group_count_y, u32 group_count_z) {}
            void DispatchMesh(command_list_t* cmdList, u32 group_count_x, u32 group_count_y, u32 group_count_z) {}
            void DrawIndirect(command_list_t* cmdList, buffer_t* buffer, u32 offset) {}
            void DrawIndexedIndirect(command_list_t* cmdList, buffer_t* buffer, u32 offset) {}
            void DispatchIndirect(command_list_t* cmdList, buffer_t* buffer, u32 offset) {}
            void DispatchMeshIndirect(command_list_t* cmdList, buffer_t* buffer, u32 offset) {}
            void MultiDrawIndirect(command_list_t* cmdList, u32 max_count, buffer_t* args_buffer, u32 args_buffer_offset, buffer_t* count_buffer, u32 count_buffer_offset) {}
            void MultiDrawIndexedIndirect(command_list_t* cmdList, u32 max_count, buffer_t* args_buffer, u32 args_buffer_offset, buffer_t* count_buffer, u32 count_buffer_offset) {}
            void MultiDispatchIndirect(command_list_t* cmdList, u32 max_count, buffer_t* args_buffer, u32 args_buffer_offset, buffer_t* count_buffer, u32 count_buffer_offset) {}
            void MultiDispatchMeshIndirect(command_list_t* cmdList, u32 max_count, buffer_t* args_buffer, u32 args_buffer_offset, buffer_t* count_buffer, u32 count_buffer_offset) {}
            void BuildRayTracingBLAS(command_list_t* cmdList, blas_t* blas) {}
            void UpdateRayTracingBLAS(command_list_t* cmdList, blas_t* blas, buffer_t* vertex_buffer, u32 vertex_buffer_offset) {}
            void BuildRayTracingTLAS(command_list_t* cmdList, tlas_t* tlas, const rt_instance_t* instances, u32 instance_count) {}
        }  // namespace nmock
    }  // namespace ngfx
}  // namespace ncore
