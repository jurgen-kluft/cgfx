#ifndef __CGFX_MOCK_COMMAND_LIST_H__
#define __CGFX_MOCK_COMMAND_LIST_H__

#include "cgfx/gfx_command_list.h"

namespace ncore
{
    namespace ngfx
    {
        struct device_t;

        namespace nmock
        {
            ngfx::command_list_t* CreateCommandList(ngfx::command_list_t* cmdList);
            bool                  Create(ngfx::command_list_t*);
            void                  Destroy(ngfx::command_list_t*);
            void*                 GetHandle(ngfx::command_list_t*);

            void ResetAllocator(ngfx::command_list_t* commandList);
            void Begin(ngfx::command_list_t* commandList);
            void End(ngfx::command_list_t* commandList);
            void Wait(ngfx::command_list_t* commandList, fence_t* fence, u64 value);
            void Signal(ngfx::command_list_t* commandList, fence_t* fence, u64 value);
            void Present(ngfx::command_list_t* commandList, swapchain_t* swapchain);
            void Submit(ngfx::command_list_t* commandList);
            void ResetState(ngfx::command_list_t* commandList);

            void BeginProfiling(ngfx::command_list_t* commandList);
            void EndProfiling(ngfx::command_list_t* commandList);
            void BeginEvent(ngfx::command_list_t* commandList, const char* event_name);
            void EndEvent(ngfx::command_list_t* commandList);

            void CopyBufferToTexture(ngfx::command_list_t* commandList, texture_t* dst_texture, u32 mip_level, u32 array_slice, buffer_t* src_buffer, u32 offset);
            void CopyTextureToBuffer(ngfx::command_list_t* commandList, buffer_t* dst_buffer, u32 offset, texture_t* src_texture, u32 mip_level, u32 array_slice);
            void CopyBuffer(ngfx::command_list_t* commandList, buffer_t* dst, u32 dst_offset, buffer_t* src, u32 src_offset, u32 size);
            void CopyTexture(ngfx::command_list_t* commandList, texture_t* dst, u32 dst_mip, u32 dst_array, texture_t* src, u32 src_mip, u32 src_array);
            void WriteBuffer(ngfx::command_list_t* commandList, buffer_t* buffer, u32 offset, u32 data);
            void UpdateTileMappings(ngfx::command_list_t* commandList, texture_t* texture, heap_t* heap, u32 mapping_count, const tile_mapping_t* mappings);

            void ClearUAV(command_list_t* cmdList, texture_t* texture, descriptor_t* uav, const float* value);
            void ClearUAV(command_list_t* cmdList, buffer_t* buffer, descriptor_t* uav, const float* value);
            void ClearUAV(command_list_t* cmdList, texture_t* texture, descriptor_t* uav, const u32* value);
            void ClearUAV(command_list_t* cmdList, buffer_t* buffer, descriptor_t* uav, const u32* value);

            void TextureBarrier(ngfx::command_list_t* commandList, texture_t* texture, u32 sub_resource, enums::access_flags access_before, enums::access_flags access_after);
            void BufferBarrier(ngfx::command_list_t* commandList, buffer_t* buffer, enums::access_flags access_before, enums::access_flags access_after);
            void GlobalBarrier(ngfx::command_list_t* commandList, enums::access_flags access_before, enums::access_flags access_after);
            void FlushBarriers(ngfx::command_list_t* commandList);

            void BeginRenderPass(ngfx::command_list_t* commandList, const renderpass_desc_t& render_pass);
            void EndRenderPass(ngfx::command_list_t* commandList);
            void SetPipelineState(ngfx::command_list_t* commandList, pipeline_state_t* state);
            void SetStencilReference(ngfx::command_list_t* commandList, u8 stencil);
            void SetBlendFactor(ngfx::command_list_t* commandList, const float* blend_factor);
            void SetIndexBuffer(ngfx::command_list_t* commandList, buffer_t* buffer, u32 offset, enums::format format);
            void SetViewport(ngfx::command_list_t* commandList, u32 x, u32 y, u32 width, u32 height);
            void SetScissorRect(ngfx::command_list_t* commandList, u32 x, u32 y, u32 width, u32 height);
            void SetGraphicsConstants(ngfx::command_list_t* commandList, u32 slot, const void* data, s64 data_size);
            void SetComputeConstants(ngfx::command_list_t* commandList, u32 slot, const void* data, s64 data_size);

            void Draw(ngfx::command_list_t* commandList, u32 vertex_count, u32 instance_count = 1);
            void DrawIndexed(ngfx::command_list_t* commandList, u32 index_count, u32 instance_count = 1, u32 index_offset = 0);
            void Dispatch(ngfx::command_list_t* commandList, u32 group_count_x, u32 group_count_y, u32 group_count_z);
            void DispatchMesh(ngfx::command_list_t* commandList, u32 group_count_x, u32 group_count_y, u32 group_count_z);

            void DrawIndirect(ngfx::command_list_t* commandList, buffer_t* buffer, u32 offset);
            void DrawIndexedIndirect(ngfx::command_list_t* commandList, buffer_t* buffer, u32 offset);
            void DispatchIndirect(ngfx::command_list_t* commandList, buffer_t* buffer, u32 offset);
            void DispatchMeshIndirect(ngfx::command_list_t* commandList, buffer_t* buffer, u32 offset);

            void MultiDrawIndirect(ngfx::command_list_t* commandList, u32 max_count, buffer_t* args_buffer, u32 args_buffer_offset, buffer_t* count_buffer, u32 count_buffer_offset);
            void MultiDrawIndexedIndirect(ngfx::command_list_t* commandList, u32 max_count, buffer_t* args_buffer, u32 args_buffer_offset, buffer_t* count_buffer, u32 count_buffer_offset);
            void MultiDispatchIndirect(ngfx::command_list_t* commandList, u32 max_count, buffer_t* args_buffer, u32 args_buffer_offset, buffer_t* count_buffer, u32 count_buffer_offset);
            void MultiDispatchMeshIndirect(ngfx::command_list_t* commandList, u32 max_count, buffer_t* args_buffer, u32 args_buffer_offset, buffer_t* count_buffer, u32 count_buffer_offset);

            void BuildRayTracingBLAS(ngfx::command_list_t* commandList, blas_t* blas);
            void UpdateRayTracingBLAS(ngfx::command_list_t* commandList, blas_t* blas, buffer_t* vertex_buffer, u32 vertex_buffer_offset);
            void BuildRayTracingTLAS(ngfx::command_list_t* commandList, tlas_t* tlas, const rt_instance_t* instances, u32 instance_count);
        }  // namespace nmock
    }  // namespace ngfx
}  // namespace ncore
#endif
