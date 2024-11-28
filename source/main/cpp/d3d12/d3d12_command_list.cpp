#include "cgfx/d3d12/d3d12_command_list.h"
#include "cgfx/d3d12/d3d12_device.h"
#include "cgfx/d3d12/d3d12_fence.h"
#include "cgfx/d3d12/d3d12_texture.h"
#include "cgfx/d3d12/d3d12_buffer.h"
#include "cgfx/d3d12/d3d12_pipeline_state.h"
#include "cgfx/d3d12/d3d12_heap.h"
#include "cgfx/d3d12/d3d12_descriptor.h"
#include "cgfx/d3d12/d3d12_rt_blas.h"
#include "cgfx/d3d12/d3d12_rt_tlas.h"
#include "cgfx/d3d12/d3d12_swapchain.h"
#include "cd3d12/c_pix_runtime.h"
#include "cd3d12/amd/c_ags.h"
#include "cgfx/gfx.h"

#include "cbase/c_debug.h"
#include "ccore/c_math.h"

namespace ncore
{
    namespace ngfx
    {
#ifdef TARGET_PC

        static bool s_nearly_equal(float a, float b, float epsilon = 0.0001f) { return fabs(a - b) < epsilon; }

        //         D3D12CommandList::D3D12CommandList(D3D12Device* pDevice, GfxCommandQueue queue_type, const char* name)
        //         {
        //             m_pDevice   = pDevice;
        //             m_name      = name;
        //             m_queueType = queue_type;
        //         }

        //         D3D12CommandList::~D3D12CommandList()
        //         {
        //             D3D12Device* pDevice = (D3D12Device*)m_pDevice;
        //             pDevice->Delete(m_pCommandAllocator);
        //             pDevice->Delete(m_pCommandList);
        //         }

        //         bool D3D12CommandList::Create()
        //         {
        //             D3D12Device*            pDevice = (D3D12Device*)m_pDevice;
        //             D3D12_COMMAND_LIST_TYPE type;

        //             switch (m_queueType)
        //             {
        //                 case enums::CommandQueueGraphics: type = D3D12_COMMAND_LIST_TYPE_DIRECT; m_pCommandQueue = pDevice->GetGraphicsQueue();
        // #if MICROPROFILE_GPU_TIMERS_D3D12
        //                     m_nProfileQueue = pDevice->GetProfileGraphicsQueue();
        // #endif
        //                     break;
        //                 case enums::CommandQueueCompute: type = D3D12_COMMAND_LIST_TYPE_COMPUTE; m_pCommandQueue = pDevice->GetComputeQueue();
        // #if MICROPROFILE_GPU_TIMERS_D3D12
        //                     m_nProfileQueue = pDevice->GetProfileComputeQueue();
        // #endif
        //                     break;
        //                 case enums::CommandQueueCopy: type = D3D12_COMMAND_LIST_TYPE_COPY; m_pCommandQueue = pDevice->GetCopyQueue();
        // #if MICROPROFILE_GPU_TIMERS_D3D12
        //                     m_nProfileQueue = pDevice->GetProfileCopyQueue();
        // #endif
        //                     break;
        //                 default: break;
        //             }

        //             ID3D12Device* pD3D12Device = (ID3D12Device*)pDevice->GetHandle();
        //             HRESULT       hr           = pD3D12Device->CreateCommandAllocator(type, IID_PPV_ARGS(&m_pCommandAllocator));
        //             if (FAILED(hr))
        //             {
        //                 return false;
        //             }
        //             // string_to_wstring(m_name + " allocator").c_str()
        //             // m_pCommandAllocator->SetName(m_name);

        //             hr = pD3D12Device->CreateCommandList(0, type, m_pCommandAllocator, nullptr, IID_PPV_ARGS(&m_pCommandList));
        //             if (FAILED(hr))
        //             {
        //                 return false;
        //             }

        //             m_pCommandList->Close();

        //             return true;
        //         }

        //         void D3D12CommandList::ResetAllocator() { m_pCommandAllocator->Reset(); }

        //         void D3D12CommandList::Begin()
        //         {
        //             m_pCommandList->Reset(m_pCommandAllocator, nullptr);
        //             //m_pCommandList->SetName(string_to_wstring(m_name).c_str());

        //             ResetState();
        //         }

        //         void D3D12CommandList::End()
        //         {
        //             FlushBarriers();

        //             m_pCommandList->Close();
        //         }

        //         void D3D12CommandList::Wait(fence_t* fence, u64 value) { m_pendingWaits.push_back(fence, value); }
        //         void D3D12CommandList::Signal(fence_t* fence, u64 value) { m_pendingSignals.push_back(fence, value); }
        //         void D3D12CommandList::Present(swapchain_t* swapchain) { m_pendingSwapchain.push_back(swapchain); }

        //         void D3D12CommandList::Submit()
        //         {
        //             for (size_t i = 0; i < m_pendingWaits.size; ++i)
        //             {
        //                 m_pCommandQueue->Wait((ID3D12Fence*)m_pendingWaits[i].first->GetHandle(), m_pendingWaits[i].second);
        //             }
        //             m_pendingWaits.clear();

        //             if (m_commandCount > 0)
        //             {
        //                 ID3D12CommandList* ppCommandLists[] = {m_pCommandList};
        //                 m_pCommandQueue->ExecuteCommandLists(1, ppCommandLists);
        //             }

        //             for (size_t i = 0; i < m_pendingSwapchain.size; ++i)
        //             {
        //                 ((D3D12Swapchain*)m_pendingSwapchain[i])->Present();
        //             }
        //             m_pendingSwapchain.clear();

        //             for (size_t i = 0; i < m_pendingSignals.size; ++i)
        //             {
        //                 m_pCommandQueue->Signal((ID3D12Fence*)m_pendingSignals[i].first->GetHandle(), m_pendingSignals[i].second);
        //             }
        //             m_pendingSignals.clear();
        //         }

        //         void D3D12CommandList::ResetState()
        //         {
        //             m_commandCount = 0;
        //             m_pCurrentPSO  = nullptr;

        //             if (m_queueType == enums::CommandQueueGraphics || m_queueType == enums::CommandQueueCompute)
        //             {
        //                 D3D12Device*          pDevice  = (D3D12Device*)m_pDevice;
        //                 ID3D12DescriptorHeap* heaps[2] = {pDevice->GetResourceDescriptorHeap(), pDevice->GetSamplerDescriptorHeap()};
        //                 m_pCommandList->SetDescriptorHeaps(2, heaps);

        //                 ID3D12RootSignature* pRootSignature = pDevice->GetRootSignature();
        //                 m_pCommandList->SetComputeRootSignature(pRootSignature);

        //                 if (m_queueType == enums::CommandQueueGraphics)
        //                 {
        //                     m_pCommandList->SetGraphicsRootSignature(pRootSignature);
        //                 }
        //             }
        //         }

        //         void D3D12CommandList::BeginProfiling()
        //         {
        // #if MICROPROFILE_GPU_TIMERS_D3D12
        //             if (m_nProfileQueue != -1)
        //             {
        //                 m_pProfileLog = MicroProfileThreadLogGpuAlloc();
        //                 MicroProfileGpuBegin(m_pCommandList, m_pProfileLog);
        //             }
        // #endif
        //         }

        //         void D3D12CommandList::EndProfiling()
        //         {
        // #if MICROPROFILE_GPU_TIMERS_D3D12
        //             if (m_nProfileQueue != -1)
        //             {
        //                 MicroProfileGpuSubmit(m_nProfileQueue, MicroProfileGpuEnd(m_pProfileLog));
        //                 MicroProfileThreadLogGpuFree(m_pProfileLog);
        //             }
        // #endif
        //         }

        //         void D3D12CommandList::BeginEvent(const char* event_name)
        //         {
        //             pix::BeginEvent(m_pCommandList, event_name);
        //             ags::BeginEvent(m_pCommandList, event_name);
        //         }

        //         void D3D12CommandList::EndEvent()
        //         {
        //             pix::EndEvent(m_pCommandList);
        //             ags::EndEvent(m_pCommandList);
        //         }

        //         void D3D12CommandList::CopyBufferToTexture(texture_t* dst_texture, u32 mip_level, u32 array_slice, buffer_t* src_buffer, u32 offset)
        //         {
        //             FlushBarriers();

        //             const texture_desc_t& desc = dst_texture->GetDesc();

        //             u32 min_width  = GetFormatBlockWidth(desc.format);
        //             u32 min_height = GetFormatBlockHeight(desc.format);
        //             u32 w          = math::g_max(desc.width >> mip_level, min_width);
        //             u32 h          = math::g_max(desc.height >> mip_level, min_height);
        //             u32 d          = math::g_max(desc.depth >> mip_level, 1u);

        //             D3D12_TEXTURE_COPY_LOCATION dst = {};
        //             dst.pResource                   = (ID3D12Resource*)dst_texture->GetHandle();
        //             dst.Type                        = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
        //             dst.SubresourceIndex            = CalcSubresource(desc, mip_level, array_slice);

        //             D3D12_TEXTURE_COPY_LOCATION src        = {};
        //             src.pResource                          = (ID3D12Resource*)src_buffer->GetHandle();
        //             src.Type                               = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
        //             src.PlacedFootprint.Offset             = offset;
        //             src.PlacedFootprint.Footprint.Format   = dxgi_format(desc.format);
        //             src.PlacedFootprint.Footprint.Width    = w;
        //             src.PlacedFootprint.Footprint.Height   = h;
        //             src.PlacedFootprint.Footprint.Depth    = d;
        //             src.PlacedFootprint.Footprint.RowPitch = dst_texture->GetRowPitch(mip_level);

        //             m_pCommandList->CopyTextureRegion(&dst, 0, 0, 0, &src, nullptr);
        //             ++m_commandCount;
        //         }

        //         void D3D12CommandList::CopyTextureToBuffer(buffer_t* dst_buffer, u32 offset, texture_t* src_texture, u32 mip_level, u32 array_slice)
        //         {
        //             FlushBarriers();

        //             texture_desc_t desc = src_texture->GetDesc();

        //             u32 min_width  = GetFormatBlockWidth(desc.format);
        //             u32 min_height = GetFormatBlockHeight(desc.format);
        //             u32 w          = math::g_max((u32)(desc.width >> mip_level), min_width);
        //             u32 h          = math::g_max((u32)(desc.height >> mip_level), min_height);
        //             u32 d          = math::g_max((u32)(desc.depth >> mip_level), 1u);

        //             D3D12_TEXTURE_COPY_LOCATION dst        = {};
        //             dst.pResource                          = (ID3D12Resource*)dst_buffer->GetHandle();
        //             dst.Type                               = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
        //             dst.PlacedFootprint.Offset             = offset;
        //             dst.PlacedFootprint.Footprint.Format   = dxgi_format(desc.format);
        //             dst.PlacedFootprint.Footprint.Width    = w;
        //             dst.PlacedFootprint.Footprint.Height   = h;
        //             dst.PlacedFootprint.Footprint.Depth    = d;
        //             dst.PlacedFootprint.Footprint.RowPitch = src_texture->GetRowPitch(mip_level);

        //             D3D12_TEXTURE_COPY_LOCATION src;
        //             src.pResource        = (ID3D12Resource*)src_texture->GetHandle();
        //             src.Type             = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
        //             src.SubresourceIndex = CalcSubresource(desc, mip_level, array_slice);

        //             m_pCommandList->CopyTextureRegion(&dst, 0, 0, 0, &src, nullptr);
        //             ++m_commandCount;
        //         }

        //         void D3D12CommandList::CopyBuffer(buffer_t* dst, u32 dst_offset, buffer_t* src, u32 src_offset, u32 size)
        //         {
        //             FlushBarriers();

        //             m_pCommandList->CopyBufferRegion((ID3D12Resource*)dst->GetHandle(), dst_offset, (ID3D12Resource*)src->GetHandle(), src_offset, size);
        //             ++m_commandCount;
        //         }

        //         void D3D12CommandList::CopyTexture(texture_t* dst, u32 dst_mip, u32 dst_array, texture_t* src, u32 src_mip, u32 src_array)
        //         {
        //             FlushBarriers();

        //             D3D12_TEXTURE_COPY_LOCATION dst_texture;
        //             dst_texture.pResource        = (ID3D12Resource*)dst->GetHandle();
        //             dst_texture.Type             = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
        //             dst_texture.SubresourceIndex = CalcSubresource(dst->GetDesc(), dst_mip, dst_array);

        //             D3D12_TEXTURE_COPY_LOCATION src_texture;
        //             src_texture.pResource        = (ID3D12Resource*)src->GetHandle();
        //             src_texture.Type             = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
        //             src_texture.SubresourceIndex = CalcSubresource(src->GetDesc(), src_mip, src_array);

        //             m_pCommandList->CopyTextureRegion(&dst_texture, 0, 0, 0, &src_texture, nullptr);
        //             ++m_commandCount;
        //         }

        //         void D3D12CommandList::ClearUAV(resource_t* resource, descriptor_t* uav, const float* clear_value, IGfxClearUavApi* clear_api)
        //         {
        //             ASSERT(resource->IsTexture() || resource->IsBuffer());

        //             FlushBarriers();

        //             D3D12Descriptor shaderVisibleDescriptor    = ((D3D12UnorderedAccessView*)uav)->GetShaderVisibleDescriptor();
        //             D3D12Descriptor nonShaderVisibleDescriptor = ((D3D12UnorderedAccessView*)uav)->GetNonShaderVisibleDescriptor();

        //             m_pCommandList->ClearUnorderedAccessViewFloat(shaderVisibleDescriptor.gpu_handle, nonShaderVisibleDescriptor.cpu_handle, (ID3D12Resource*)resource->GetHandle(), clear_value, 0, nullptr);
        //             ++m_commandCount;
        //         }

        //         void D3D12CommandList::ClearUAV(resource_t* resource, descriptor_t* uav, const u32* clear_value, IGfxClearUavApi* clear_api)
        //         {
        //             ASSERT(resource->IsTexture() || resource->IsBuffer());

        //             FlushBarriers();

        //             D3D12Descriptor shaderVisibleDescriptor    = ((D3D12UnorderedAccessView*)uav)->GetShaderVisibleDescriptor();
        //             D3D12Descriptor nonShaderVisibleDescriptor = ((D3D12UnorderedAccessView*)uav)->GetNonShaderVisibleDescriptor();

        //             m_pCommandList->ClearUnorderedAccessViewUint(shaderVisibleDescriptor.gpu_handle, nonShaderVisibleDescriptor.cpu_handle, (ID3D12Resource*)resource->GetHandle(), clear_value, 0, nullptr);
        //             ++m_commandCount;
        //         }

        //         void D3D12CommandList::WriteBuffer(buffer_t* buffer, u32 offset, u32 data)
        //         {
        //             FlushBarriers();

        //             D3D12_WRITEBUFFERIMMEDIATE_PARAMETER parameter;
        //             parameter.Dest  = buffer->GetGpuAddress() + offset;
        //             parameter.Value = data;

        //             m_pCommandList->WriteBufferImmediate(1, &parameter, nullptr);
        //             ++m_commandCount;
        //         }

        //         void D3D12CommandList::UpdateTileMappings(texture_t* texture, heap_t* heap, u32 mapping_count, const tile_mapping_t* mappings)
        //         {
        //             vector_t<D3D12_TILED_RESOURCE_COORDINATE> coordinates;
        //             vector_t<D3D12_TILE_REGION_SIZE>          sizes;
        //             vector_t<D3D12_TILE_RANGE_FLAGS>          flags;
        //             vector_t<UINT>                            heapTileOffsets;
        //             vector_t<UINT>                            tileCounts;

        //             coordinates.reserve(mapping_count);
        //             sizes.reserve(mapping_count);
        //             flags.reserve(mapping_count);
        //             heapTileOffsets.reserve(mapping_count);
        //             tileCounts.reserve(mapping_count);

        //             for (u32 i = 0; i < mapping_count; ++i)
        //             {
        //                 D3D12_TILED_RESOURCE_COORDINATE coordinate;
        //                 coordinate.Subresource = mappings[i].subresource;
        //                 coordinate.X           = mappings[i].x;
        //                 coordinate.Y           = mappings[i].y;
        //                 coordinate.Z           = mappings[i].z;

        //                 if (mappings[i].use_box)
        //                 {
        //                     ASSERT(mappings[i].tile_count == mappings[i].width * mappings[i].height * mappings[i].depth);
        //                 }

        //                 D3D12_TILE_REGION_SIZE size;
        //                 size.UseBox   = mappings[i].use_box;
        //                 size.NumTiles = mappings[i].tile_count;
        //                 size.Width    = mappings[i].width;
        //                 size.Height   = mappings[i].height;
        //                 size.Depth    = mappings[i].depth;

        //                 D3D12_TILE_RANGE_FLAGS flag       = mappings[i].type == GfxTileMappingType::Map ? D3D12_TILE_RANGE_FLAG_NONE : D3D12_TILE_RANGE_FLAG_NULL;
        //                 UINT                   tile_count = size.NumTiles;

        //                 coordinates.push_back(coordinate);
        //                 sizes.push_back(size);
        //                 flags.push_back(flag);
        //                 heapTileOffsets.push_back(mappings[i].heap_offset);
        //                 tileCounts.push_back(tile_count);
        //             }

        //             m_pCommandQueue->UpdateTileMappings((ID3D12Resource*)texture->GetHandle(), mapping_count, coordinates.data(), sizes.data(), ((D3D12Heap*)heap)->GetHeap(), mapping_count, flags.data(), heapTileOffsets.data(), tileCounts.data(),
        //                                                 D3D12_TILE_MAPPING_FLAG_NONE);
        //         }

        //         void D3D12CommandList::TextureBarrier(texture_t* texture, u32 sub_resource, GfxAccessFlags access_before, GfxAccessFlags access_after)
        //         {
        //             D3D12_TEXTURE_BARRIER barrier = {};
        //             barrier.SyncBefore            = d3d12_barrier_sync(access_before);
        //             barrier.SyncAfter             = d3d12_barrier_sync(access_after);
        //             barrier.AccessBefore          = d3d12_barrier_access(access_before);
        //             barrier.AccessAfter           = d3d12_barrier_access(access_after);
        //             barrier.LayoutBefore          = d3d12_barrier_layout(access_before);
        //             barrier.LayoutAfter           = d3d12_barrier_layout(access_after);
        //             barrier.pResource             = (ID3D12Resource*)texture->GetHandle();
        //             barrier.Subresources          = CD3DX12_BARRIER_SUBRESOURCE_RANGE(sub_resource);

        //             if (access_before & GfxAccess::Discard)
        //             {
        //                 barrier.Flags = D3D12_TEXTURE_BARRIER_FLAG_DISCARD;
        //             }

        //             m_textureBarriers.push_back(barrier);
        //         }

        //         void D3D12CommandList::BufferBarrier(buffer_t* buffer, GfxAccessFlags access_before, GfxAccessFlags access_after)
        //         {
        //             D3D12_BUFFER_BARRIER barrier = {};
        //             barrier.SyncBefore           = d3d12_barrier_sync(access_before);
        //             barrier.SyncAfter            = d3d12_barrier_sync(access_after);
        //             barrier.AccessBefore         = d3d12_barrier_access(access_before);
        //             barrier.AccessAfter          = d3d12_barrier_access(access_after);
        //             barrier.pResource            = (ID3D12Resource*)buffer->GetHandle();
        //             barrier.Offset               = 0;
        //             barrier.Size                 = UINT64_MAX;

        //             m_bufferBarriers.push_back(barrier);
        //         }

        //         void D3D12CommandList::GlobalBarrier(GfxAccessFlags access_before, GfxAccessFlags access_after)
        //         {
        //             D3D12_GLOBAL_BARRIER barrier = {};
        //             barrier.SyncBefore           = d3d12_barrier_sync(access_before);
        //             barrier.SyncAfter            = d3d12_barrier_sync(access_after);
        //             barrier.AccessBefore         = d3d12_barrier_access(access_before);
        //             barrier.AccessAfter          = d3d12_barrier_access(access_after);

        //             m_globalBarriers.push_back(barrier);
        //         }

        //         void D3D12CommandList::FlushBarriers()
        //         {
        //             vector_t<D3D12_BARRIER_GROUP> barrierGroup;
        //             barrierGroup.reserve(3);

        //             if (!m_textureBarriers.empty())
        //             {
        //                 barrierGroup.push_back(CD3DX12_BARRIER_GROUP((UINT32)m_textureBarriers.size(), m_textureBarriers.data()));
        //             }

        //             if (!m_bufferBarriers.empty())
        //             {
        //                 barrierGroup.push_back(CD3DX12_BARRIER_GROUP((UINT32)m_bufferBarriers.size(), m_bufferBarriers.data()));
        //             }

        //             if (!m_globalBarriers.empty())
        //             {
        //                 barrierGroup.push_back(CD3DX12_BARRIER_GROUP((UINT32)m_globalBarriers.size(), m_globalBarriers.data()));
        //             }

        //             if (!barrierGroup.empty())
        //             {
        //                 ++m_commandCount;
        //                 m_pCommandList->Barrier((UINT32)barrierGroup.size(), barrierGroup.data());
        //             }

        //             m_textureBarriers.clear();
        //             m_bufferBarriers.clear();
        //             m_globalBarriers.clear();
        //         }

        //         void D3D12CommandList::BeginRenderPass(const renderpass_desc_t& render_pass)
        //         {
        //             FlushBarriers();

        //             D3D12_RENDER_PASS_RENDER_TARGET_DESC rtDesc[8] = {};
        //             D3D12_RENDER_PASS_DEPTH_STENCIL_DESC dsDesc    = {};
        //             u32                                  flags     = D3D12_RENDER_PASS_FLAG_NONE;

        //             u32 width  = 0;
        //             u32 height = 0;

        //             unsigned int rt_count = 0;
        //             for (int i = 0; i < 8; ++i)
        //             {
        //                 if (render_pass.color[i].texture == nullptr)
        //                 {
        //                     continue;
        //                 }

        //                 if (width == 0)
        //                 {
        //                     width = render_pass.color[i].texture->GetDesc().width;
        //                 }

        //                 if (height == 0)
        //                 {
        //                     height = render_pass.color[i].texture->GetDesc().height;
        //                 }

        //                 ASSERT(width == render_pass.color[i].texture->GetDesc().width);
        //                 ASSERT(height == render_pass.color[i].texture->GetDesc().height);

        //                 rtDesc[i].cpuDescriptor                           = ((D3D12Texture*)render_pass.color[i].texture)->GetRTV(render_pass.color[i].mip_slice, render_pass.color[i].array_slice);
        //                 rtDesc[i].BeginningAccess.Type                    = d3d12_render_pass_loadop(render_pass.color[i].load_op);
        //                 rtDesc[i].BeginningAccess.Clear.ClearValue.Format = dxgi_format(render_pass.color[i].texture->GetDesc().format);
        //                 memcpy(rtDesc[i].BeginningAccess.Clear.ClearValue.Color, render_pass.color[i].clear_color, sizeof(float) * 4);
        //                 rtDesc[i].EndingAccess.Type = d3d12_render_pass_storeop(render_pass.color[i].store_op);

        //                 ++rt_count;
        //             }

        //             if (render_pass.depth.texture != nullptr)
        //             {
        //                 if (width == 0)
        //                 {
        //                     width = render_pass.depth.texture->GetDesc().width;
        //                 }

        //                 if (height == 0)
        //                 {
        //                     height = render_pass.depth.texture->GetDesc().height;
        //                 }

        //                 ASSERT(width == render_pass.depth.texture->GetDesc().width);
        //                 ASSERT(height == render_pass.depth.texture->GetDesc().height);

        //                 if (render_pass.depth.read_only)
        //                 {
        //                     dsDesc.cpuDescriptor = ((D3D12Texture*)render_pass.depth.texture)->GetReadOnlyDSV(render_pass.depth.mip_slice, render_pass.depth.array_slice);

        //                     flags |= D3D12_RENDER_PASS_FLAG_BIND_READ_ONLY_DEPTH;
        //                     if (IsStencilFormat(render_pass.depth.texture->GetDesc().format))
        //                     {
        //                         flags |= D3D12_RENDER_PASS_FLAG_BIND_READ_ONLY_STENCIL;
        //                     }
        //                 }
        //                 else
        //                 {
        //                     dsDesc.cpuDescriptor = ((D3D12Texture*)render_pass.depth.texture)->GetDSV(render_pass.depth.mip_slice, render_pass.depth.array_slice);
        //                 }

        //                 dsDesc.DepthBeginningAccess.Type                                  = d3d12_render_pass_loadop(render_pass.depth.load_op);
        //                 dsDesc.DepthBeginningAccess.Clear.ClearValue.Format               = dxgi_format(render_pass.depth.texture->GetDesc().format);
        //                 dsDesc.DepthBeginningAccess.Clear.ClearValue.DepthStencil.Depth   = render_pass.depth.clear_depth;
        //                 dsDesc.DepthBeginningAccess.Clear.ClearValue.DepthStencil.Stencil = render_pass.depth.clear_stencil;
        //                 dsDesc.DepthEndingAccess.Type                                     = d3d12_render_pass_storeop(render_pass.depth.store_op);

        //                 if (IsStencilFormat(render_pass.depth.texture->GetDesc().format))
        //                 {
        //                     dsDesc.StencilBeginningAccess.Type                                  = d3d12_render_pass_loadop(render_pass.depth.stencil_load_op);
        //                     dsDesc.StencilBeginningAccess.Clear.ClearValue.Format               = dxgi_format(render_pass.depth.texture->GetDesc().format);
        //                     dsDesc.StencilBeginningAccess.Clear.ClearValue.DepthStencil.Depth   = render_pass.depth.clear_depth;
        //                     dsDesc.StencilBeginningAccess.Clear.ClearValue.DepthStencil.Stencil = render_pass.depth.clear_stencil;
        //                     dsDesc.StencilEndingAccess.Type                                     = d3d12_render_pass_storeop(render_pass.depth.stencil_store_op);
        //                 }
        //                 else
        //                 {
        //                     dsDesc.StencilBeginningAccess.Type = D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_NO_ACCESS;
        //                     dsDesc.StencilEndingAccess.Type    = D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_NO_ACCESS;
        //                 }
        //             }

        //             m_pCommandList->BeginRenderPass(rt_count, rtDesc, render_pass.depth.texture != nullptr ? &dsDesc : nullptr, (D3D12_RENDER_PASS_FLAGS)flags);

        //             ++m_commandCount;

        //             SetViewport(0, 0, width, height);
        //         }

        //         void D3D12CommandList::EndRenderPass()
        //         {
        //             m_pCommandList->EndRenderPass();

        //             ++m_commandCount;
        //         }

        //         void D3D12CommandList::SetPipelineState(pipeline_state_t* state)
        //         {
        //             if (m_pCurrentPSO != state)
        //             {
        //                 m_pCurrentPSO = state;

        //                 m_pCommandList->SetPipelineState((ID3D12PipelineState*)state->GetHandle());

        //                 if (state->GetType() == GfxPipelineType::Graphics)
        //                 {
        //                     m_pCommandList->IASetPrimitiveTopology(((D3D12GraphicsPipelineState*)state)->GetPrimitiveTopology());
        //                 }
        //             }
        //         }

        //         void D3D12CommandList::SetStencilReference(u8 stencil) { m_pCommandList->OMSetStencilRef(stencil); }

        //         void D3D12CommandList::SetBlendFactor(const float* blend_factor) { m_pCommandList->OMSetBlendFactor(blend_factor); }

        //         void D3D12CommandList::SetIndexBuffer(buffer_t* buffer, u32 offset, GfxFormat format)
        //         {
        //             ASSERT(format == Gfx::R16UI || format == Gfx::R32UI);

        //             D3D12_INDEX_BUFFER_VIEW ibv;
        //             ibv.BufferLocation = buffer->GetGpuAddress() + offset;
        //             ibv.SizeInBytes    = buffer->GetDesc().size - offset;
        //             ibv.Format         = dxgi_format(format);

        //             m_pCommandList->IASetIndexBuffer(&ibv);
        //         }

        //         void D3D12CommandList::SetViewport(u32 x, u32 y, u32 width, u32 height)
        //         {
        //             D3D12_VIEWPORT vp = {(float)x, (float)y, (float)width, (float)height, 0.0f, 1.0f};
        //             m_pCommandList->RSSetViewports(1, &vp);

        //             SetScissorRect(x, y, width, height);
        //         }

        //         void D3D12CommandList::SetScissorRect(u32 x, u32 y, u32 width, u32 height)
        //         {
        //             D3D12_RECT rect = {(LONG)x, (LONG)y, LONG(x + width), LONG(y + height)};
        //             m_pCommandList->RSSetScissorRects(1, &rect);
        //         }

        //         void D3D12CommandList::SetGraphicsConstants(u32 slot, const void* data, s64 data_size)
        //         {
        //             if (slot == 0)
        //             {
        //                 u32 consts_count = (u32)data_size / sizeof(u32);
        //                 ASSERT(consts_count <= GFX_MAX_ROOT_CONSTANTS);
        //                 m_pCommandList->SetGraphicsRoot32BitConstants(0, consts_count, data, 0);
        //             }
        //             else
        //             {
        //                 ASSERT(slot < GFX_MAX_CBV_BINDINGS);
        //                 D3D12_GPU_VIRTUAL_ADDRESS address = ((D3D12Device*)m_pDevice)->AllocateConstantBuffer(data, data_size);
        //                 ASSERT(address);

        //                 m_pCommandList->SetGraphicsRootConstantBufferView(slot, address);
        //             }
        //         }

        //         void D3D12CommandList::SetComputeConstants(u32 slot, const void* data, s64 data_size)
        //         {
        //             if (slot == 0)
        //             {
        //                 u32 consts_count = (u32)data_size / sizeof(u32);
        //                 ASSERT(consts_count <= GFX_MAX_ROOT_CONSTANTS);
        //                 m_pCommandList->SetComputeRoot32BitConstants(0, consts_count, data, 0);
        //             }
        //             else
        //             {
        //                 ASSERT(slot < GFX_MAX_CBV_BINDINGS);
        //                 D3D12_GPU_VIRTUAL_ADDRESS address = ((D3D12Device*)m_pDevice)->AllocateConstantBuffer(data, data_size);
        //                 ASSERT(address);

        //                 m_pCommandList->SetComputeRootConstantBufferView(slot, address);
        //             }
        //         }

        //         void D3D12CommandList::Draw(u32 vertex_count, u32 instance_count)
        //         {
        //             m_pCommandList->DrawInstanced(vertex_count, instance_count, 0, 0);
        //             ++m_commandCount;
        //         }

        //         void D3D12CommandList::DrawIndexed(u32 index_count, u32 instance_count, u32 index_offset)
        //         {
        //             m_pCommandList->DrawIndexedInstanced(index_count, instance_count, index_offset, 0, 0);
        //             ++m_commandCount;
        //         }

        //         void D3D12CommandList::Dispatch(u32 group_count_x, u32 group_count_y, u32 group_count_z)
        //         {
        //             FlushBarriers();

        //             m_pCommandList->Dispatch(group_count_x, group_count_y, group_count_z);
        //             ++m_commandCount;
        //         }

        //         void D3D12CommandList::DispatchMesh(u32 group_count_x, u32 group_count_y, u32 group_count_z)
        //         {
        //             m_pCommandList->DispatchMesh(group_count_x, group_count_y, group_count_z);
        //             ++m_commandCount;
        //         }

        //         void D3D12CommandList::DrawIndirect(buffer_t* buffer, u32 offset)
        //         {
        //             ID3D12CommandSignature* signature = ((D3D12Device*)m_pDevice)->GetDrawSignature();
        //             m_pCommandList->ExecuteIndirect(signature, 1, (ID3D12Resource*)buffer->GetHandle(), offset, nullptr, 0);
        //             ++m_commandCount;
        //         }

        //         void D3D12CommandList::DrawIndexedIndirect(buffer_t* buffer, u32 offset)
        //         {
        //             ID3D12CommandSignature* signature = ((D3D12Device*)m_pDevice)->GetDrawIndexedSignature();
        //             m_pCommandList->ExecuteIndirect(signature, 1, (ID3D12Resource*)buffer->GetHandle(), offset, nullptr, 0);
        //             ++m_commandCount;
        //         }

        //         void D3D12CommandList::DispatchIndirect(buffer_t* buffer, u32 offset)
        //         {
        //             FlushBarriers();

        //             ID3D12CommandSignature* signature = ((D3D12Device*)m_pDevice)->GetDispatchSignature();
        //             m_pCommandList->ExecuteIndirect(signature, 1, (ID3D12Resource*)buffer->GetHandle(), offset, nullptr, 0);
        //             ++m_commandCount;
        //         }

        //         void D3D12CommandList::DispatchMeshIndirect(buffer_t* buffer, u32 offset)
        //         {
        //             ID3D12CommandSignature* signature = ((D3D12Device*)m_pDevice)->GetDispatchMeshSignature();
        //             m_pCommandList->ExecuteIndirect(signature, 1, (ID3D12Resource*)buffer->GetHandle(), offset, nullptr, 0);
        //             ++m_commandCount;
        //         }

        //         void D3D12CommandList::MultiDrawIndirect(u32 max_count, buffer_t* args_buffer, u32 args_buffer_offset, buffer_t* count_buffer, u32 count_buffer_offset)
        //         {
        //             ID3D12CommandSignature* signature = ((D3D12Device*)m_pDevice)->GetMultiDrawSignature();
        //             m_pCommandList->ExecuteIndirect(signature, max_count, (ID3D12Resource*)args_buffer->GetHandle(), args_buffer_offset, (ID3D12Resource*)count_buffer->GetHandle(), count_buffer_offset);
        //             ++m_commandCount;
        //         }

        //         void D3D12CommandList::MultiDrawIndexedIndirect(u32 max_count, buffer_t* args_buffer, u32 args_buffer_offset, buffer_t* count_buffer, u32 count_buffer_offset)
        //         {
        //             ID3D12CommandSignature* signature = ((D3D12Device*)m_pDevice)->GetMultiDrawIndexedSignature();
        //             m_pCommandList->ExecuteIndirect(signature, max_count, (ID3D12Resource*)args_buffer->GetHandle(), args_buffer_offset, (ID3D12Resource*)count_buffer->GetHandle(), count_buffer_offset);
        //             ++m_commandCount;
        //         }

        //         void D3D12CommandList::MultiDispatchIndirect(u32 max_count, buffer_t* args_buffer, u32 args_buffer_offset, buffer_t* count_buffer, u32 count_buffer_offset)
        //         {
        //             ID3D12CommandSignature* signature = ((D3D12Device*)m_pDevice)->GetMultiDispatchSignature();
        //             m_pCommandList->ExecuteIndirect(signature, max_count, (ID3D12Resource*)args_buffer->GetHandle(), args_buffer_offset, (ID3D12Resource*)count_buffer->GetHandle(), count_buffer_offset);
        //             ++m_commandCount;
        //         }

        //         void D3D12CommandList::MultiDispatchMeshIndirect(u32 max_count, buffer_t* args_buffer, u32 args_buffer_offset, buffer_t* count_buffer, u32 count_buffer_offset)
        //         {
        //             ID3D12CommandSignature* signature = ((D3D12Device*)m_pDevice)->GetMultiDispatchMeshSignature();
        //             m_pCommandList->ExecuteIndirect(signature, max_count, (ID3D12Resource*)args_buffer->GetHandle(), args_buffer_offset, (ID3D12Resource*)count_buffer->GetHandle(), count_buffer_offset);
        //             ++m_commandCount;
        //         }

        //         void D3D12CommandList::BuildRayTracingBLAS(blas_t* blas)
        //         {
        //             FlushBarriers();

        //             m_pCommandList->BuildRaytracingAccelerationStructure(((D3D12RayTracingBLAS*)blas)->GetBuildDesc(), 0, nullptr);
        //             ++m_commandCount;
        //         }

        //         void D3D12CommandList::UpdateRayTracingBLAS(blas_t* blas, buffer_t* vertex_buffer, u32 vertex_buffer_offset)
        //         {
        //             FlushBarriers();

        //             D3D12_RAYTRACING_GEOMETRY_DESC                     geometry;
        //             D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC desc;
        //             ((D3D12RayTracingBLAS*)blas)->GetUpdateDesc(desc, geometry, vertex_buffer, vertex_buffer_offset);

        //             m_pCommandList->BuildRaytracingAccelerationStructure(&desc, 0, nullptr);
        //             ++m_commandCount;
        //         }

        //         void D3D12CommandList::BuildRayTracingTLAS(tlas_t* tlas, const GfxRayTracing::Instance* instances, u32 instance_count)
        //         {
        //             FlushBarriers();

        //             D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC desc = {};
        //             ((D3D12RayTracingTLAS*)tlas)->GetBuildDesc(desc, instances, instance_count);

        //             m_pCommandList->BuildRaytracingAccelerationStructure(&desc, 0, nullptr);
        //             ++m_commandCount;
        //         }

        // #if MICROPROFILE_GPU_TIMERS
        //         MicroProfileThreadLogGpu* D3D12CommandList::GetProfileLog() const
        //         {
        //     #if MICROPROFILE_GPU_TIMERS_D3D12
        //             return m_pProfileLog;
        //     #else
        //             return nullptr;
        //     #endif
        //         }
        // #endif

        // --------------------------------------------------------------------------------------------------------------------------------
        // --------------------------------------------------------------------------------------------------------------------------------
        // --------------------------------------------------------------------------------------------------------------------------------
        // --------------------------------------------------------------------------------------------------------------------------------

        namespace nd3d12
        {
            void CreateCommandList(ngfx::command_list_t* cmdList) { nd3d12::command_list_t* dxCmdList = CreateComponent<ngfx::command_list_t, nd3d12::command_list_t>(cmdList->m_device, cmdList); }

            void DestroyCommandList(ngfx::command_list_t* cmdList)
            {
                nd3d12::command_list_t* dxCmdList = GetComponent<ngfx::command_list_t, nd3d12::command_list_t>(cmdList->m_device, cmdList);
                if (dxCmdList)
                {
                    nd3d12::Destroy(cmdList);
                    DestroyComponent<ngfx::command_list_t, nd3d12::command_list_t>(cmdList->m_device, cmdList);
                }
            }

            bool Create(ngfx::command_list_t* cmdList)
            {
                nd3d12::device_t*       dxDevice  = GetComponent<ngfx::device_t, nd3d12::device_t>(cmdList->m_device, cmdList->m_device);
                nd3d12::command_list_t* dxCmdList = GetComponent<ngfx::command_list_t, nd3d12::command_list_t>(cmdList->m_device, cmdList);

                D3D12_COMMAND_LIST_TYPE type;
                switch (cmdList->m_queueType)
                {
                    case enums::CommandQueueGraphics:
                        type                       = D3D12_COMMAND_LIST_TYPE_DIRECT;
                        dxCmdList->m_pCommandQueue = dxDevice->m_pGraphicsQueue;
                        dxCmdList->m_nProfileQueue = dxDevice->m_nProfileGraphicsQueue;
                        break;
                    case enums::CommandQueueCompute:
                        type                       = D3D12_COMMAND_LIST_TYPE_COMPUTE;
                        dxCmdList->m_pCommandQueue = dxDevice->m_pComputeQueue;
                        dxCmdList->m_nProfileQueue = dxDevice->m_nProfileComputeQueue;
                        break;
                    case enums::CommandQueueCopy:
                        type                       = D3D12_COMMAND_LIST_TYPE_COPY;
                        dxCmdList->m_pCommandQueue = dxDevice->m_pCopyQueue;
                        dxCmdList->m_nProfileQueue = dxDevice->m_nProfileCopyQueue;
                        break;
                    default: break;
                }

                ID3D12Device12* pD3D12Device = dxDevice->m_pDevice;
                HRESULT       hr           = pD3D12Device->CreateCommandAllocator(type, IID_PPV_ARGS(&dxCmdList->m_pCommandAllocator));
                if (FAILED(hr))
                {
                    return false;
                }

                const name_t* name = GetComponent<ngfx::command_list_t, name_t>(cmdList->m_device, cmdList);
                dxCmdList->m_pCommandAllocator->SetName(name->m_wname);

                hr = pD3D12Device->CreateCommandList(0, type, dxCmdList->m_pCommandAllocator, nullptr, IID_PPV_ARGS(&dxCmdList->m_pCommandList));
                if (FAILED(hr))
                {
                    return false;
                }

                dxCmdList->m_pCommandList->Close();
                return true;
            }

            void Destroy(ngfx::command_list_t* cmdList)
            {
                nd3d12::device_t*       dxDevice  = GetComponent<ngfx::device_t, nd3d12::device_t>(cmdList->m_device, cmdList->m_device);
                nd3d12::command_list_t* dxCmdList = GetComponent<ngfx::command_list_t, nd3d12::command_list_t>(cmdList->m_device, cmdList);
                nd3d12::Delete(dxDevice, dxCmdList->m_pCommandAllocator);
                nd3d12::Delete(dxDevice, dxCmdList->m_pCommandList);
            }

            void ResetAllocator(ngfx::command_list_t* cmdList)
            {
                nd3d12::command_list_t* dxCmdList = GetComponent<ngfx::command_list_t, nd3d12::command_list_t>(cmdList->m_device, cmdList);
                dxCmdList->m_pCommandAllocator->Reset();
            }

            void Begin(ngfx::command_list_t* cmdList)
            {
                nd3d12::command_list_t* dxCmdList = GetComponent<ngfx::command_list_t, nd3d12::command_list_t>(cmdList->m_device, cmdList);
                dxCmdList->m_pCommandAllocator->Reset();
                dxCmdList->m_pCommandList->Reset(dxCmdList->m_pCommandAllocator, nullptr);
            }

            void End(ngfx::command_list_t* cmdList)
            {
                nd3d12::command_list_t* dxCmdList = GetComponent<ngfx::command_list_t, nd3d12::command_list_t>(cmdList->m_device, cmdList);
                dxCmdList->m_pCommandList->Close();
            }

            void Wait(ngfx::command_list_t* cmdList, ngfx::fence_t* fence, u64 value)
            {
                nd3d12::command_list_t* dxCmdList = GetComponent<ngfx::command_list_t, nd3d12::command_list_t>(cmdList->m_device, cmdList);
                nd3d12::command_list_t::fence_value_t fenceValue = {fence, value};
                dxCmdList->m_pendingWaits.push_back(fenceValue);
            }

            void Signal(ngfx::command_list_t* cmdList, ngfx::fence_t* fence, u64 value)
            {
                nd3d12::command_list_t* dxCmdList = GetComponent<ngfx::command_list_t, nd3d12::command_list_t>(cmdList->m_device, cmdList);
                nd3d12::command_list_t::fence_value_t fenceValue = {fence, value};
                dxCmdList->m_pendingSignals.push_back(fencealue);
            }

            void Present(ngfx::command_list_t* cmdList, ngfx::swapchain_t* swapchain)
            {
                nd3d12::command_list_t* dxCmdList = GetComponent<ngfx::command_list_t, nd3d12::command_list_t>(cmdList->m_device, cmdList);
                dxCmdList->m_pendingSwapchain.push_back(swapchain);
            }

            void Submit(ngfx::command_list_t* cmdList)
            {
                nd3d12::command_list_t* dxCmdList = GetComponent<ngfx::command_list_t, nd3d12::command_list_t>(cmdList->m_device, cmdList);

                for (s32 i = 0; i < dxCmdList->m_pendingWaits.size(); ++i)
                {
                    ngfx::fence_t*   fence   = dxCmdList->m_pendingWaits[i].m_fence;
                    nd3d12::fence_t* dxFence = GetComponent<ngfx::fence_t, nd3d12::fence_t>(cmdList->m_device, fence);
                    u64 const        value   = dxCmdList->m_pendingWaits[i].m_value;
                    dxCmdList->m_pCommandQueue->Wait(dxFence->m_pFence, value);
                }
                dxCmdList->m_pendingWaits.clear();

                if (dxCmdList->m_commandCount > 0)
                {
                    ID3D12CommandList* ppCommandLists[] = {dxCmdList->m_pCommandList};
                    dxCmdList->m_pCommandQueue->ExecuteCommandLists(1, ppCommandLists);
                }

                for (s32 i = 0; i < dxCmdList->m_pendingSwapchain.size(); ++i)
                {
                    ngfx::swapchain_t* sc = dxCmdList->m_pendingSwapchain[i];
                    nd3d12::Present(cmdList->m_device, sc);
                }
                dxCmdList->m_pendingSwapchain.clear();

                for (s32 i = 0; i < dxCmdList->m_pendingSignals.size(); ++i)
                {
                    ngfx::fence_t*   fence   = dxCmdList->m_pendingSignals[i].m_fence;
                    nd3d12::fence_t* dxFence = GetComponent<ngfx::fence_t, nd3d12::fence_t>(cmdList->m_device, fence);
                    u64 const        value   = dxCmdList->m_pendingSignals[i].m_value;
                    dxCmdList->m_pCommandQueue->Signal(dxFence, value);
                }
                dxCmdList->m_pendingSignals.clear();
            }

            void ResetState(ngfx::command_list_t* cmdList)
            {
                nd3d12::command_list_t* dxCmdList = GetComponent<ngfx::command_list_t, nd3d12::command_list_t>(cmdList->m_device, cmdList);
                nd3d12::device_t*       dxDevice  = GetComponent<ngfx::device_t, nd3d12::device_t>(cmdList->m_device, cmdList->m_device);

                dxCmdList->m_commandCount = 0;
                dxCmdList->m_pCurrentPSO  = nullptr;

                if (cmdList->m_queueType == enums::CommandQueueGraphics || cmdList->m_queueType == enums::CommandQueueCompute)
                {
                    ID3D12DescriptorHeap* heaps[2] = {dxDevice->m_pResourceDescriptorHeap, dxDevice->m_pSamplerDescriptorHeap};
                    dxCmdList->m_pCommandList->SetDescriptorHeaps(2, heaps);

                    ID3D12RootSignature* pRootSignature = dxDevice->m_pRootSignature;
                    dxCmdList->m_pCommandList->SetComputeRootSignature(pRootSignature);

                    if (cmdList->m_queueType == enums::CommandQueueGraphics)
                    {
                        dxCmdList->m_pCommandList->SetGraphicsRootSignature(pRootSignature);
                    }
                }
            }

            void BeginProfiling(ngfx::command_list_t* cmdList);
            {
    #if MICROPROFILE_GPU_TIMERS_D3D12
                nd3d12::command_list_t* dxCmdList = GetComponent<ngfx::command_list_t, nd3d12::command_list_t>(cmdList->m_device, cmdList);
                if (dxCmdList->m_nProfileQueue != -1)
                {
                    dxCmdList->m_pProfileLog = MicroProfileThreadLogGpuAlloc();
                    MicroProfileGpuBegin(dxCmdList->m_pCommandList, dxCmdList->m_pProfileLog);
                }
    #endif
            }

            void EndProfiling(ngfx::command_list_t* cmdList);
            {
    #if MICROPROFILE_GPU_TIMERS_D3D12
                nd3d12::command_list_t* dxCmdList = GetComponent<ngfx::command_list_t, nd3d12::command_list_t>(cmdList->m_device, cmdList);
                if (dxCmdList->m_nProfileQueue != -1)
                {
                    MicroProfileGpuSubmit(dxCmdList->m_nProfileQueue, MicroProfileGpuEnd(dxCmdList->m_pProfileLog));
                    MicroProfileThreadLogGpuFree(dxCmdList->m_pProfileLog);
                }
    #endif
            }

            void BeginEvent(ngfx::command_list_t* cmdList, const char* event_name);
            {
                nd3d12::command_list_t* dxCmdList = GetComponent<ngfx::command_list_t, nd3d12::command_list_t>(cmdList->m_device, cmdList);
                pix::BeginEvent(dxCmdList->m_pCommandList, event_name);
                ags::BeginEvent(dxCmdList->m_pCommandList, event_name);
            }

            void EndEvent(ngfx::command_list_t* cmdList);
            {
                nd3d12::command_list_t* dxCmdList = GetComponent<ngfx::command_list_t, nd3d12::command_list_t>(cmdList->m_device, cmdList);
                pix::EndEvent(dxCmdList->m_pCommandList);
                ags::EndEvent(dxCmdList->m_pCommandList);
            }

            void CopyBufferToTexture(ngfx::command_list_t* cmdList, ngfx::texture_t* dst_texture, u32 mip_level, u32 array_slice, buffer_t* src_buffer, u32 offset)
            {
                nd3d12::command_list_t* dxCmdList = GetComponent<ngfx::command_list_t, nd3d12::command_list_t>(cmdList->m_device, cmdList);
                nd3d12::texture_t*      dxTexture = GetComponent<ngfx::texture_t, nd3d12::texture_t>(cmdList->m_device, dst_texture);
                nd3d12::buffer_t*       dxBuffer  = GetComponent<ngfx::buffer_t, nd3d12::buffer_t>(cmdList->m_device, src_buffer);

                FlushBarriers();

                const texture_desc_t& desc = dst_texture->GetDesc();

                u32 min_width  = enums::GetFormatBlockWidth(desc.format);
                u32 min_height = enums::GetFormatBlockHeight(desc.format);
                u32 w          = math::g_max(desc.width >> mip_level, min_width);
                u32 h          = math::g_max(desc.height >> mip_level, min_height);
                u32 d          = math::g_max(desc.depth >> mip_level, 1u);

                D3D12_TEXTURE_COPY_LOCATION dst = {};
                dst.pResource                   = dxTexture->m_pTexture;
                dst.Type                        = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
                dst.SubresourceIndex            = CalcSubresource(desc, mip_level, array_slice);

                D3D12_TEXTURE_COPY_LOCATION src        = {};
                src.pResource                          = dxBuffer->m_pBuffer;
                src.Type                               = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
                src.PlacedFootprint.Offset             = offset;
                src.PlacedFootprint.Footprint.Format   = dxgi_format(desc.format);
                src.PlacedFootprint.Footprint.Width    = w;
                src.PlacedFootprint.Footprint.Height   = h;
                src.PlacedFootprint.Footprint.Depth    = d;
                src.PlacedFootprint.Footprint.RowPitch = nd3d12::GetRowPitch(cmdList->m_device, dst_texture, mip_level);

                dxCmdList->m_pCommandList->CopyTextureRegion(&dst, 0, 0, 0, &src, nullptr);
                dxCmdList->m_commandCount += 1;
            }

            void CopyTextureToBuffer(ngfx::command_list_t* cmdList, ngfx::buffer_t* dst_buffer, u32 offset, texture_t* src_texture, u32 mip_level, u32 array_slice);
            void CopyBuffer(ngfx::command_list_t* cmdList, ngfx::buffer_t* dst, u32 dst_offset, buffer_t* src, u32 src_offset, u32 size);
            void CopyTexture(ngfx::command_list_t* cmdList, ngfx::texture_t* dst, u32 dst_mip, u32 dst_array, texture_t* src, u32 src_mip, u32 src_array);
            void ClearUAV(ngfx::command_list_t* cmdList, ngfx::descriptor_t* uav, const float* clear_value);
            void ClearUAV(ngfx::command_list_t* cmdList, ngfx::descriptor_t* uav, const u32* clear_value);
            void WriteBuffer(ngfx::command_list_t* cmdList, ngfx::buffer_t* buffer, u32 offset, u32 data);
            void UpdateTileMappings(ngfx::command_list_t* cmdList, ngfx::texture_t* texture, ngfx::heap_t* heap, u32 mapping_count, const tile_mapping_t* mappings);

            void TextureBarrier(ngfx::command_list_t* cmdList, ngfx::texture_t* texture, u32 sub_resource, enums::access_flags access_before, enums::access_flags access_after);
            void BufferBarrier(ngfx::command_list_t* cmdList, ngfx::buffer_t* buffer, enums::access_flags access_before, enums::access_flags access_after);
            void GlobalBarrier(ngfx::command_list_t* cmdList, enums::access_flags access_before, enums::access_flags access_after);
            void FlushBarriers(ngfx::command_list_t* cmdList);

            void BeginRenderPass(ngfx::command_list_t* cmdList, const renderpass_desc_t& render_pass);
            void EndRenderPass(ngfx::command_list_t* cmdList);
            void SetPipelineState(ngfx::command_list_t* cmdList, ngfx::pipeline_state_t* state);
            void SetStencilReference(ngfx::command_list_t* cmdList, u8 stencil);
            void SetBlendFactor(ngfx::command_list_t* cmdList, const float* blend_factor);
            void SetIndexBuffer(ngfx::command_list_t* cmdList, buffer_t* buffer, u32 offset, enums::format format);
            void SetViewport(ngfx::command_list_t* cmdList, u32 x, u32 y, u32 width, u32 height);
            void SetScissorRect(ngfx::command_list_t* cmdList, u32 x, u32 y, u32 width, u32 height);
            void SetGraphicsConstants(ngfx::command_list_t* cmdList, u32 slot, const void* data, s64 data_size);
            void SetComputeConstants(ngfx::command_list_t* cmdList, u32 slot, const void* data, s64 data_size);

            void Draw(ngfx::command_list_t* cmdList, u32 vertex_count, u32 instance_count);
            void DrawIndexed(ngfx::command_list_t* cmdList, u32 index_count, u32 instance_count, u32 index_offset);
            void Dispatch(ngfx::command_list_t* cmdList, u32 group_count_x, u32 group_count_y, u32 group_count_z);
            void DispatchMesh(ngfx::command_list_t* cmdList, u32 group_count_x, u32 group_count_y, u32 group_count_z);

            void DrawIndirect(ngfx::command_list_t* cmdList, ngfx::buffer_t* buffer, u32 offset);
            void DrawIndexedIndirect(ngfx::command_list_t* cmdList, ngfx::buffer_t* buffer, u32 offset);
            void DispatchIndirect(ngfx::command_list_t* cmdList, ngfx::buffer_t* buffer, u32 offset);
            void DispatchMeshIndirect(ngfx::command_list_t* cmdList, ngfx::buffer_t* buffer, u32 offset);

            void MultiDrawIndirect(ngfx::command_list_t* cmdList, u32 max_count, ngfx::buffer_t* args_buffer, u32 args_buffer_offset, ngfx::buffer_t* count_buffer, u32 count_buffer_offset);
            void MultiDrawIndexedIndirect(ngfx::command_list_t* cmdList, u32 max_count, ngfx::buffer_t* args_buffer, u32 args_buffer_offset, ngfx::buffer_t* count_buffer, u32 count_buffer_offset);
            void MultiDispatchIndirect(ngfx::command_list_t* cmdList, u32 max_count, ngfx::buffer_t* args_buffer, u32 args_buffer_offset, ngfx::buffer_t* count_buffer, u32 count_buffer_offset);
            void MultiDispatchMeshIndirect(ngfx::command_list_t* cmdList, u32 max_count, ngfx::buffer_t* args_buffer, u32 args_buffer_offset, ngfx::buffer_t* count_buffer, u32 count_buffer_offset);

            void BuildRayTracingBLAS(ngfx::command_list_t* cmdList, ngfx::blas_t* blas);
            void UpdateRayTracingBLAS(ngfx::command_list_t* cmdList, ngfx::blas_t* blas, ngfx::buffer_t* vertex_buffer, u32 vertex_buffer_offset);
            void BuildRayTracingTLAS(ngfx::command_list_t* cmdList, ngfx::tlas_t* tlas, const rt_instance_t* instances, u32 instance_count);

        }  // namespace nd3d12
#endif
    }  // namespace ngfx
}  // namespace ncore
