#include "cgfx/mock/mock_command_list.h"
#include "cgfx/mock/mock_device.h"
#include "cgfx/mock/mock_swapchain.h"

namespace ncore
{
    namespace ngfx
    {

        MockCommandList::MockCommandList(MockDevice* pDevice, GfxCommandQueue queue_type, const char* name)
        {
            m_pDevice   = pDevice;
            m_name      = name;
            m_queueType = queue_type;
        }

        MockCommandList::~MockCommandList() {}

        bool MockCommandList::Create() { return true; }

        void* MockCommandList::GetHandle() const { return nullptr; }

        void MockCommandList::ResetAllocator() {}

        void MockCommandList::Begin() {}

        void MockCommandList::End() {}

        void MockCommandList::Wait(IGfxFence* fence, u64 value) {}

        void MockCommandList::Signal(IGfxFence* fence, u64 value) {}

        void MockCommandList::Present(IGfxSwapchain* swapchain) { ((MockSwapchain*)swapchain)->Present(); }

        void MockCommandList::Submit() {}

        void MockCommandList::ResetState() {}

        void MockCommandList::BeginProfiling() {}

        void MockCommandList::EndProfiling() {}

        void MockCommandList::BeginEvent(const char* event_name) {}

        void MockCommandList::EndEvent() {}

        void MockCommandList::CopyBufferToTexture(IGfxTexture* dst_texture, u32 mip_level, u32 array_slice, IGfxBuffer* src_buffer, u32 offset) {}

        void MockCommandList::CopyTextureToBuffer(IGfxBuffer* dst_buffer, u32 offset, IGfxTexture* src_texture, u32 mip_level, u32 array_slice) {}

        void MockCommandList::CopyBuffer(IGfxBuffer* dst, u32 dst_offset, IGfxBuffer* src, u32 src_offset, u32 size) {}

        void MockCommandList::CopyTexture(IGfxTexture* dst, u32 dst_mip, u32 dst_array, IGfxTexture* src, u32 src_mip, u32 src_array) {}

        void MockCommandList::ClearUAV(IGfxResource* resource, IGfxDescriptor* uav, const float* clear_value, IGfxClearUavApi* clear_api)
        {
            GfxUnorderedAccessViewDesc desc;
            clear_api->ClearUAV(this, resource, uav, desc, clear_value);
        }

        void MockCommandList::ClearUAV(IGfxResource* resource, IGfxDescriptor* uav, const u32* clear_value, IGfxClearUavApi* clear_api)
        {
            GfxUnorderedAccessViewDesc desc;
            clear_api->ClearUAV(this, resource, uav, desc, clear_value);
        }

        void MockCommandList::WriteBuffer(IGfxBuffer* buffer, u32 offset, u32 data) {}

        void MockCommandList::UpdateTileMappings(IGfxTexture* texture, IGfxHeap* heap, u32 mapping_count, const GfxTileMapping* mappings) {}

        void MockCommandList::TextureBarrier(IGfxTexture* texture, u32 sub_resource, GfxAccessFlags access_before, GfxAccessFlags access_after) {}

        void MockCommandList::BufferBarrier(IGfxBuffer* buffer, GfxAccessFlags access_before, GfxAccessFlags access_after) {}

        void MockCommandList::GlobalBarrier(GfxAccessFlags access_before, GfxAccessFlags access_after) {}

        void MockCommandList::FlushBarriers() {}

        void MockCommandList::BeginRenderPass(const GfxRenderPassDesc& render_pass) {}

        void MockCommandList::EndRenderPass() {}

        void MockCommandList::SetPipelineState(IGfxPipelineState* state) {}

        void MockCommandList::SetStencilReference(u8 stencil) {}

        void MockCommandList::SetBlendFactor(const float* blend_factor) {}

        void MockCommandList::SetIndexBuffer(IGfxBuffer* buffer, u32 offset, GfxFormat format) {}

        void MockCommandList::SetViewport(u32 x, u32 y, u32 width, u32 height) {}

        void MockCommandList::SetScissorRect(u32 x, u32 y, u32 width, u32 height) {}

        void MockCommandList::SetGraphicsConstants(u32 slot, const void* data, s64 data_size) {}

        void MockCommandList::SetComputeConstants(u32 slot, const void* data, s64 data_size) {}

        void MockCommandList::Draw(u32 vertex_count, u32 instance_count) {}

        void MockCommandList::DrawIndexed(u32 index_count, u32 instance_count, u32 index_offset) {}

        void MockCommandList::Dispatch(u32 group_count_x, u32 group_count_y, u32 group_count_z) {}

        void MockCommandList::DispatchMesh(u32 group_count_x, u32 group_count_y, u32 group_count_z) {}

        void MockCommandList::DrawIndirect(IGfxBuffer* buffer, u32 offset) {}

        void MockCommandList::DrawIndexedIndirect(IGfxBuffer* buffer, u32 offset) {}

        void MockCommandList::DispatchIndirect(IGfxBuffer* buffer, u32 offset) {}

        void MockCommandList::DispatchMeshIndirect(IGfxBuffer* buffer, u32 offset) {}

        void MockCommandList::MultiDrawIndirect(u32 max_count, IGfxBuffer* args_buffer, u32 args_buffer_offset, IGfxBuffer* count_buffer, u32 count_buffer_offset) {}

        void MockCommandList::MultiDrawIndexedIndirect(u32 max_count, IGfxBuffer* args_buffer, u32 args_buffer_offset, IGfxBuffer* count_buffer, u32 count_buffer_offset) {}

        void MockCommandList::MultiDispatchIndirect(u32 max_count, IGfxBuffer* args_buffer, u32 args_buffer_offset, IGfxBuffer* count_buffer, u32 count_buffer_offset) {}

        void MockCommandList::MultiDispatchMeshIndirect(u32 max_count, IGfxBuffer* args_buffer, u32 args_buffer_offset, IGfxBuffer* count_buffer, u32 count_buffer_offset) {}

        void MockCommandList::BuildRayTracingBLAS(IGfxRayTracingBLAS* blas) {}

        void MockCommandList::UpdateRayTracingBLAS(IGfxRayTracingBLAS* blas, IGfxBuffer* vertex_buffer, u32 vertex_buffer_offset) {}

        void MockCommandList::BuildRayTracingTLAS(IGfxRayTracingTLAS* tlas, const GfxRayTracing::Instance* instances, u32 instance_count) {}

#if MICROPROFILE_GPU_TIMERS
        MicroProfileThreadLogGpu* MockCommandList::GetProfileLog() const { return nullptr; }
#endif

    }  // namespace ngfx
}  // namespace ncore
