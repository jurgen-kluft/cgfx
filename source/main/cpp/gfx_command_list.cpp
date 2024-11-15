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
        command_list_t* CreateCommandList(device_t* device, enums::command_queue_type queue_type, const char* name)
        {
            resource_t*     resource = CreateObject<resource_t>(device, name);
            command_list_t* cmdlist  = AddComponent<resource_t, command_list_t>(device, resource);

            cmdlist->m_device    = device;
            cmdlist->m_queueType = queue_type;
            return cmdlist;
        }

#define D_CL_ROUTER1(name, cl)                              \
    switch (cl->m_device->m_desc.backend)                   \
    {                                                       \
        case enums::Backend_D3D12: nd3d12::name(cl); break; \
        case enums::Backend_Metal: nmetal::name(cl); break; \
        case enums::Backend_Mock: nmock::name(cl); break;   \
    }

#define D_CL_ROUTER2(name, cl, ...)                                      \
    switch (cl->m_device->m_desc.backend)                                \
    {                                                                    \
        case enums::Backend_D3D12: nd3d12::name(cl, __VA_ARGS__); break; \
        case enums::Backend_Metal: nmetal::name(cl, __VA_ARGS__); break; \
        case enums::Backend_Mock: nmock::name(cl, __VA_ARGS__); break;   \
    }

        bool Create(command_list_t* cl)
        {
            switch (cl->m_device->m_desc.backend)
            {
                case enums::Backend_D3D12: return nd3d12::Create(cl);
                case enums::Backend_Metal: return nmetal::Create(cl);
                case enums::Backend_Mock: return nmock::Create(cl);
            }
            return false;
        }

        void Destroy(command_list_t* cl) { D_CL_ROUTER1(Destroy, cl); }

        // move to cpp file, implementation can be somewhere else ?
        void ClearUAV(command_list_t* cl, resource_t* resource, descriptor_t* descriptor, const uav_desc_t& uavDesc, const float* value);
        void ClearUAV(command_list_t* cl, resource_t* resource, descriptor_t* descriptor, const uav_desc_t& uavDesc, const u32* value);

        void ResetAllocator(command_list_t* cl) { D_CL_ROUTER1(ResetAllocator, cl); }
        void Begin(command_list_t* cl) { D_CL_ROUTER1(Begin, cl); }
        void End(command_list_t* cl) { D_CL_ROUTER1(End, cl); }
        void Wait(command_list_t* cl, fence_t* fence, u64 value) { D_CL_ROUTER2(Wait, cl, fence, value); }
        void Signal(command_list_t* cl, fence_t* fence, u64 value) { D_CL_ROUTER2(Signal, cl, fence, value); }
        void Present(command_list_t* cl, swapchain_t* swapchain) { D_CL_ROUTER2(Present, cl, swapchain); }
        void Submit(command_list_t* cl) { D_CL_ROUTER1(Submit, cl); }
        void ResetState(command_list_t* cl) { D_CL_ROUTER1(ResetState, cl); }

        void BeginProfiling(command_list_t* cl) { D_CL_ROUTER1(BeginProfiling, cl); }
        void EndProfiling(command_list_t* cl) { D_CL_ROUTER1(EndProfiling, cl); }
        void BeginEvent(command_list_t* cl, const char* event_name) { D_CL_ROUTER2(BeginEvent, cl, event_name); }
        void EndEvent(command_list_t* cl) { D_CL_ROUTER1(EndEvent, cl); }

        void CopyBufferToTexture(command_list_t* cl, texture_t* dst_texture, u32 mip_level, u32 array_slice, buffer_t* src_buffer, u32 offset) { D_CL_ROUTER2(CopyBufferToTexture, cl, dst_texture, mip_level, array_slice, src_buffer, offset); }
        void CopyTextureToBuffer(command_list_t* cl, buffer_t* dst_buffer, u32 offset, texture_t* src_texture, u32 mip_level, u32 array_slice) { D_CL_ROUTER2(CopyTextureToBuffer, cl, dst_buffer, offset, src_texture, mip_level, array_slice); }
        void CopyBuffer(command_list_t* cl, buffer_t* dst, u32 dst_offset, buffer_t* src, u32 src_offset, u32 size) { D_CL_ROUTER2(CopyBuffer, cl, dst, dst_offset, src, src_offset, size); }
        void CopyTexture(command_list_t* cl, texture_t* dst, u32 dst_mip, u32 dst_array, texture_t* src, u32 src_mip, u32 src_array) { D_CL_ROUTER2(CopyTexture, cl, dst, dst_mip, dst_array, src, src_mip, src_array); }
        void ClearUAV(command_list_t* cl, resource_t* resource, descriptor_t* uav, const float* clear_value) { D_CL_ROUTER2(ClearUAV, cl, resource, uav, clear_value); }
        void ClearUAV(command_list_t* cl, resource_t* resource, descriptor_t* uav, const u32* clear_value) { D_CL_ROUTER2(ClearUAV, cl, resource, uav, clear_value); }
        void WriteBuffer(command_list_t* cl, buffer_t* buffer, u32 offset, u32 data) { D_CL_ROUTER2(WriteBuffer, cl, buffer, offset, data); }
        void UpdateTileMappings(command_list_t* cl, texture_t* texture, heap_t* heap, u32 mapping_count, const GfxTileMapping* mappings) { D_CL_ROUTER2(UpdateTileMappings, cl, texture, heap, mapping_count, mappings); }

        void TextureBarrier(command_list_t* cl, texture_t* texture, u32 sub_resource, enums::access_flags access_before, enums::access_flags access_after) { D_CL_ROUTER2(TextureBarrier, cl, texture, sub_resource, access_before, access_after); }
        void BufferBarrier(command_list_t* cl, buffer_t* buffer, enums::access_flags access_before, enums::access_flags access_after) { D_CL_ROUTER2(BufferBarrier, cl, buffer, access_before, access_after); }
        void GlobalBarrier(command_list_t* cl, enums::access_flags access_before, enums::access_flags access_after) { D_CL_ROUTER2(GlobalBarrier, cl, access_before, access_after); }
        void FlushBarriers(command_list_t* cl) { D_CL_ROUTER1(FlushBarriers, cl); }

        void BeginRenderPass(command_list_t* cl, const GfxRenderPassDesc& render_pass) { D_CL_ROUTER2(BeginRenderPass, cl, render_pass); }
        void EndRenderPass(command_list_t* cl) { D_CL_ROUTER1(EndRenderPass, cl); }
        void SetPipelineState(command_list_t* cl, pipeline_state_t* state) { D_CL_ROUTER2(SetPipelineState, cl, state); }
        void SetStencilReference(command_list_t* cl, u8 stencil) { D_CL_ROUTER2(SetStencilReference, cl, stencil); }
        void SetBlendFactor(command_list_t* cl, const float* blend_factor) { D_CL_ROUTER2(SetBlendFactor, cl, blend_factor); }
        void SetIndexBuffer(command_list_t* cl, buffer_t* buffer, u32 offset, enums::format format) { D_CL_ROUTER2(SetIndexBuffer, cl, buffer, offset, format); }
        void SetViewport(command_list_t* cl, u32 x, u32 y, u32 width, u32 height) { D_CL_ROUTER2(SetViewport, cl, x, y, width, height); }
        void SetScissorRect(command_list_t* cl, u32 x, u32 y, u32 width, u32 height) { D_CL_ROUTER2(SetScissorRect, cl, x, y, width, height); }
        void SetGraphicsConstants(command_list_t* cl, u32 slot, const void* data, s64 data_size) { D_CL_ROUTER2(SetGraphicsConstants, cl, slot, data, data_size); }
        void SetComputeConstants(command_list_t* cl, u32 slot, const void* data, s64 data_size) { D_CL_ROUTER2(SetComputeConstants, cl, slot, data, data_size); }

        void Draw(command_list_t* cl, u32 vertex_count, u32 instance_count) { D_CL_ROUTER2(Draw, cl, vertex_count, instance_count); }
        void DrawIndexed(command_list_t* cl, u32 index_count, u32 instance_count = 1, u32 index_offset) { D_CL_ROUTER2(DrawIndexed, cl, index_count, instance_count, index_offset); }
        void Dispatch(command_list_t* cl, u32 group_count_x, u32 group_count_y, u32 group_count_z) { D_CL_ROUTER2(Dispatch, cl, group_count_x, group_count_y, group_count_z); }
        void DispatchMesh(command_list_t* cl, u32 group_count_x, u32 group_count_y, u32 group_count_z) { D_CL_ROUTER2(DispatchMesh, cl, group_count_x, group_count_y, group_count_z); }

        void DrawIndirect(command_list_t* cl, buffer_t* buffer, u32 offset) { D_CL_ROUTER2(DrawIndirect, cl, buffer, offset); }
        void DrawIndexedIndirect(command_list_t* cl, buffer_t* buffer, u32 offset) { D_CL_ROUTER2(DrawIndexedIndirect, cl, buffer, offset); }
        void DispatchIndirect(command_list_t* cl, buffer_t* buffer, u32 offset) { D_CL_ROUTER2(DispatchIndirect, cl, buffer, offset); }
        void DispatchMeshIndirect(command_list_t* cl, buffer_t* buffer, u32 offset) { D_CL_ROUTER2(DispatchMeshIndirect, cl, buffer, offset); }

        void MultiDrawIndirect(command_list_t* cl, u32 max_count, buffer_t* args_buffer, u32 args_buffer_offset, buffer_t* count_buffer, u32 count_buffer_offset)
        {
            D_CL_ROUTER2(MultiDrawIndirect, cl, max_count, args_buffer, args_buffer_offset, count_buffer, count_buffer_offset);
        }
        void MultiDrawIndexedIndirect(command_list_t* cl, u32 max_count, buffer_t* args_buffer, u32 args_buffer_offset, buffer_t* count_buffer, u32 count_buffer_offset)
        {
            D_CL_ROUTER2(MultiDrawIndexedIndirect, cl, max_count, args_buffer, args_buffer_offset, count_buffer, count_buffer_offset);
        }
        void MultiDispatchIndirect(command_list_t* cl, u32 max_count, buffer_t* args_buffer, u32 args_buffer_offset, buffer_t* count_buffer, u32 count_buffer_offset)
        {
            D_CL_ROUTER2(MultiDispatchIndirect, cl, max_count, args_buffer, args_buffer_offset, count_buffer, count_buffer_offset);
        }
        void MultiDispatchMeshIndirect(command_list_t* cl, u32 max_count, buffer_t* args_buffer, u32 args_buffer_offset, buffer_t* count_buffer, u32 count_buffer_offset)
        {
            D_CL_ROUTER2(MultiDispatchMeshIndirect, cl, max_count, args_buffer, args_buffer_offset, count_buffer, count_buffer_offset);
        }

        void BuildRayTracingBLAS(command_list_t* cl, blas_t* blas) { D_CL_ROUTER2(BuildRayTracingBLAS, cl, blas); }
        void UpdateRayTracingBLAS(command_list_t* cl, blas_t* blas, buffer_t* vertex_buffer, u32 vertex_buffer_offset) { D_CL_ROUTER2(UpdateRayTracingBLAS, cl, blas, vertex_buffer, vertex_buffer_offset); }
        void BuildRayTracingTLAS(command_list_t* cl, tlas_t* tlas, const rt_instance_t* instances, u32 instance_count) { D_CL_ROUTER2(BuildRayTracingTLAS, cl, tlas, instances, instance_count); }

    }  // namespace ngfx
}  // namespace ncore
