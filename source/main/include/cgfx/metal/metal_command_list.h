#ifndef __CGFX_METAL_COMMAND_LIST_H__
#define __CGFX_METAL_COMMAND_LIST_H__

#include "cgfx/metal/metal_utils.h"
#include "cgfx/gfx_command_list.h"

namespace ncore
{
    namespace ngfx
    {
        struct device_t;

#ifdef TARGET_MAC
        namespace nmetal
        {
            struct top_level_argument_buffer_t
            {
                u32      cbv0[GFX_MAX_ROOT_CONSTANTS];  // root constants
                uint64_t cbv1;                          // gpu address
                uint64_t cbv2;
            };

            struct command_list_t
            {
                D_GFX_OCS_COMPONENT_SET(enums::ComponentMetalCommandList);

                MTL::CommandBuffer*                       m_pCommandBuffer         = nullptr;
                MTL::BlitCommandEncoder*                  m_pBlitCommandEncoder    = nullptr;
                MTL::RenderCommandEncoder*                m_pRenderCommandEncoder  = nullptr;
                MTL::ComputeCommandEncoder*               m_pComputeCommandEncoder = nullptr;
                MTL::AccelerationStructureCommandEncoder* m_pASEncoder             = nullptr;
                MTL::Fence*                               m_pFence                 = nullptr;
                pipeline_state_t*                         m_pCurrentPSO            = nullptr;
                MTL::Buffer*                              m_pIndexBuffer           = nullptr;
                NS::UInteger                              m_indexBufferOffset      = 0;
                MTL::IndexType                            m_indexType              = MTL::IndexTypeUInt16;
                MTL::PrimitiveType                        m_primitiveType          = MTL::PrimitiveTypeTriangle;
                MTL::CullMode                             m_cullMode               = MTL::CullModeNone;
                MTL::Winding                              m_frontFaceWinding       = MTL::WindingClockwise;
                MTL::TriangleFillMode                     m_fillMode               = MTL::TriangleFillModeFill;
                MTL::DepthClipMode                        m_clipMode               = MTL::DepthClipModeClip;
                float                                     m_depthBias              = 0.0f;
                float                                     m_depthBiasClamp         = 0.0f;
                float                                     m_depthSlopeScale        = 0.0f;
                top_level_argument_buffer_t               m_graphicsArgumentBuffer;
                top_level_argument_buffer_t               m_computeArgumentBuffer;

                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            ngfx::command_list_t* CreateCommandList(ngfx::command_list_t*);

            bool                Create(ngfx::command_list_t*);
            void                Destroy(ngfx::command_list_t*);
            MTL::CommandBuffer* GetHandle(ngfx::command_list_t*);

            void ResetAllocator(ngfx::command_list_t* cmdList);
            void Begin(ngfx::command_list_t* cmdList);
            void End(ngfx::command_list_t* cmdList);
            void Wait(ngfx::command_list_t* cmdList, ngfx::fence_t* fence, u64 value);
            void Signal(ngfx::command_list_t* cmdList, ngfx::fence_t* fence, u64 value);
            void Present(ngfx::command_list_t* cmdList, ngfx::swapchain_t* swapchain);
            void Submit(ngfx::command_list_t* cmdList);
            void ResetState(ngfx::command_list_t* cmdList);

            void BeginProfiling(ngfx::command_list_t* cmdList);
            void EndProfiling(ngfx::command_list_t* cmdList);
            void BeginEvent(ngfx::command_list_t* cmdList, const char* event_name);
            void EndEvent(ngfx::command_list_t* cmdList);

            void CopyBufferToTexture(ngfx::command_list_t* cmdList, texture_t* dst_texture, u32 mip_level, u32 array_slice, buffer_t* src_buffer, u32 offset);
            void CopyTextureToBuffer(ngfx::command_list_t* cmdList, buffer_t* dst_buffer, u32 offset, texture_t* src_texture, u32 mip_level, u32 array_slice);
            void CopyBuffer(ngfx::command_list_t* cmdList, buffer_t* dst, u32 dst_offset, buffer_t* src, u32 src_offset, u32 size);
            void CopyTexture(ngfx::command_list_t* cmdList, texture_t* dst, u32 dst_mip, u32 dst_array, texture_t* src, u32 src_mip, u32 src_array);

            // TODO: Should we pass a ccore callback_t so that this level can invoke a function on a higher level?
            void ClearUAV(ngfx::command_list_t* cmdList, resource_t* resource, descriptor_t* uav, const float* clear_value);
            void ClearUAV(ngfx::command_list_t* cmdList, resource_t* resource, descriptor_t* uav, const u32* clear_value);

            void WriteBuffer(ngfx::command_list_t* cmdList, buffer_t* buffer, u32 offset, u32 data);
            void UpdateTileMappings(ngfx::command_list_t* cmdList, texture_t* texture, heap_t* heap, u32 mapping_count, const tile_mapping_t* mappings);

            void TextureBarrier(ngfx::command_list_t* cmdList, texture_t* texture, u32 sub_resource, enums::access_flags access_before, enums::access_flags access_after);
            void BufferBarrier(ngfx::command_list_t* cmdList, buffer_t* buffer, enums::access_flags access_before, enums::access_flags access_after);
            void GlobalBarrier(ngfx::command_list_t* cmdList, enums::access_flags access_before, enums::access_flags access_after);
            void FlushBarriers(ngfx::command_list_t* cmdList);

            void BeginRenderPass(ngfx::command_list_t* cmdList, const renderpass_desc_t& render_pass);
            void EndRenderPass(ngfx::command_list_t* cmdList);
            void SetPipelineState(ngfx::command_list_t* cmdList, pipeline_state_t* state);
            void SetStencilReference(ngfx::command_list_t* cmdList, u8 stencil);
            void SetBlendFactor(ngfx::command_list_t* cmdList, const float* blend_factor);
            void SetIndexBuffer(ngfx::command_list_t* cmdList, buffer_t* buffer, u32 offset, enums::format format);
            void SetViewport(ngfx::command_list_t* cmdList, u32 x, u32 y, u32 width, u32 height);
            void SetScissorRect(ngfx::command_list_t* cmdList, u32 x, u32 y, u32 width, u32 height);
            void SetGraphicsConstants(ngfx::command_list_t* cmdList, u32 slot, const void* data, s64 data_size);
            void SetComputeConstants(ngfx::command_list_t* cmdList, u32 slot, const void* data, s64 data_size);

            void Draw(ngfx::command_list_t* cmdList, u32 vertex_count, u32 instance_count = 1);
            void DrawIndexed(ngfx::command_list_t* cmdList, u32 index_count, u32 instance_count = 1, u32 index_offset = 0);
            void Dispatch(ngfx::command_list_t* cmdList, u32 group_count_x, u32 group_count_y, u32 group_count_z);
            void DispatchMesh(ngfx::command_list_t* cmdList, u32 group_count_x, u32 group_count_y, u32 group_count_z);

            void DrawIndirect(ngfx::command_list_t* cmdList, buffer_t* buffer, u32 offset);
            void DrawIndexedIndirect(ngfx::command_list_t* cmdList, buffer_t* buffer, u32 offset);
            void DispatchIndirect(ngfx::command_list_t* cmdList, buffer_t* buffer, u32 offset);
            void DispatchMeshIndirect(ngfx::command_list_t* cmdList, buffer_t* buffer, u32 offset);

            void MultiDrawIndirect(ngfx::command_list_t* cmdList, u32 max_count, buffer_t* args_buffer, u32 args_buffer_offset, buffer_t* count_buffer, u32 count_buffer_offset);
            void MultiDrawIndexedIndirect(ngfx::command_list_t* cmdList, u32 max_count, buffer_t* args_buffer, u32 args_buffer_offset, buffer_t* count_buffer, u32 count_buffer_offset);
            void MultiDispatchIndirect(ngfx::command_list_t* cmdList, u32 max_count, buffer_t* args_buffer, u32 args_buffer_offset, buffer_t* count_buffer, u32 count_buffer_offset);
            void MultiDispatchMeshIndirect(ngfx::command_list_t* cmdList, u32 max_count, buffer_t* args_buffer, u32 args_buffer_offset, buffer_t* count_buffer, u32 count_buffer_offset);

            void BuildRayTracingBLAS(ngfx::command_list_t* cmdList, blas_t* blas);
            void UpdateRayTracingBLAS(ngfx::command_list_t* cmdList, blas_t* blas, buffer_t* vertex_buffer, u32 vertex_buffer_offset);
            void BuildRayTracingTLAS(ngfx::command_list_t* cmdList, tlas_t* tlas, const rt_instance_t* instances, u32 instance_count);
        }  // namespace nmetal
#else
        namespace nmetal
        {
            ngfx::command_list_t* CreateCommandList(ngfx::device_t* device, ngfx::command_list_t* cmdList) { return cmdList; }

            bool  Create(ngfx::command_list_t*) { return false; }
            void  Destroy(ngfx::command_list_t*) {}
            void* GetHandle(ngfx::command_list_t*) { return nullptr; }

            void ResetAllocator(ngfx::command_list_t* cmdList) {}
            void Begin(ngfx::command_list_t* cmdList) {}
            void End(ngfx::command_list_t* cmdList) {}
            void Wait(ngfx::command_list_t* cmdList, fence_t* fence, u64 value) {}
            void Signal(ngfx::command_list_t* cmdList, fence_t* fence, u64 value) {}
            void Present(ngfx::command_list_t* cmdList, swapchain_t* swapchain) {}
            void Submit(ngfx::command_list_t* cmdList) {}
            void ResetState(ngfx::command_list_t* cmdList) {}

            void BeginProfiling(ngfx::command_list_t* cmdList) {}
            void EndProfiling(ngfx::command_list_t* cmdList) {}
            void BeginEvent(ngfx::command_list_t* cmdList, const char* event_name) {}
            void EndEvent(ngfx::command_list_t* cmdList) {}

            void CopyBufferToTexture(ngfx::command_list_t* cmdList, texture_t* dst_texture, u32 mip_level, u32 array_slice, buffer_t* src_buffer, u32 offset) {}
            void CopyTextureToBuffer(ngfx::command_list_t* cmdList, buffer_t* dst_buffer, u32 offset, texture_t* src_texture, u32 mip_level, u32 array_slice) {}
            void CopyBuffer(ngfx::command_list_t* cmdList, buffer_t* dst, u32 dst_offset, buffer_t* src, u32 src_offset, u32 size) {}
            void CopyTexture(ngfx::command_list_t* cmdList, texture_t* dst, u32 dst_mip, u32 dst_array, texture_t* src, u32 src_mip, u32 src_array) {}
            void ClearUAV(ngfx::command_list_t* cmdList, resource_t* resource, descriptor_t* uav, const float* clear_value) {}
            void ClearUAV(ngfx::command_list_t* cmdList, resource_t* resource, descriptor_t* uav, const u32* clear_value) {}
            void WriteBuffer(ngfx::command_list_t* cmdList, buffer_t* buffer, u32 offset, u32 data) {}
            void UpdateTileMappings(ngfx::command_list_t* cmdList, texture_t* texture, heap_t* heap, u32 mapping_count, const tile_mapping_t* mappings) {}

            void TextureBarrier(ngfx::command_list_t* cmdList, texture_t* texture, u32 sub_resource, enums::access_flags access_before, enums::access_flags access_after) {}
            void BufferBarrier(ngfx::command_list_t* cmdList, buffer_t* buffer, enums::access_flags access_before, enums::access_flags access_after) {}
            void GlobalBarrier(ngfx::command_list_t* cmdList, enums::access_flags access_before, enums::access_flags access_after) {}
            void FlushBarriers(ngfx::command_list_t* cmdList) {}

            void BeginRenderPass(ngfx::command_list_t* cmdList, const renderpass_desc_t& render_pass) {}
            void EndRenderPass(ngfx::command_list_t* cmdList) {}
            void SetPipelineState(ngfx::command_list_t* cmdList, pipeline_state_t* state) {}
            void SetStencilReference(ngfx::command_list_t* cmdList, u8 stencil) {}
            void SetBlendFactor(ngfx::command_list_t* cmdList, const float* blend_factor) {}
            void SetIndexBuffer(ngfx::command_list_t* cmdList, buffer_t* buffer, u32 offset, enums::format format) {}
            void SetViewport(ngfx::command_list_t* cmdList, u32 x, u32 y, u32 width, u32 height) {}
            void SetScissorRect(ngfx::command_list_t* cmdList, u32 x, u32 y, u32 width, u32 height) {}
            void SetGraphicsConstants(ngfx::command_list_t* cmdList, u32 slot, const void* data, s64 data_size) {}
            void SetComputeConstants(ngfx::command_list_t* cmdList, u32 slot, const void* data, s64 data_size) {}

            void Draw(ngfx::command_list_t* cmdList, u32 vertex_count, u32 instance_count = 1) {}
            void DrawIndexed(ngfx::command_list_t* cmdList, u32 index_count, u32 instance_count = 1, u32 index_offset = 0) {}
            void Dispatch(ngfx::command_list_t* cmdList, u32 group_count_x, u32 group_count_y, u32 group_count_z) {}
            void DispatchMesh(ngfx::command_list_t* cmdList, u32 group_count_x, u32 group_count_y, u32 group_count_z) {}

            void DrawIndirect(ngfx::command_list_t* cmdList, buffer_t* buffer, u32 offset) {}
            void DrawIndexedIndirect(ngfx::command_list_t* cmdList, buffer_t* buffer, u32 offset) {}
            void DispatchIndirect(ngfx::command_list_t* cmdList, buffer_t* buffer, u32 offset) {}
            void DispatchMeshIndirect(ngfx::command_list_t* cmdList, buffer_t* buffer, u32 offset) {}

            void MultiDrawIndirect(ngfx::command_list_t* cmdList, u32 max_count, buffer_t* args_buffer, u32 args_buffer_offset, buffer_t* count_buffer, u32 count_buffer_offset) {}
            void MultiDrawIndexedIndirect(ngfx::command_list_t* cmdList, u32 max_count, buffer_t* args_buffer, u32 args_buffer_offset, buffer_t* count_buffer, u32 count_buffer_offset) {}
            void MultiDispatchIndirect(ngfx::command_list_t* cmdList, u32 max_count, buffer_t* args_buffer, u32 args_buffer_offset, buffer_t* count_buffer, u32 count_buffer_offset) {}
            void MultiDispatchMeshIndirect(ngfx::command_list_t* cmdList, u32 max_count, buffer_t* args_buffer, u32 args_buffer_offset, buffer_t* count_buffer, u32 count_buffer_offset) {}

            void BuildRayTracingBLAS(ngfx::command_list_t* cmdList, blas_t* blas) {}
            void UpdateRayTracingBLAS(ngfx::command_list_t* cmdList, blas_t* blas, buffer_t* vertex_buffer, u32 vertex_buffer_offset) {}
            void BuildRayTracingTLAS(ngfx::command_list_t* cmdList, tlas_t* tlas, const rt_instance_t* instances, u32 instance_count) {}

        }  // namespace nmetal
#endif
    }  // namespace ngfx
}  // namespace ncore

#endif
