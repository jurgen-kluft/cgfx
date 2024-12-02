#include "cgfx/gfx.h"
#include "cbase/c_debug.h"
#include "cbase/c_log.h"

#include "cgfx/d3d12/d3d12_texture.h"
#include "cgfx/d3d12/d3d12_device.h"
#include "cgfx/d3d12/d3d12_heap.h"
#include "cd3d12/ma/D3D12MemAlloc.h"

namespace ncore
{
    namespace ngfx
    {
#ifdef TARGET_PC
        namespace nd3d12
        {
            void CreateTexture(ngfx::device_t* device, ngfx::texture_t* texture, const texture_desc_t& desc) { nd3d12::texture_t* dxtexture = CreateComponent<ngfx::texture_t, nd3d12::texture_t>(device, texture); }

            void DestroyTexture(ngfx::device_t* device, ngfx::texture_t* texture)
            {
                nd3d12::texture_t* dxtexture = GetComponent<ngfx::texture_t, nd3d12::texture_t>(device, texture);
                if (dxtexture)
                {
                    nd3d12::Destroy(device, texture);
                    DestroyComponent<ngfx::texture_t, nd3d12::texture_t>(device, texture);
                }
            }

            bool Create(ngfx::device_t* device, ngfx::texture_t* texture)
            {
                nd3d12::device_t*  dxdevice  = GetComponent<ngfx::device_t, nd3d12::device_t>(device, device);
                nd3d12::texture_t* dxtexture = GetComponent<ngfx::texture_t, nd3d12::texture_t>(device, texture);

                ID3D12Device*        pDevice       = dxdevice->m_pDevice;
                D3D12MA::Allocator*  pAllocator    = dxdevice->m_pResourceAllocator;
                D3D12_RESOURCE_DESC  resourceDesc  = d3d12_resource_desc(texture->m_desc);
                D3D12_RESOURCE_DESC1 resourceDesc1 = CD3DX12_RESOURCE_DESC1(resourceDesc);
                D3D12_BARRIER_LAYOUT initial_layout;
                if (texture->m_desc.usage & enums::TextureUsageRenderTarget)
                {
                    initial_layout = D3D12_BARRIER_LAYOUT_RENDER_TARGET;
                }
                else if (texture->m_desc.usage & enums::TextureUsageDepthStencil)
                {
                    initial_layout = D3D12_BARRIER_LAYOUT_DEPTH_STENCIL_WRITE;
                }
                else if (texture->m_desc.usage & enums::TextureUsageUnorderedAccess)
                {
                    initial_layout = D3D12_BARRIER_LAYOUT_UNORDERED_ACCESS;
                }
                else
                {
                    initial_layout = D3D12_BARRIER_LAYOUT_COMMON;
                }
                HRESULT hr;
                if (texture->m_desc.heap != nullptr)
                {
                    ASSERT(texture->m_desc.alloc_type == enums::AllocationPlaced);
                    ASSERT(texture->m_desc.memory_type == texture->m_desc.heap->m_desc.memory_type);
                    nd3d12::heap_t* heap = GetComponent<ngfx::heap_t, nd3d12::heap_t>(device, texture->m_desc.heap);
                    hr = pAllocator->CreateAliasingResource2(heap->m_pAllocation, texture->m_desc.heap_offset, &resourceDesc1, initial_layout, nullptr, 0, nullptr, IID_PPV_ARGS(&dxtexture->m_pTexture));
                }
                else if (texture->m_desc.alloc_type == enums::AllocationSparse)
                {
                    hr = dxdevice->m_pDevice->CreateReservedResource2(&resourceDesc, initial_layout, nullptr, nullptr, 0, nullptr, IID_PPV_ARGS(&dxtexture->m_pTexture));
                }
                else
                {
                    D3D12MA::ALLOCATION_DESC allocationDesc = {};
                    allocationDesc.HeapType                 = d3d12_heap_type(texture->m_desc.memory_type);
                    allocationDesc.Flags                    = texture->m_desc.alloc_type == enums::AllocationCommitted ? D3D12MA::ALLOCATION_FLAG_COMMITTED : D3D12MA::ALLOCATION_FLAG_NONE;

                    if (texture->m_desc.usage & enums::TextureUsageShared)
                    {
                        resourceDesc1.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
                        resourceDesc1.Flags |= D3D12_RESOURCE_FLAG_ALLOW_CROSS_ADAPTER;
                        allocationDesc.ExtraHeapFlags |= D3D12_HEAP_FLAG_SHARED | D3D12_HEAP_FLAG_SHARED_CROSS_ADAPTER;
                    }

                    hr = pAllocator->CreateResource3(&allocationDesc, &resourceDesc1, initial_layout, nullptr, 0, nullptr, &dxtexture->m_pAllocation, IID_PPV_ARGS(&dxtexture->m_pTexture));
                }

                name_t const* name = GetComponent<ngfx::texture_t, ngfx::name_t>(device, texture);

                if (FAILED(hr))
                {
                    D_LogError("ngfx::nd3d12::texture", "failed to create %s", va_t(name->c_str()));
                    return false;
                }

                dxtexture->m_pTexture->SetName(name->m_wname);
                if (dxtexture->m_pAllocation)
                {
                    dxtexture->m_pAllocation->SetName(name->m_wname);
                }

                if (texture->m_desc.usage & enums::TextureUsageShared)
                {
                    hr = dxdevice->m_pDevice->CreateSharedHandle(dxtexture->m_pTexture, nullptr, GENERIC_ALL, nullptr, &dxtexture->m_sharedHandle);
                    if (FAILED(hr))
                    {
                        D_LogError("ngfx::nd3d12::texture", "failed to create shared handle for %s", va_t(name->c_str()));
                        return false;
                    }
                }
                return true;
            }

            void Destroy(ngfx::device_t* device, ngfx::texture_t* texture)
            {
                nd3d12::texture_t* dxtexture = GetComponent<ngfx::texture_t, nd3d12::texture_t>(device, texture);
                nd3d12::device_t*  dxdevice  = GetComponent<ngfx::device_t, nd3d12::device_t>(device, device);

                nd3d12::Delete(dxdevice, dxtexture->m_pTexture);
                nd3d12::Delete(dxdevice, dxtexture->m_pAllocation);

                for (s32 i = 0; i < dxtexture->m_RTV.size(); ++i)
                {
                    nd3d12::DeleteRTV(dxdevice, dxtexture->m_RTV[i]);
                }
                for (s32 i = 0; i < dxtexture->m_DSV.size(); ++i)
                {
                    nd3d12::DeleteDSV(dxdevice, dxtexture->m_DSV[i]);
                }
            }

            u32 GetRequiredStagingBufferSize(ngfx::device_t* device, ngfx::texture_t* texture)
            {
                nd3d12::texture_t* dxtexture = GetComponent<ngfx::texture_t, nd3d12::texture_t>(device, texture);
                nd3d12::device_t*  dxdevice  = GetComponent<ngfx::device_t, nd3d12::device_t>(device, device);
                ID3D12Device*      pDevice   = dxdevice->m_pDevice;

                D3D12_RESOURCE_DESC desc              = dxtexture->m_pTexture->GetDesc();
                u32                 subresource_count = texture->m_desc.mip_levels * texture->m_desc.array_size;

                u64 size;
                pDevice->GetCopyableFootprints(&desc, 0, subresource_count, 0, nullptr, nullptr, nullptr, &size);
                return (u32)size;
            }

            u32 GetRowPitch(ngfx::device_t* device, ngfx::texture_t* texture, u32 mip_level)
            {
                ASSERT(mip_level < texture->m_desc.mip_levels);

                nd3d12::texture_t* dxtexture = GetComponent<ngfx::texture_t, nd3d12::texture_t>(device, texture);
                nd3d12::device_t*  dxdevice  = GetComponent<ngfx::device_t, nd3d12::device_t>(device, device);
                ID3D12Device*      pDevice   = dxdevice->m_pDevice;

                D3D12_RESOURCE_DESC desc = dxtexture->m_pTexture->GetDesc();

                D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint;
                pDevice->GetCopyableFootprints(&desc, mip_level, 1, 0, &footprint, nullptr, nullptr, nullptr);
                return footprint.Footprint.RowPitch;
            }

            tiling_desc_t GetTilingDesc(ngfx::device_t* device, ngfx::texture_t* texture)
            {
                ASSERT(texture->m_desc.alloc_type == enums::AllocationSparse);

                nd3d12::texture_t* dxtexture = GetComponent<ngfx::texture_t, nd3d12::texture_t>(device, texture);
                nd3d12::device_t*  dxdevice  = GetComponent<ngfx::device_t, nd3d12::device_t>(device, device);
                ID3D12Device*      pDevice   = dxdevice->m_pDevice;

                UINT                  tile_count;
                D3D12_PACKED_MIP_INFO packedMipInfo;
                D3D12_TILE_SHAPE      tileShape;
                pDevice->GetResourceTiling(dxtexture->m_pTexture, &tile_count, &packedMipInfo, &tileShape, nullptr, 0, nullptr);

                tiling_desc_t info;
                info.tile_count       = tile_count;
                info.standard_mips    = packedMipInfo.NumStandardMips;
                info.tile_width       = tileShape.WidthInTexels;
                info.tile_height      = tileShape.HeightInTexels;
                info.tile_depth       = tileShape.DepthInTexels;
                info.packed_mips      = packedMipInfo.NumPackedMips;
                info.packed_mip_tiles = packedMipInfo.NumTilesForPackedMips;

                return info;
            }

            subresource_tiling_desc_t GetSubResourceTilingDesc(ngfx::device_t* device, ngfx::texture_t* texture, u32 subresource)
            {
                nd3d12::texture_t* dxtexture = GetComponent<ngfx::texture_t, nd3d12::texture_t>(device, texture);
                nd3d12::device_t*  dxdevice  = GetComponent<ngfx::device_t, nd3d12::device_t>(device, device);
                ASSERT(texture->m_desc.alloc_type == enums::AllocationSparse);

                ID3D12Device* pDevice = dxdevice->m_pDevice;

                u32                      subresource_count = 1;
                D3D12_SUBRESOURCE_TILING tiling;
                pDevice->GetResourceTiling(dxtexture->m_pTexture, nullptr, nullptr, nullptr, &subresource_count, subresource, &tiling);

                subresource_tiling_desc_t info;
                info.width       = tiling.WidthInTiles;
                info.height      = tiling.HeightInTiles;
                info.depth       = tiling.DepthInTiles;
                info.tile_offset = tiling.StartTileIndexInOverallResource;

                return info;
            }

            void* GetSharedHandle(ngfx::device_t* device, ngfx::texture_t* texture)
            {
                nd3d12::texture_t* dxtexture = GetComponent<ngfx::texture_t, nd3d12::texture_t>(device, texture);
                return dxtexture->m_sharedHandle;
            }

            D3D12_CPU_DESCRIPTOR_HANDLE GetRTV(ngfx::device_t* device, ngfx::texture_t* texture, u32 mip_slice, u32 array_slice)
            {
                ASSERT(texture->m_desc.usage & enums::TextureUsageRenderTarget);

                nd3d12::texture_t* dxtexture = GetComponent<ngfx::texture_t, nd3d12::texture_t>(device, texture);
                nd3d12::device_t*  dxdevice  = GetComponent<ngfx::device_t, nd3d12::device_t>(device, device);
                if (dxtexture->m_RTV.empty())
                {
                    dxtexture->m_RTV.resize(texture->m_desc.mip_levels * texture->m_desc.array_size);
                }

                u32 const index = texture->m_desc.mip_levels * array_slice + mip_slice;
                if (IsNullDescriptor(dxtexture->m_RTV[index]))
                {
                    dxtexture->m_RTV[index] = nd3d12::AllocateRTV(dxdevice);

                    D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
                    rtvDesc.Format                        = dxgi_format(texture->m_desc.format);

                    switch (texture->m_desc.type)
                    {
                        case enums::TextureType2D:
                            rtvDesc.ViewDimension      = D3D12_RTV_DIMENSION_TEXTURE2D;
                            rtvDesc.Texture2D.MipSlice = mip_slice;
                            break;
                        case enums::TextureType2DArray:
                        case enums::TextureTypeCube:
                            rtvDesc.ViewDimension                  = D3D12_RTV_DIMENSION_TEXTURE2DARRAY;
                            rtvDesc.Texture2DArray.MipSlice        = mip_slice;
                            rtvDesc.Texture2DArray.FirstArraySlice = array_slice;
                            rtvDesc.Texture2DArray.ArraySize       = 1;
                            break;
                        default: ASSERT(false); break;
                    }

                    dxdevice->m_pDevice->CreateRenderTargetView(dxtexture->m_pTexture, &rtvDesc, dxtexture->m_RTV[index].cpu_handle);
                }

                return dxtexture->m_RTV[index].cpu_handle;
            }

            D3D12_CPU_DESCRIPTOR_HANDLE GetDSV(ngfx::device_t* device, ngfx::texture_t* texture, u32 mip_slice, u32 array_slice)
            {
                ASSERT(texture->m_desc.usage & enums::TextureUsageDepthStencil);

                nd3d12::texture_t* dxtexture = GetComponent<ngfx::texture_t, nd3d12::texture_t>(device, texture);
                nd3d12::device_t*  dxdevice  = GetComponent<ngfx::device_t, nd3d12::device_t>(device, device);
                if (dxtexture->m_DSV.empty())
                {
                    dxtexture->m_DSV.resize(texture->m_desc.mip_levels * texture->m_desc.array_size);
                }

                u32 const index = texture->m_desc.mip_levels * array_slice + mip_slice;
                if (IsNullDescriptor(dxtexture->m_DSV[index]))
                {
                    dxtexture->m_DSV[index] = nd3d12::AllocateDSV(dxdevice);

                    D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
                    dsvDesc.Format                        = dxgi_format(texture->m_desc.format);

                    switch (texture->m_desc.type)
                    {
                        case enums::TextureType2D:
                            dsvDesc.ViewDimension      = D3D12_DSV_DIMENSION_TEXTURE2D;
                            dsvDesc.Texture2D.MipSlice = mip_slice;
                            break;
                        case enums::TextureType2DArray:
                        case enums::TextureTypeCube:
                            dsvDesc.ViewDimension                  = D3D12_DSV_DIMENSION_TEXTURE2DARRAY;
                            dsvDesc.Texture2DArray.MipSlice        = mip_slice;
                            dsvDesc.Texture2DArray.FirstArraySlice = array_slice;
                            dsvDesc.Texture2DArray.ArraySize       = 1;
                            break;
                        default: ASSERT(false); break;
                    }

                    dxdevice->m_pDevice->CreateDepthStencilView(dxtexture->m_pTexture, &dsvDesc, dxtexture->m_DSV[index].cpu_handle);
                }

                return dxtexture->m_DSV[index].cpu_handle;
            }

            D3D12_CPU_DESCRIPTOR_HANDLE GetReadOnlyDSV(ngfx::device_t* device, ngfx::texture_t* texture, u32 mip_slice, u32 array_slice)
            {
                ASSERT(texture->m_desc.usage & enums::TextureUsageDepthStencil);

                nd3d12::device_t*  dxdevice  = GetComponent<ngfx::device_t, nd3d12::device_t>(device, device);
                nd3d12::texture_t* dxtexture = GetComponent<ngfx::texture_t, nd3d12::texture_t>(device, texture);

                if (dxtexture->m_readonlyDSV.empty())
                {
                    dxtexture->m_readonlyDSV.resize(texture->m_desc.mip_levels * texture->m_desc.array_size);
                }

                u32 const index = texture->m_desc.mip_levels * array_slice + mip_slice;
                if (IsNullDescriptor(dxtexture->m_readonlyDSV[index]))
                {
                    dxtexture->m_readonlyDSV[index] = AllocateDSV(dxdevice);

                    D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
                    dsvDesc.Format                        = dxgi_format(texture->m_desc.format);
                    dsvDesc.Flags                         = D3D12_DSV_FLAG_READ_ONLY_DEPTH;
                    if (enums::IsStencilFormat(texture->m_desc.format))
                    {
                        dsvDesc.Flags |= D3D12_DSV_FLAG_READ_ONLY_STENCIL;
                    }

                    switch (texture->m_desc.type)
                    {
                        case enums::TextureType2D:
                            dsvDesc.ViewDimension      = D3D12_DSV_DIMENSION_TEXTURE2D;
                            dsvDesc.Texture2D.MipSlice = mip_slice;
                            break;
                        case enums::TextureType2DArray:
                        case enums::TextureTypeCube:
                            dsvDesc.ViewDimension                  = D3D12_DSV_DIMENSION_TEXTURE2DARRAY;
                            dsvDesc.Texture2DArray.MipSlice        = mip_slice;
                            dsvDesc.Texture2DArray.FirstArraySlice = array_slice;
                            dsvDesc.Texture2DArray.ArraySize       = 1;
                            break;
                        default: ASSERT(false); break;
                    }

                    dxdevice->m_pDevice->CreateDepthStencilView(dxtexture->m_pTexture, &dsvDesc, dxtexture->m_readonlyDSV[index].cpu_handle);
                }

                return dxtexture->m_readonlyDSV[index].cpu_handle;
            }
        }  // namespace nd3d12
#endif
    }  // namespace ngfx
}  // namespace ncore
