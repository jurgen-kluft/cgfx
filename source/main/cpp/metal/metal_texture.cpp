#include "cbase/c_integer.h"
#include "cgfx/metal/metal_texture.h"
#include "cgfx/metal/metal_device.h"
#include "cgfx/metal/metal_heap.h"
#include "cgfx/gfx.h"

namespace ncore
{
    namespace ngfx
    {
        // MetalTexture::MetalTexture(MetalDevice* pDevice, const texture_desc_t& desc, const char* name)
        // {
        //     m_pDevice = pDevice;
        //     m_desc    = desc;
        //     m_name    = name;
        // }

        // MetalTexture::~MetalTexture()
        // {
        //     ((MetalDevice*)m_pDevice)->Evict(m_pTexture);

        //     if (!m_bSwapchainTexture)
        //     {
        //         m_pTexture->release();
        //     }
        // }

        // bool MetalTexture::Create()
        // {
        //     MTL::TextureDescriptor* descriptor = ToTextureDescriptor(m_desc);

        //     if (m_desc.heap)
        //     {
        //         ASSERT(m_desc.alloc_type == GfxAllocation::Placed);
        //         ASSERT(m_desc.memory_type == m_desc.heap->GetDesc().memory_type);

        //         MTL::Heap* heap = (MTL::Heap*)m_desc.heap->GetHandle();
        //         m_pTexture      = heap->newTexture(descriptor, m_desc.heap_offset);
        //     }
        //     else
        //     {
        //         MTL::Device* device = (MTL::Device*)m_pDevice->GetHandle();
        //         m_pTexture          = device->newTexture(descriptor);
        //     }

        //     descriptor->release();

        //     if (m_pTexture == nullptr)
        //     {
        //         // RE_ERROR("[MetalTexture] failed to create {}", m_name);
        //         return false;
        //     }

        //     ((MetalDevice*)m_pDevice)->MakeResident(m_pTexture);

        //     SetDebugLabel(m_pTexture, m_name);

        //     return true;
        // }

        // void MetalTexture::SetSwapchainTexture(MTL::Texture* texture)
        // {
        //     m_pTexture    = texture;
        //     m_desc.width  = (u32)m_pTexture->width();
        //     m_desc.height = (u32)m_pTexture->height();

        //     m_bSwapchainTexture = true;
        // }

        // u32 MetalTexture::GetRequiredStagingBufferSize() const
        // {
        //     u32 size = 0;

        //     u32 min_width  = GetFormatBlockWidth(m_desc.format);
        //     u32 min_height = GetFormatBlockHeight(m_desc.format);

        //     for (u32 layer = 0; layer < m_desc.array_size; ++layer)
        //     {
        //         for (u32 mip = 0; mip < m_desc.mip_levels; ++mip)
        //         {
        //             u32 width  = math::max(m_desc.width >> mip, min_width);
        //             u32 height = math::max(m_desc.height >> mip, min_height);
        //             u32 depth  = math::max(m_desc.depth >> mip, 1u);

        //             size += GetFormatRowPitch(m_desc.format, width) * height * depth;
        //         }
        //     }

        //     return size;
        // }

        // u32 MetalTexture::GetRowPitch(u32 mip_level) const
        // {
        //     u32 min_width = GetFormatBlockWidth(m_desc.format);
        //     u32 width     = math::max(m_desc.width >> mip_level, min_width);

        //     return GetFormatRowPitch(m_desc.format, width) * GetFormatBlockHeight(m_desc.format);
        // }

        // tiling_desc_t MetalTexture::GetTilingDesc() const
        // {
        //     // todo
        //     return tiling_desc_t();
        // }

        // subresource_tiling_desc_t MetalTexture::GetTilingDesc(u32 subresource) const
        // {
        //     // todo
        //     return subresource_tiling_desc_t();
        // }

        // void* MetalTexture::GetSharedHandle() const
        // {
        //     // todo
        //     return nullptr;
        // }

        namespace nmetal
        {
            ngfx::texture_t* CreateTexture(ngfx::device_t* device, ngfx::texture_t* texture, const texture_desc_t& desc)
            {
                nmetal::mtexture_t* mtexture = AddAnotherComponent<ngfx::texture_t, nmetal::mtexture_t>(device, texture);

                return texture;
            }

            bool Create(ngfx::device_t* device, ngfx::texture_t* texture)
            {
                // TODO separate the heap and heap offset into a separate component, this shouldn't
                //      be part of the texture desc
                MTL::TextureDescriptor* descriptor = ToTextureDescriptor(texture->m_desc);

                mtexture_t* mtexture = GetOtherComponent<ngfx::texture_t, mtexture_t>(device, texture);
                if (texture->m_desc.heap)
                {
                    ASSERT(texture->m_desc.alloc_type == enums::AllocationPlaced);
                    ASSERT(texture->m_desc.memory_type == texture->m_desc.heap->GetDesc().memory_type);

                    MTL::Heap* heap      = (MTL::Heap*)GetHandle(device, texture->m_desc.heap);
                    mtexture->m_pTexture = heap->newTexture(descriptor, texture->m_desc.heap_offset);
                }
                else
                {
                    nmetal::device_t* mdevice = GetOtherComponent<ngfx::device_t, nmetal::device_t>(device, device);
                    mtexture->m_pTexture      = mdevice->m_pDevice->newTexture(descriptor);
                }

                descriptor->release();

                if (mtexture->m_pTexture == nullptr)
                {
                    // RE_ERROR("[MetalTexture] failed to create {}", m_name);
                    return false;
                }

                nmetal::MakeResident(device, mtexture->m_pTexture);

                name_t const* name = GetOtherComponent<ngfx::texture_t, name_t>(device, texture);
                SetDebugLabel(mtexture->m_pTexture, name->m_name);

                return true;
            }

            void Destroy(ngfx::device_t* device, ngfx::texture_t* texture)
            {
                mtexture_t* mtexture = GetOtherComponent<ngfx::texture_t, mtexture_t>(device, texture);
                Evict(device, mtexture->m_pTexture);
                if (!mtexture->m_bSwapchainTexture)
                {
                    mtexture->m_pTexture->release();
                }
            }

            void* GetHandle(ngfx::device_t* device, ngfx::texture_t* texture)
            {
                mtexture_t* mtexture = GetOtherComponent<ngfx::texture_t, mtexture_t>(device, texture);
                return mtexture->m_pTexture;
            }

            u32 GetRequiredStagingBufferSize(ngfx::device_t* device, ngfx::texture_t* texture)
            {
                u32 min_width  = enums::GetFormatBlockWidth(texture->m_desc.format);
                u32 min_height = enums::GetFormatBlockHeight(texture->m_desc.format);

                u32 size = 0;
                for (u32 layer = 0; layer < texture->m_desc.array_size; ++layer)
                {
                    for (u32 mip = 0; mip < texture->m_desc.mip_levels; ++mip)
                    {
                        u32 width  = math::g_max(texture->m_desc.width >> mip, min_width);
                        u32 height = math::g_max(texture->m_desc.height >> mip, min_height);
                        u32 depth  = math::g_max(texture->m_desc.depth >> mip, 1u);
                        size += enums::GetFormatRowPitch(texture->m_desc.format, width) * height * depth;
                    }
                }
                return size;
            }

            u32 GetRowPitch(ngfx::device_t* device, ngfx::texture_t* texture, u32 mip_level)
            {
                u32 min_width = enums::GetFormatBlockWidth(texture->m_desc.format);
                u32 width     = math::g_max(texture->m_desc.width >> mip_level, min_width);
                return enums::GetFormatRowPitch(texture->m_desc.format, width) * enums::GetFormatBlockHeight(texture->m_desc.format);
            }

            tiling_desc_t GetTilingDesc(ngfx::device_t* device, ngfx::texture_t* texture)
            {
                // TODO
                return tiling_desc_t();
            }

            subresource_tiling_desc_t GetSubResourceTilingDesc(ngfx::device_t* device, ngfx::texture_t* texture, u32 subresource)
            {
                // TODO
                return subresource_tiling_desc_t();
            }

            void* GetSharedHandle(ngfx::device_t* device, ngfx::texture_t* texture)
            {
                // TODO
                return nullptr;
            }

        }  // namespace nmetal

    }  // namespace ngfx
}  // namespace ncore
