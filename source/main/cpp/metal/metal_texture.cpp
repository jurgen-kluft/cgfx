#include "cbase/c_integer.h"
#include "cgfx/metal/metal_texture.h"
#include "cgfx/metal/metal_device.h"
#include "cgfx/metal/metal_heap.h"
#include "cgfx/gfx.h"

namespace ncore
{
    namespace ngfx
    {
        namespace nmetal
        {
            void CreateTexture(ngfx::device_t* device, ngfx::texture_t* texture, const texture_desc_t& desc)
            {
                nmetal::mtexture_t* mtexture = CreateComponent<ngfx::texture_t, nmetal::mtexture_t>(device, texture);
            }

            bool Create(ngfx::device_t* device, ngfx::texture_t* texture)
            {
                // TODO separate the heap and heap offset into a separate component, this shouldn't
                //      be part of the texture desc
                MTL::TextureDescriptor* descriptor = ToTextureDescriptor(texture->m_desc);

                nmetal::device_t* mdevice  = GetComponent<ngfx::device_t, nmetal::device_t>(device, device);
                mtexture_t*       mtexture = GetComponent<ngfx::texture_t, mtexture_t>(device, texture);
                if (texture->m_desc.heap)
                {
                    ASSERT(texture->m_desc.alloc_type == enums::AllocationPlaced);
                    ASSERT(texture->m_desc.memory_type == texture->m_desc.heap->GetDesc().memory_type);

                    MTL::Heap* heap      = ngfx::nmetal::GetHandle(device, texture->m_desc.heap);
                    mtexture->m_pTexture = heap->newTexture(descriptor, texture->m_desc.heap_offset);
                }
                else
                {
                    mtexture->m_pTexture = mdevice->m_pDevice->newTexture(descriptor);
                }

                descriptor->release();

                if (mtexture->m_pTexture == nullptr)
                {
                    // RE_ERROR("[MetalTexture] failed to create {}", m_name);
                    return false;
                }

                nmetal::MakeResident(mdevice, mtexture->m_pTexture);

                name_t const* name = GetComponent<ngfx::texture_t, name_t>(device, texture);
                SetDebugLabel(mtexture->m_pTexture, name->m_name);

                return true;
            }

            void Destroy(ngfx::device_t* device, ngfx::texture_t* texture)
            {
                nmetal::device_t* mdevice  = GetComponent<ngfx::device_t, nmetal::device_t>(device, device);
                mtexture_t*       mtexture = GetComponent<ngfx::texture_t, mtexture_t>(device, texture);
                Evict(mdevice, mtexture->m_pTexture);
                if (!mtexture->m_bSwapchainTexture)
                {
                    mtexture->m_pTexture->release();
                }
            }

            MTL::Texture* GetHandle(ngfx::device_t* device, ngfx::texture_t* texture)
            {
                mtexture_t* mtexture = GetComponent<ngfx::texture_t, mtexture_t>(device, texture);
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
