#include "cbase/c_integer.h"
#include "cgfx/metal/metal_texture.h"
#include "cgfx/metal/metal_device.h"
#include "cgfx/metal/metal_heap.h"
#include "cgfx/gfx.h"

namespace ncore
{
    namespace ngfx
    {

        MetalTexture::MetalTexture(MetalDevice* pDevice, const texture_desc_t& desc, const char* name)
        {
            m_pDevice = pDevice;
            m_desc    = desc;
            m_name    = name;
        }

        MetalTexture::~MetalTexture()
        {
            ((MetalDevice*)m_pDevice)->Evict(m_pTexture);

            if (!m_bSwapchainTexture)
            {
                m_pTexture->release();
            }
        }

        bool MetalTexture::Create()
        {
            MTL::TextureDescriptor* descriptor = ToTextureDescriptor(m_desc);

            if (m_desc.heap)
            {
                ASSERT(m_desc.alloc_type == GfxAllocation::Placed);
                ASSERT(m_desc.memory_type == m_desc.heap->GetDesc().memory_type);

                MTL::Heap* heap = (MTL::Heap*)m_desc.heap->GetHandle();
                m_pTexture      = heap->newTexture(descriptor, m_desc.heap_offset);
            }
            else
            {
                MTL::Device* device = (MTL::Device*)m_pDevice->GetHandle();
                m_pTexture          = device->newTexture(descriptor);
            }

            descriptor->release();

            if (m_pTexture == nullptr)
            {
                // RE_ERROR("[MetalTexture] failed to create {}", m_name);
                return false;
            }

            ((MetalDevice*)m_pDevice)->MakeResident(m_pTexture);

            SetDebugLabel(m_pTexture, m_name);

            return true;
        }

        void MetalTexture::SetSwapchainTexture(MTL::Texture* texture)
        {
            m_pTexture    = texture;
            m_desc.width  = (u32)m_pTexture->width();
            m_desc.height = (u32)m_pTexture->height();

            m_bSwapchainTexture = true;
        }

        u32 MetalTexture::GetRequiredStagingBufferSize() const
        {
            u32 size = 0;

            u32 min_width  = GetFormatBlockWidth(m_desc.format);
            u32 min_height = GetFormatBlockHeight(m_desc.format);

            for (u32 layer = 0; layer < m_desc.array_size; ++layer)
            {
                for (u32 mip = 0; mip < m_desc.mip_levels; ++mip)
                {
                    u32 width  = math::max(m_desc.width >> mip, min_width);
                    u32 height = math::max(m_desc.height >> mip, min_height);
                    u32 depth  = math::max(m_desc.depth >> mip, 1u);

                    size += GetFormatRowPitch(m_desc.format, width) * height * depth;
                }
            }

            return size;
        }

        u32 MetalTexture::GetRowPitch(u32 mip_level) const
        {
            u32 min_width = GetFormatBlockWidth(m_desc.format);
            u32 width     = math::max(m_desc.width >> mip_level, min_width);

            return GetFormatRowPitch(m_desc.format, width) * GetFormatBlockHeight(m_desc.format);
        }

        GfxTilingDesc MetalTexture::GetTilingDesc() const
        {
            // todo
            return GfxTilingDesc();
        }

        GfxSubresourceTilingDesc MetalTexture::GetTilingDesc(u32 subresource) const
        {
            // todo
            return GfxSubresourceTilingDesc();
        }

        void* MetalTexture::GetSharedHandle() const
        {
            // todo
            return nullptr;
        }

    }  // namespace ngfx
}  // namespace ncore
