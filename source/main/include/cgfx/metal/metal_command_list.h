#ifndef __CGFX_METAL_COMMAND_LIST_H__
#define __CGFX_METAL_COMMAND_LIST_H__

#include "cgfx/metal/metal_utils.h"
#include "cgfx/gfx_command_list.h"

namespace ncore
{
    namespace ngfx
    {
        class MetalDevice;

        class MetalCommandList : public IGfxCommandList
        {
        public:
            MetalCommandList(MetalDevice* pDevice, GfxCommandQueue queue_type, const char* name);
            ~MetalCommandList();

            bool Create();

            virtual void* GetHandle() const override { return m_pCommandBuffer; }

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

            virtual void CopyBufferToTexture(IGfxTexture* dst_texture, u32 mip_level, u32 array_slice, IGfxBuffer* src_buffer, u32 offset) override;
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
            virtual void BuildRayTracingTLAS(IGfxRayTracingTLAS* tlas, const GfxRayTracingInstance* instances, u32 instance_count) override;

#if MICROPROFILE_GPU_TIMERS
            virtual struct MicroProfileThreadLogGpu* GetProfileLog() const override;
#endif

        private:
            void BeginBlitEncoder();
            void EndBlitEncoder();

            void BeginComputeEncoder();
            void EndComputeEncoder();

            void BeginASEncoder();
            void EndASEncoder();

            void SetRasterizerState(const GfxRasterizerState& state);

        private:
            MTL::CommandBuffer* m_pCommandBuffer = nullptr;

            MTL::BlitCommandEncoder*                  m_pBlitCommandEncoder    = nullptr;
            MTL::RenderCommandEncoder*                m_pRenderCommandEncoder  = nullptr;
            MTL::ComputeCommandEncoder*               m_pComputeCommandEncoder = nullptr;
            MTL::AccelerationStructureCommandEncoder* m_pASEncoder             = nullptr;
            MTL::Fence*                               m_pFence                 = nullptr;

            IGfxPipelineState* m_pCurrentPSO = nullptr;

            MTL::Buffer*          m_pIndexBuffer      = nullptr;
            NS::UInteger          m_indexBufferOffset = 0;
            MTL::IndexType        m_indexType         = MTL::IndexTypeUInt16;
            MTL::PrimitiveType    m_primitiveType     = MTL::PrimitiveTypeTriangle;
            MTL::CullMode         m_cullMode          = MTL::CullModeNone;
            MTL::Winding          m_frontFaceWinding  = MTL::WindingClockwise;
            MTL::TriangleFillMode m_fillMode          = MTL::TriangleFillModeFill;
            MTL::DepthClipMode    m_clipMode          = MTL::DepthClipModeClip;
            float                 m_depthBias         = 0.0f;
            float                 m_depthBiasClamp    = 0.0f;
            float                 m_depthSlopeScale   = 0.0f;

            struct TopLevelArgumentBuffer
            {
                u32 cbv0[GFX_MAX_ROOT_CONSTANTS];  // root constants
                u64 cbv1;                          // gpu address
                u64 cbv2;
            };

            TopLevelArgumentBuffer m_graphicsArgumentBuffer;
            TopLevelArgumentBuffer m_computeArgumentBuffer;
        };

    }  // namespace ngfx
}  // namespace ncore

#endif
