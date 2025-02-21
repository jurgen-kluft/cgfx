#include "cgfx/gfx.h"

#include "cgfx/d3d12/d3d12_device.h"
#include "cgfx/d3d12/d3d12_command_list.h"

#include "cgfx/metal/metal_device.h"
#include "cgfx/metal/metal_command_list.h"

#include "cgfx/mock/mock_device.h"
#include "cgfx/mock/mock_command_list.h"

namespace ncore
{
    namespace ngfx
    {
#define D_CL_ROUTER1(name, cmdList)                              \
    switch (cmdList->m_device->m_desc.backend)                   \
    {                                                            \
        case enums::Backend_D3D12: nd3d12::name(cmdList); break; \
        case enums::Backend_Metal: nmetal::name(cmdList); break; \
        case enums::Backend_Mock: nmock::name(cmdList); break;   \
    }

#define D_CL_ROUTER2(name, cmdList, ...)                                      \
    switch (cmdList->m_device->m_desc.backend)                                \
    {                                                                         \
        case enums::Backend_D3D12: nd3d12::name(cmdList, __VA_ARGS__); break; \
        case enums::Backend_Metal: nmetal::name(cmdList, __VA_ARGS__); break; \
        case enums::Backend_Mock: nmock::name(cmdList, __VA_ARGS__); break;   \
    }

        command_list_t* CreateCommandList(device_t* device, enums::command_queue_type queue_type, const char* name)
        {
            command_list_t* cmdlist  = CreateInstance<command_list_t>(device, name);
            cmdlist->m_device        = device;
            cmdlist->m_queueType     = queue_type;
            D_CL_ROUTER1(CreateCommandList, cmdlist);
            return cmdlist;
        }

        bool Create(command_list_t* cmdList)
        {
            switch (cmdList->m_device->m_desc.backend)
            {
                case enums::Backend_D3D12: return nd3d12::Create(cmdList);
                case enums::Backend_Metal: return nmetal::Create(cmdList);
                case enums::Backend_Mock: return nmock::Create(cmdList);
            }
            return false;
        }

        void Destroy(command_list_t* cmdList) { D_CL_ROUTER1(Destroy, cmdList); }

        // move to cpp file, implementation can be somewhere else ?
        // void ClearUAV(command_list_t* cmdList, texture_t* texture, descriptor_t* descriptor, const float* value);
        // void ClearUAV(command_list_t* cmdList, buffer_t* buffer, descriptor_t* descriptor, const float* value);
        // void ClearUAV(command_list_t* cmdList, texture_t* texture, descriptor_t* descriptor, const u32* value);
        // void ClearUAV(command_list_t* cmdList, buffer_t* buffer, descriptor_t* descriptor, const u32* value);

        void ResetAllocator(command_list_t* cmdList) { D_CL_ROUTER1(ResetAllocator, cmdList); }
        void Begin(command_list_t* cmdList) { D_CL_ROUTER1(Begin, cmdList); }
        void End(command_list_t* cmdList) { D_CL_ROUTER1(End, cmdList); }
        void Wait(command_list_t* cmdList, fence_t* fence, u64 value) { D_CL_ROUTER2(Wait, cmdList, fence, value); }
        void Signal(command_list_t* cmdList, fence_t* fence, u64 value) { D_CL_ROUTER2(Signal, cmdList, fence, value); }
        void Present(command_list_t* cmdList, swapchain_t* swapchain) { D_CL_ROUTER2(Present, cmdList, swapchain); }
        void Submit(command_list_t* cmdList) { D_CL_ROUTER1(Submit, cmdList); }
        void ResetState(command_list_t* cmdList) { D_CL_ROUTER1(ResetState, cmdList); }

        void BeginProfiling(command_list_t* cmdList) { D_CL_ROUTER1(BeginProfiling, cmdList); }
        void EndProfiling(command_list_t* cmdList) { D_CL_ROUTER1(EndProfiling, cmdList); }
        void BeginEvent(command_list_t* cmdList, const char* event_name) { D_CL_ROUTER2(BeginEvent, cmdList, event_name); }
        void EndEvent(command_list_t* cmdList) { D_CL_ROUTER1(EndEvent, cmdList); }

        void ClearUAV(command_list_t* cmdList, texture_t* texture, descriptor_t* uav, const float* clear_value) { D_CL_ROUTER2(ClearUAV, cmdList, texture, uav, clear_value); }
        void ClearUAV(command_list_t* cmdList, buffer_t* buffer, descriptor_t* uav, const float* clear_value) { D_CL_ROUTER2(ClearUAV, cmdList, buffer, uav, clear_value); }
        void ClearUAV(command_list_t* cmdList, texture_t* texture, descriptor_t* uav, const u32* clear_value) { D_CL_ROUTER2(ClearUAV, cmdList, texture, uav, clear_value); }
        void ClearUAV(command_list_t* cmdList, buffer_t* buffer, descriptor_t* uav, const u32* clear_value) { D_CL_ROUTER2(ClearUAV, cmdList, buffer, uav, clear_value); }

        void CopyBufferToTexture(command_list_t* cmdList, texture_t* dst_texture, u32 mip_level, u32 array_slice, buffer_t* src_buffer, u32 offset) { D_CL_ROUTER2(CopyBufferToTexture, cmdList, dst_texture, mip_level, array_slice, src_buffer, offset); }
        void CopyTextureToBuffer(command_list_t* cmdList, buffer_t* dst_buffer, u32 offset, texture_t* src_texture, u32 mip_level, u32 array_slice) { D_CL_ROUTER2(CopyTextureToBuffer, cmdList, dst_buffer, offset, src_texture, mip_level, array_slice); }
        void CopyBuffer(command_list_t* cmdList, buffer_t* dst, u32 dst_offset, buffer_t* src, u32 src_offset, u32 size) { D_CL_ROUTER2(CopyBuffer, cmdList, dst, dst_offset, src, src_offset, size); }
        void CopyTexture(command_list_t* cmdList, texture_t* dst, u32 dst_mip, u32 dst_array, texture_t* src, u32 src_mip, u32 src_array) { D_CL_ROUTER2(CopyTexture, cmdList, dst, dst_mip, dst_array, src, src_mip, src_array); }
        void WriteBuffer(command_list_t* cmdList, buffer_t* buffer, u32 offset, u32 data) { D_CL_ROUTER2(WriteBuffer, cmdList, buffer, offset, data); }
        void UpdateTileMappings(command_list_t* cmdList, texture_t* texture, heap_t* heap, u32 mapping_count, const tile_mapping_t* mappings) { D_CL_ROUTER2(UpdateTileMappings, cmdList, texture, heap, mapping_count, mappings); }

        void TextureBarrier(command_list_t* cmdList, texture_t* texture, u32 sub_resource, enums::access_flags access_before, enums::access_flags access_after) { D_CL_ROUTER2(TextureBarrier, cmdList, texture, sub_resource, access_before, access_after); }
        void BufferBarrier(command_list_t* cmdList, buffer_t* buffer, enums::access_flags access_before, enums::access_flags access_after) { D_CL_ROUTER2(BufferBarrier, cmdList, buffer, access_before, access_after); }
        void GlobalBarrier(command_list_t* cmdList, enums::access_flags access_before, enums::access_flags access_after) { D_CL_ROUTER2(GlobalBarrier, cmdList, access_before, access_after); }
        void FlushBarriers(command_list_t* cmdList) { D_CL_ROUTER1(FlushBarriers, cmdList); }

        void BeginRenderPass(command_list_t* cmdList, const renderpass_desc_t& render_pass) { D_CL_ROUTER2(BeginRenderPass, cmdList, render_pass); }
        void EndRenderPass(command_list_t* cmdList) { D_CL_ROUTER1(EndRenderPass, cmdList); }
        void SetPipelineState(command_list_t* cmdList, pipeline_state_t* state) { D_CL_ROUTER2(SetPipelineState, cmdList, state); }
        void SetStencilReference(command_list_t* cmdList, u8 stencil) { D_CL_ROUTER2(SetStencilReference, cmdList, stencil); }
        void SetBlendFactor(command_list_t* cmdList, const float* blend_factor) { D_CL_ROUTER2(SetBlendFactor, cmdList, blend_factor); }
        void SetIndexBuffer(command_list_t* cmdList, buffer_t* buffer, u32 offset, enums::format format) { D_CL_ROUTER2(SetIndexBuffer, cmdList, buffer, offset, format); }
        void SetViewport(command_list_t* cmdList, u32 x, u32 y, u32 width, u32 height) { D_CL_ROUTER2(SetViewport, cmdList, x, y, width, height); }
        void SetScissorRect(command_list_t* cmdList, u32 x, u32 y, u32 width, u32 height) { D_CL_ROUTER2(SetScissorRect, cmdList, x, y, width, height); }
        void SetGraphicsConstants(command_list_t* cmdList, u32 slot, const void* data, s64 data_size) { D_CL_ROUTER2(SetGraphicsConstants, cmdList, slot, data, data_size); }
        void SetComputeConstants(command_list_t* cmdList, u32 slot, const void* data, s64 data_size) { D_CL_ROUTER2(SetComputeConstants, cmdList, slot, data, data_size); }

        void Draw(command_list_t* cmdList, u32 vertex_count, u32 instance_count) { D_CL_ROUTER2(Draw, cmdList, vertex_count, instance_count); }
        void DrawIndexed(command_list_t* cmdList, u32 index_count, u32 instance_count, u32 index_offset) { D_CL_ROUTER2(DrawIndexed, cmdList, index_count, instance_count, index_offset); }
        void Dispatch(command_list_t* cmdList, u32 group_count_x, u32 group_count_y, u32 group_count_z) { D_CL_ROUTER2(Dispatch, cmdList, group_count_x, group_count_y, group_count_z); }
        void DispatchMesh(command_list_t* cmdList, u32 group_count_x, u32 group_count_y, u32 group_count_z) { D_CL_ROUTER2(DispatchMesh, cmdList, group_count_x, group_count_y, group_count_z); }

        void DrawIndirect(command_list_t* cmdList, buffer_t* buffer, u32 offset) { D_CL_ROUTER2(DrawIndirect, cmdList, buffer, offset); }
        void DrawIndexedIndirect(command_list_t* cmdList, buffer_t* buffer, u32 offset) { D_CL_ROUTER2(DrawIndexedIndirect, cmdList, buffer, offset); }
        void DispatchIndirect(command_list_t* cmdList, buffer_t* buffer, u32 offset) { D_CL_ROUTER2(DispatchIndirect, cmdList, buffer, offset); }
        void DispatchMeshIndirect(command_list_t* cmdList, buffer_t* buffer, u32 offset) { D_CL_ROUTER2(DispatchMeshIndirect, cmdList, buffer, offset); }

        void MultiDrawIndirect(command_list_t* cmdList, u32 max_count, buffer_t* args_buffer, u32 args_buffer_offset, buffer_t* count_buffer, u32 count_buffer_offset)
        {
            D_CL_ROUTER2(MultiDrawIndirect, cmdList, max_count, args_buffer, args_buffer_offset, count_buffer, count_buffer_offset);
        }
        void MultiDrawIndexedIndirect(command_list_t* cmdList, u32 max_count, buffer_t* args_buffer, u32 args_buffer_offset, buffer_t* count_buffer, u32 count_buffer_offset)
        {
            D_CL_ROUTER2(MultiDrawIndexedIndirect, cmdList, max_count, args_buffer, args_buffer_offset, count_buffer, count_buffer_offset);
        }
        void MultiDispatchIndirect(command_list_t* cmdList, u32 max_count, buffer_t* args_buffer, u32 args_buffer_offset, buffer_t* count_buffer, u32 count_buffer_offset)
        {
            D_CL_ROUTER2(MultiDispatchIndirect, cmdList, max_count, args_buffer, args_buffer_offset, count_buffer, count_buffer_offset);
        }
        void MultiDispatchMeshIndirect(command_list_t* cmdList, u32 max_count, buffer_t* args_buffer, u32 args_buffer_offset, buffer_t* count_buffer, u32 count_buffer_offset)
        {
            D_CL_ROUTER2(MultiDispatchMeshIndirect, cmdList, max_count, args_buffer, args_buffer_offset, count_buffer, count_buffer_offset);
        }

        void BuildRayTracingBLAS(command_list_t* cmdList, blas_t* blas) { D_CL_ROUTER2(BuildRayTracingBLAS, cmdList, blas); }
        void UpdateRayTracingBLAS(command_list_t* cmdList, blas_t* blas, buffer_t* vertex_buffer, u32 vertex_buffer_offset) { D_CL_ROUTER2(UpdateRayTracingBLAS, cmdList, blas, vertex_buffer, vertex_buffer_offset); }
        void BuildRayTracingTLAS(command_list_t* cmdList, tlas_t* tlas, const rt_instance_t* instances, u32 instance_count) { D_CL_ROUTER2(BuildRayTracingTLAS, cmdList, tlas, instances, instance_count); }

    }  // namespace ngfx
}  // namespace ncore
