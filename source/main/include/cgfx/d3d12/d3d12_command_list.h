#ifndef __CGFX_D3D12_COMMAND_LIST_H__
#define __CGFX_D3D12_COMMAND_LIST_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
    #pragma once
#endif

#include "cgfx/d3d12/d3d12_header.h"
#include "cgfx/gfx_command_list.h"

namespace ncore
{
    namespace ngfx
    {
        class D3D12Device;

        class D3D12CommandList : public IGfxCommandList
        {
        public:
            D3D12CommandList(D3D12Device* pDevice, GfxCommandQueue queue_type, const char* name);
            ~D3D12CommandList();

            bool Create();

            virtual void* GetHandle() const override { return m_pCommandList; }

            virtual void ResetAllocator() override;
            virtual void Begin() override;
            virtual void End() override;
            virtual void Wait(IGfxFence* fence, u64 value) override;
            virtual void Signal(IGfxFence* fence, u64 value) override;
            virtual void Present(IGfxSwapchain* swapchain) override;
            virtual void Submit() override;
            virtual void ResetState() override;

            virtual void BeginProfiling() override;
            virtual void EndProfiling() override;
            virtual void BeginEvent(const char* event_name) override;
            virtual void EndEvent() override;

            virtual void CopyBufferToTexture(IGfxTexture* texture, u32 mip_level, u32 array_slice, IGfxBuffer* buffer, u32 offset) override;
            virtual void CopyTextureToBuffer(IGfxBuffer* dst_buffer, u32 offset, IGfxTexture* src_texture, u32 mip_level, u32 array_slice) override;
            virtual void CopyBuffer(IGfxBuffer* dst, u32 dst_offset, IGfxBuffer* src, u32 src_offset, u32 size) override;
            virtual void CopyTexture(IGfxTexture* dst, u32 dst_mip, u32 dst_array, IGfxTexture* src, u32 src_mip, u32 src_array) override;
            virtual void ClearUAV(IGfxResource* resource, IGfxDescriptor* uav, const float* clear_value, IGfxClearUavApi* clear_api) override;
            virtual void ClearUAV(IGfxResource* resource, IGfxDescriptor* uav, const u32* clear_value, IGfxClearUavApi* clear_api) override;
            virtual void WriteBuffer(IGfxBuffer* buffer, u32 offset, u32 data) override;
            virtual void UpdateTileMappings(IGfxTexture* texture, IGfxHeap* heap, u32 mapping_count, const GfxTileMapping* mappings) override;

            virtual void TextureBarrier(IGfxTexture* texture, u32 sub_resource, GfxAccessFlags access_before, GfxAccessFlags access_after) override;
            virtual void BufferBarrier(IGfxBuffer* buffer, GfxAccessFlags access_before, GfxAccessFlags access_after) override;
            virtual void GlobalBarrier(GfxAccessFlags access_before, GfxAccessFlags access_after) override;
            virtual void FlushBarriers() override;

            virtual void BeginRenderPass(const GfxRenderPassDesc& render_pass) override;
            virtual void EndRenderPass() override;
            virtual void SetPipelineState(IGfxPipelineState* state) override;
            virtual void SetStencilReference(u8 stencil) override;
            virtual void SetBlendFactor(const float* blend_factor) override;
            virtual void SetIndexBuffer(IGfxBuffer* buffer, u32 offset, GfxFormat format) override;
            virtual void SetViewport(u32 x, u32 y, u32 width, u32 height) override;
            virtual void SetScissorRect(u32 x, u32 y, u32 width, u32 height) override;
            virtual void SetGraphicsConstants(u32 slot, const void* data, s64 data_size) override;
            virtual void SetComputeConstants(u32 slot, const void* data, s64 data_size) override;

            virtual void Draw(u32 vertex_count, u32 instance_count = 1) override;
            virtual void DrawIndexed(u32 index_count, u32 instance_count = 1, u32 index_offset = 0) override;
            virtual void Dispatch(u32 group_count_x, u32 group_count_y, u32 group_count_z) override;
            virtual void DispatchMesh(u32 group_count_x, u32 group_count_y, u32 group_count_z) override;

            virtual void DrawIndirect(IGfxBuffer* buffer, u32 offset) override;
            virtual void DrawIndexedIndirect(IGfxBuffer* buffer, u32 offset) override;
            virtual void DispatchIndirect(IGfxBuffer* buffer, u32 offset) override;
            virtual void DispatchMeshIndirect(IGfxBuffer* buffer, u32 offset) override;

            virtual void MultiDrawIndirect(u32 max_count, IGfxBuffer* args_buffer, u32 args_buffer_offset, IGfxBuffer* count_buffer, u32 count_buffer_offset) override;
            virtual void MultiDrawIndexedIndirect(u32 max_count, IGfxBuffer* args_buffer, u32 args_buffer_offset, IGfxBuffer* count_buffer, u32 count_buffer_offset) override;
            virtual void MultiDispatchIndirect(u32 max_count, IGfxBuffer* args_buffer, u32 args_buffer_offset, IGfxBuffer* count_buffer, u32 count_buffer_offset) override;
            virtual void MultiDispatchMeshIndirect(u32 max_count, IGfxBuffer* args_buffer, u32 args_buffer_offset, IGfxBuffer* count_buffer, u32 count_buffer_offset) override;

            virtual void BuildRayTracingBLAS(IGfxRayTracingBLAS* blas) override;
            virtual void UpdateRayTracingBLAS(IGfxRayTracingBLAS* blas, IGfxBuffer* vertex_buffer, u32 vertex_buffer_offset) override;
            virtual void BuildRayTracingTLAS(IGfxRayTracingTLAS* tlas, const GfxRayTracing::Instance* instances, u32 instance_count) override;

#if MICROPROFILE_GPU_TIMERS
            virtual struct MicroProfileThreadLogGpu* GetProfileLog() const override;
#endif

        private:
            ID3D12CommandQueue*         m_pCommandQueue     = nullptr;
            ID3D12CommandAllocator*     m_pCommandAllocator = nullptr;
            ID3D12GraphicsCommandList7* m_pCommandList      = nullptr;

            u32                m_commandCount = 0;
            IGfxPipelineState* m_pCurrentPSO  = nullptr;

            template <typename T>
            struct vector_t
            {
                T*  data     = nullptr;
                u32 size     = 0;
                u32 capacity = 0;
            };

            vector_t<D3D12_TEXTURE_BARRIER> m_textureBarriers;
            vector_t<D3D12_BUFFER_BARRIER>  m_bufferBarriers;
            vector_t<D3D12_GLOBAL_BARRIER>  m_globalBarriers;

            template <typename F, typename S>
            struct pair_t
            {
				F first;
				S second;
			};

            vector_t<pair_t<IGfxFence*, u64>> m_pendingWaits;
            vector_t<pair_t<IGfxFence*, u64>> m_pendingSignals;

            vector_t<IGfxSwapchain*> m_pendingSwapchain;

#if MICROPROFILE_GPU_TIMERS_D3D12
            struct MicroProfileThreadLogGpu* m_pProfileLog   = nullptr;
            int                              m_nProfileQueue = -1;
#endif
        };

    }  // namespace ngfx
}  // namespace ncore
#endif
