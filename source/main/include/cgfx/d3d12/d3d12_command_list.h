#ifndef __CGFX_D3D12_COMMAND_LIST_H__
#define __CGFX_D3D12_COMMAND_LIST_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
    #pragma once
#endif

#include "cgfx/gfx_command_list.h"
#include "cgfx/d3d12/d3d12_header.h"
#include "cbase/c_allocator.h"
#include "cbase/c_carray.h"

namespace ncore
{
    namespace ngfx
    {
#ifdef TARGET_PC
        struct device_t;

        namespace nd3d12
        {
            struct command_list_t
            {
                D_GFX_CS_COMPONENT_SET(enums::ComponentD3D12CommandList);
                ID3D12CommandQueue*         m_pCommandQueue     = nullptr;
                ID3D12CommandAllocator*     m_pCommandAllocator = nullptr;
                ID3D12GraphicsCommandList7* m_pCommandList      = nullptr;

                u32                     m_commandCount = 0;
                ngfx::pipeline_state_t* m_pCurrentPSO  = nullptr;

                carray_t<D3D12_TEXTURE_BARRIER> m_textureBarriers;
                carray_t<D3D12_BUFFER_BARRIER>  m_bufferBarriers;
                carray_t<D3D12_GLOBAL_BARRIER>  m_globalBarriers;

                struct fence_value_t
                {
                    ngfx::fence_t* m_fence;
                    u64            m_value;
                };

                carray_t<fence_value_t>      m_pendingWaits;
                carray_t<fence_value_t>      m_pendingSignals;
                carray_t<ngfx::swapchain_t*> m_pendingSwapchain;

                struct MicroProfileThreadLogGpu* m_pProfileLog   = nullptr;
                int                              m_nProfileQueue = -1;

                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            void CreateCommandList(ngfx::command_list_t* cl);
            void DestroyCommandList(ngfx::command_list_t* cl);
            bool Create(ngfx::command_list_t*);
            void Destroy(ngfx::command_list_t*);

            void ResetAllocator(ngfx::command_list_t* commandList);
            void Begin(ngfx::command_list_t* commandList);
            void End(ngfx::command_list_t* commandList);
            void Wait(ngfx::command_list_t* commandList, ngfx::fence_t* fence, u64 value);
            void Signal(ngfx::command_list_t* commandList, ngfx::fence_t* fence, u64 value);
            void Present(ngfx::command_list_t* commandList, ngfx::swapchain_t* swapchain);
            void Submit(ngfx::command_list_t* commandList);
            void ResetState(ngfx::command_list_t* commandList);

            void BeginProfiling(ngfx::command_list_t* commandList);
            void EndProfiling(ngfx::command_list_t* commandList);
            void BeginEvent(ngfx::command_list_t* commandList, const char* event_name);
            void EndEvent(ngfx::command_list_t* commandList);

            void CopyBufferToTexture(ngfx::command_list_t* commandList, ngfx::texture_t* dst_texture, u32 mip_level, u32 array_slice, ngfx::buffer_t* src_buffer, u32 offset);
            void CopyTextureToBuffer(ngfx::command_list_t* commandList, ngfx::buffer_t* dst_buffer, u32 offset, ngfx::texture_t* src_texture, u32 mip_level, u32 array_slice);
            void CopyBuffer(ngfx::command_list_t* commandList, ngfx::buffer_t* dst, u32 dst_offset, ngfx::buffer_t* src, u32 src_offset, u32 size);
            void CopyTexture(ngfx::command_list_t* commandList, ngfx::texture_t* dst, u32 dst_mip, u32 dst_array, ngfx::texture_t* src, u32 src_mip, u32 src_array);
            void WriteBuffer(ngfx::command_list_t* commandList, ngfx::buffer_t* buffer, u32 offset, u32 data);
            void UpdateTileMappings(ngfx::command_list_t* commandList, ngfx::texture_t* texture, heap_t* heap, u32 mapping_count, const tile_mapping_t* mappings);

            void ClearUAV(ngfx::command_list_t* cmdList, ngfx::texture_t* texture, ngfx::descriptor_t* descriptor, const float* value);
            void ClearUAV(ngfx::command_list_t* cmdList, ngfx::buffer_t* buffer, ngfx::descriptor_t* descriptor, const float* value);
            void ClearUAV(ngfx::command_list_t* cmdList, ngfx::texture_t* texture, ngfx::descriptor_t* descriptor, const u32* value);
            void ClearUAV(ngfx::command_list_t* cmdList, ngfx::buffer_t* buffer, ngfx::descriptor_t* descriptor, const u32* value);

            void TextureBarrier(ngfx::command_list_t* commandList, ngfx::texture_t* texture, u32 sub_resource, enums::access_flags access_before, enums::access_flags access_after);
            void BufferBarrier(ngfx::command_list_t* commandList, ngfx::buffer_t* buffer, enums::access_flags access_before, enums::access_flags access_after);
            void GlobalBarrier(ngfx::command_list_t* commandList, enums::access_flags access_before, enums::access_flags access_after);
            void FlushBarriers(ngfx::command_list_t* commandList);

            void BeginRenderPass(ngfx::command_list_t* commandList, const renderpass_desc_t& render_pass);
            void EndRenderPass(ngfx::command_list_t* commandList);
            void SetPipelineState(ngfx::command_list_t* commandList, ngfx::pipeline_state_t* state);
            void SetStencilReference(ngfx::command_list_t* commandList, u8 stencil);
            void SetBlendFactor(ngfx::command_list_t* commandList, const float* blend_factor);
            void SetIndexBuffer(ngfx::command_list_t* commandList, ngfx::buffer_t* buffer, u32 offset, enums::format format);
            void SetViewport(ngfx::command_list_t* commandList, u32 x, u32 y, u32 width, u32 height);
            void SetScissorRect(ngfx::command_list_t* commandList, u32 x, u32 y, u32 width, u32 height);
            void SetGraphicsConstants(ngfx::command_list_t* commandList, u32 slot, const void* data, s64 data_size);
            void SetComputeConstants(ngfx::command_list_t* commandList, u32 slot, const void* data, s64 data_size);

            void Draw(ngfx::command_list_t* commandList, u32 vertex_count, u32 instance_count = 1);
            void DrawIndexed(ngfx::command_list_t* commandList, u32 index_count, u32 instance_count = 1, u32 index_offset = 0);
            void Dispatch(ngfx::command_list_t* commandList, u32 group_count_x, u32 group_count_y, u32 group_count_z);
            void DispatchMesh(ngfx::command_list_t* commandList, u32 group_count_x, u32 group_count_y, u32 group_count_z);

            void DrawIndirect(ngfx::command_list_t* commandList, ngfx::buffer_t* buffer, u32 offset);
            void DrawIndexedIndirect(ngfx::command_list_t* commandList, ngfx::buffer_t* buffer, u32 offset);
            void DispatchIndirect(ngfx::command_list_t* commandList, ngfx::buffer_t* buffer, u32 offset);
            void DispatchMeshIndirect(ngfx::command_list_t* commandList, ngfx::buffer_t* buffer, u32 offset);

            void MultiDrawIndirect(ngfx::command_list_t* commandList, u32 max_count, ngfx::buffer_t* args_buffer, u32 args_buffer_offset, ngfx::buffer_t* count_buffer, u32 count_buffer_offset);
            void MultiDrawIndexedIndirect(ngfx::command_list_t* commandList, u32 max_count, ngfx::buffer_t* args_buffer, u32 args_buffer_offset, ngfx::buffer_t* count_buffer, u32 count_buffer_offset);
            void MultiDispatchIndirect(ngfx::command_list_t* commandList, u32 max_count, ngfx::buffer_t* args_buffer, u32 args_buffer_offset, ngfx::buffer_t* count_buffer, u32 count_buffer_offset);
            void MultiDispatchMeshIndirect(ngfx::command_list_t* commandList, u32 max_count, ngfx::buffer_t* args_buffer, u32 args_buffer_offset, ngfx::buffer_t* count_buffer, u32 count_buffer_offset);

            void BuildRayTracingBLAS(ngfx::command_list_t* commandList, ngfx::blas_t* blas);
            void UpdateRayTracingBLAS(ngfx::command_list_t* commandList, ngfx::blas_t* blas, ngfx::buffer_t* vertex_buffer, u32 vertex_buffer_offset);
            void BuildRayTracingTLAS(ngfx::command_list_t* commandList, ngfx::tlas_t* tlas, const rt_instance_t* instances, u32 instance_count);

        }  // namespace nd3d12
#else

        namespace nd3d12
        {
            inline void CreateCommandList(ngfx::command_list_t* cmdList) {}
            inline void DestroyCommandList(ngfx::command_list_t* cmdList) {}
            inline bool Create(ngfx::command_list_t* cmdList) { return false; }
            inline void Destroy(ngfx::command_list_t* cmdList) {}

            void ResetAllocator(ngfx::command_list_t* commandList) {}
            void Begin(ngfx::command_list_t* commandList) {}
            void End(ngfx::command_list_t* commandList) {}
            void Wait(ngfx::command_list_t* commandList, ngfx::fence_t* fence, u64 value) {}
            void Signal(ngfx::command_list_t* commandList, ngfx::fence_t* fence, u64 value) {}
            void Present(ngfx::command_list_t* commandList, ngfx::swapchain_t* swapchain) {}
            void Submit(ngfx::command_list_t* commandList) {}
            void ResetState(ngfx::command_list_t* commandList) {}

            void BeginProfiling(ngfx::command_list_t* commandList) {}
            void EndProfiling(ngfx::command_list_t* commandList) {}
            void BeginEvent(ngfx::command_list_t* commandList, const char* event_name) {}
            void EndEvent(ngfx::command_list_t* commandList) {}

            void CopyBufferToTexture(ngfx::command_list_t* commandList, ngfx::texture_t* dst_texture, u32 mip_level, u32 array_slice, ngfx::buffer_t* src_buffer, u32 offset) {}
            void CopyTextureToBuffer(ngfx::command_list_t* commandList, ngfx::buffer_t* dst_buffer, u32 offset, ngfx::texture_t* src_texture, u32 mip_level, u32 array_slice) {}
            void CopyBuffer(ngfx::command_list_t* commandList, ngfx::buffer_t* dst, u32 dst_offset, ngfx::buffer_t* src, u32 src_offset, u32 size) {}
            void CopyTexture(ngfx::command_list_t* commandList, ngfx::texture_t* dst, u32 dst_mip, u32 dst_array, ngfx::texture_t* src, u32 src_mip, u32 src_array) {}

            void ClearUAV(ngfx::command_list_t* commandList, resource_t* resource, descriptor_t* uav, const float* clear_value) {}
            void ClearUAV(ngfx::command_list_t* commandList, resource_t* resource, descriptor_t* uav, const u32* clear_value) {}
            void WriteBuffer(ngfx::command_list_t* commandList, ngfx::buffer_t* buffer, u32 offset, u32 data) {}
            void UpdateTileMappings(ngfx::command_list_t* commandList, ngfx::texture_t* texture, heap_t* heap, u32 mapping_count, const tile_mapping_t* mappings) {}

            void TextureBarrier(ngfx::command_list_t* commandList, ngfx::texture_t* texture, u32 sub_resource, enums::access_flags access_before, enums::access_flags access_after) {}
            void BufferBarrier(ngfx::command_list_t* commandList, ngfx::buffer_t* buffer, enums::access_flags access_before, enums::access_flags access_after) {}
            void GlobalBarrier(ngfx::command_list_t* commandList, enums::access_flags access_before, enums::access_flags access_after) {}
            void FlushBarriers(ngfx::command_list_t* commandList) {}

            void BeginRenderPass(ngfx::command_list_t* commandList, const renderpass_desc_t& render_pass) {}
            void EndRenderPass(ngfx::command_list_t* commandList) {}
            void SetPipelineState(ngfx::command_list_t* commandList, ngfx::pipeline_state_t* state) {}
            void SetStencilReference(ngfx::command_list_t* commandList, u8 stencil) {}
            void SetBlendFactor(ngfx::command_list_t* commandList, const float* blend_factor) {}
            void SetIndexBuffer(ngfx::command_list_t* commandList, ngfx::buffer_t* buffer, u32 offset, enums::format format) {}
            void SetViewport(ngfx::command_list_t* commandList, u32 x, u32 y, u32 width, u32 height) {}
            void SetScissorRect(ngfx::command_list_t* commandList, u32 x, u32 y, u32 width, u32 height) {}
            void SetGraphicsConstants(ngfx::command_list_t* commandList, u32 slot, const void* data, s64 data_size) {}
            void SetComputeConstants(ngfx::command_list_t* commandList, u32 slot, const void* data, s64 data_size) {}

            void Draw(ngfx::command_list_t* commandList, u32 vertex_count, u32 instance_count = 1) {}
            void DrawIndexed(ngfx::command_list_t* commandList, u32 index_count, u32 instance_count = 1, u32 index_offset = 0) {}
            void Dispatch(ngfx::command_list_t* commandList, u32 group_count_x, u32 group_count_y, u32 group_count_z) {}
            void DispatchMesh(ngfx::command_list_t* commandList, u32 group_count_x, u32 group_count_y, u32 group_count_z) {}

            void DrawIndirect(ngfx::command_list_t* commandList, ngfx::buffer_t* buffer, u32 offset) {}
            void DrawIndexedIndirect(ngfx::command_list_t* commandList, ngfx::buffer_t* buffer, u32 offset) {}
            void DispatchIndirect(ngfx::command_list_t* commandList, ngfx::buffer_t* buffer, u32 offset) {}
            void DispatchMeshIndirect(ngfx::command_list_t* commandList, ngfx::buffer_t* buffer, u32 offset) {}

            void MultiDrawIndirect(ngfx::command_list_t* commandList, u32 max_count, ngfx::buffer_t* args_buffer, u32 args_buffer_offset, ngfx::buffer_t* count_buffer, u32 count_buffer_offset) {}
            void MultiDrawIndexedIndirect(ngfx::command_list_t* commandList, u32 max_count, ngfx::buffer_t* args_buffer, u32 args_buffer_offset, ngfx::buffer_t* count_buffer, u32 count_buffer_offset) {}
            void MultiDispatchIndirect(ngfx::command_list_t* commandList, u32 max_count, ngfx::buffer_t* args_buffer, u32 args_buffer_offset, ngfx::buffer_t* count_buffer, u32 count_buffer_offset) {}
            void MultiDispatchMeshIndirect(ngfx::command_list_t* commandList, u32 max_count, ngfx::buffer_t* args_buffer, u32 args_buffer_offset, ngfx::buffer_t* count_buffer, u32 count_buffer_offset) {}

            void BuildRayTracingBLAS(ngfx::command_list_t* commandList, ngfx::blas_t* blas) {}
            void UpdateRayTracingBLAS(ngfx::command_list_t* commandList, ngfx::blas_t* blas, ngfx::buffer_t* vertex_buffer, u32 vertex_buffer_offset) {}
            void BuildRayTracingTLAS(ngfx::command_list_t* commandList, ngfx::tlas_t* tlas, const rt_instance_t* instances, u32 instance_count) {}
        }  // namespace nd3d12
#endif
    }  // namespace ngfx
}  // namespace ncore

#endif
