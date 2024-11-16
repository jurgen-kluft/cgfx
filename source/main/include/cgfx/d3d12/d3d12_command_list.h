#ifndef __CGFX_D3D12_COMMAND_LIST_H__
#define __CGFX_D3D12_COMMAND_LIST_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
    #pragma once
#endif

// #include "cgfx/d3d12/d3d12_header.h"
#include "cgfx/gfx_command_list.h"

namespace ncore
{
    namespace ngfx
    {
        struct device_t;

        namespace nd3d12
        {
            ngfx::command_list_t* CreateCommandList(ngfx::command_list_t* cl);
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
            void ClearUAV(ngfx::command_list_t* commandList, resource_t* resource, descriptor_t* uav, const float* clear_value);
            void ClearUAV(ngfx::command_list_t* commandList, resource_t* resource, descriptor_t* uav, const u32* clear_value);
            void WriteBuffer(ngfx::command_list_t* commandList, buffer_t* buffer, u32 offset, u32 data);
            void UpdateTileMappings(ngfx::command_list_t* commandList, texture_t* texture, heap_t* heap, u32 mapping_count, const tile_mapping_t* mappings);

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

        }  // namespace nd3d12

        //         class D3D12Device;

        //         class D3D12CommandList : public command_list_t
        //         {
        //         public:
        //             D3D12CommandList(D3D12Device* pDevice, GfxCommandQueue queue_type, const char* name);
        //             ~D3D12CommandList();

        //             bool Create();

        //             virtual void* GetHandle() const override { return m_pCommandList; }

        //             virtual void ResetAllocator() override;
        //             virtual void Begin() override;
        //             virtual void End() override;
        //             virtual void Wait(fence_t* fence, u64 value) override;
        //             virtual void Signal(fence_t* fence, u64 value) override;
        //             virtual void Present(swapchain_t* swapchain) override;
        //             virtual void Submit() override;
        //             virtual void ResetState() override;

        //             virtual void BeginProfiling() override;
        //             virtual void EndProfiling() override;
        //             virtual void BeginEvent(const char* event_name) override;
        //             virtual void EndEvent() override;

        //             virtual void CopyBufferToTexture(texture_t* texture, u32 mip_level, u32 array_slice, buffer_t* buffer, u32 offset) override;
        //             virtual void CopyTextureToBuffer(buffer_t* dst_buffer, u32 offset, texture_t* src_texture, u32 mip_level, u32 array_slice) override;
        //             virtual void CopyBuffer(buffer_t* dst, u32 dst_offset, buffer_t* src, u32 src_offset, u32 size) override;
        //             virtual void CopyTexture(texture_t* dst, u32 dst_mip, u32 dst_array, texture_t* src, u32 src_mip, u32 src_array) override;
        //             virtual void ClearUAV(resource_t* resource, descriptor_t* uav, const float* clear_value, IGfxClearUavApi* clear_api) override;
        //             virtual void ClearUAV(resource_t* resource, descriptor_t* uav, const u32* clear_value, IGfxClearUavApi* clear_api) override;
        //             virtual void WriteBuffer(buffer_t* buffer, u32 offset, u32 data) override;
        //             virtual void UpdateTileMappings(texture_t* texture, heap_t* heap, u32 mapping_count, const tile_mapping_t* mappings) override;

        //             virtual void TextureBarrier(texture_t* texture, u32 sub_resource, GfxAccessFlags access_before, GfxAccessFlags access_after) override;
        //             virtual void BufferBarrier(buffer_t* buffer, GfxAccessFlags access_before, GfxAccessFlags access_after) override;
        //             virtual void GlobalBarrier(GfxAccessFlags access_before, GfxAccessFlags access_after) override;
        //             virtual void FlushBarriers() override;

        //             virtual void BeginRenderPass(const renderpass_desc_t& render_pass) override;
        //             virtual void EndRenderPass() override;
        //             virtual void SetPipelineState(pipeline_state_t* state) override;
        //             virtual void SetStencilReference(u8 stencil) override;
        //             virtual void SetBlendFactor(const float* blend_factor) override;
        //             virtual void SetIndexBuffer(buffer_t* buffer, u32 offset, GfxFormat format) override;
        //             virtual void SetViewport(u32 x, u32 y, u32 width, u32 height) override;
        //             virtual void SetScissorRect(u32 x, u32 y, u32 width, u32 height) override;
        //             virtual void SetGraphicsConstants(u32 slot, const void* data, s64 data_size) override;
        //             virtual void SetComputeConstants(u32 slot, const void* data, s64 data_size) override;

        //             virtual void Draw(u32 vertex_count, u32 instance_count = 1) override;
        //             virtual void DrawIndexed(u32 index_count, u32 instance_count = 1, u32 index_offset = 0) override;
        //             virtual void Dispatch(u32 group_count_x, u32 group_count_y, u32 group_count_z) override;
        //             virtual void DispatchMesh(u32 group_count_x, u32 group_count_y, u32 group_count_z) override;

        //             virtual void DrawIndirect(buffer_t* buffer, u32 offset) override;
        //             virtual void DrawIndexedIndirect(buffer_t* buffer, u32 offset) override;
        //             virtual void DispatchIndirect(buffer_t* buffer, u32 offset) override;
        //             virtual void DispatchMeshIndirect(buffer_t* buffer, u32 offset) override;

        //             virtual void MultiDrawIndirect(u32 max_count, buffer_t* args_buffer, u32 args_buffer_offset, buffer_t* count_buffer, u32 count_buffer_offset) override;
        //             virtual void MultiDrawIndexedIndirect(u32 max_count, buffer_t* args_buffer, u32 args_buffer_offset, buffer_t* count_buffer, u32 count_buffer_offset) override;
        //             virtual void MultiDispatchIndirect(u32 max_count, buffer_t* args_buffer, u32 args_buffer_offset, buffer_t* count_buffer, u32 count_buffer_offset) override;
        //             virtual void MultiDispatchMeshIndirect(u32 max_count, buffer_t* args_buffer, u32 args_buffer_offset, buffer_t* count_buffer, u32 count_buffer_offset) override;

        //             virtual void BuildRayTracingBLAS(blas_t* blas) override;
        //             virtual void UpdateRayTracingBLAS(blas_t* blas, buffer_t* vertex_buffer, u32 vertex_buffer_offset) override;
        //             virtual void BuildRayTracingTLAS(tlas_t* tlas, const GfxRayTracing::Instance* instances, u32 instance_count) override;

        // #if MICROPROFILE_GPU_TIMERS
        //             virtual struct MicroProfileThreadLogGpu* GetProfileLog() const override;
        // #endif

        //         private:
        //             ID3D12CommandQueue*         m_pCommandQueue     = nullptr;
        //             ID3D12CommandAllocator*     m_pCommandAllocator = nullptr;
        //             ID3D12GraphicsCommandList7* m_pCommandList      = nullptr;

        //             u32                m_commandCount = 0;
        //             pipeline_state_t* m_pCurrentPSO  = nullptr;

        //             template <typename T>
        //             struct vector_t
        //             {
        //                 T*  data     = nullptr;
        //                 u32 size     = 0;
        //                 u32 capacity = 0;
        //             };

        //             vector_t<D3D12_TEXTURE_BARRIER> m_textureBarriers;
        //             vector_t<D3D12_BUFFER_BARRIER>  m_bufferBarriers;
        //             vector_t<D3D12_GLOBAL_BARRIER>  m_globalBarriers;

        //             template <typename F, typename S>
        //             struct pair_t
        //             {
        // 				F first;
        // 				S second;
        // 			};

        //             vector_t<pair_t<fence_t*, u64>> m_pendingWaits;
        //             vector_t<pair_t<fence_t*, u64>> m_pendingSignals;

        //             vector_t<swapchain_t*> m_pendingSwapchain;

        // #if MICROPROFILE_GPU_TIMERS_D3D12
        //             struct MicroProfileThreadLogGpu* m_pProfileLog   = nullptr;
        //             int                              m_nProfileQueue = -1;
        // #endif
        //         };

    }  // namespace ngfx
}  // namespace ncore
#endif
