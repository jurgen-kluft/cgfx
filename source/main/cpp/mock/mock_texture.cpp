#include "cgfx/mock/mock_texture.h"
#include "cgfx/mock/mock_device.h"
#include "cgfx/gfx.h"

namespace ncore
{
    namespace ngfx
    {
        MockTexture::MockTexture(MockDevice* pDevice, const GfxTextureDesc& desc, const eastl::string& name)
        {
            m_pDevice = pDevice;
            m_desc    = desc;
            m_name    = name;
        }

        MockTexture::~MockTexture() {}

        bool MockTexture::Create() { return true; }

        void* MockTexture::GetHandle() const { return nullptr; }

        u32 MockTexture::GetRequiredStagingBufferSize() const { return m_pDevice->GetAllocationSize(m_desc); }

        u32 MockTexture::GetRowPitch(u32 mip_level) const
        {
            u32 min_width = GetFormatBlockWidth(m_desc.format);
            u32 width     = eastl::max(m_desc.width >> mip_level, min_width);

            return GetFormatRowPitch(m_desc.format, width) * GetFormatBlockHeight(m_desc.format);
        }

        GfxTilingDesc MockTexture::GetTilingDesc() const { return GfxTilingDesc(); }

        GfxSubresourceTilingDesc MockTexture::GetTilingDesc(u32 subresource) const { return GfxSubresourceTilingDesc(); }

        void* MockTexture::GetSharedHandle() const { return nullptr; }

    }  // namespace ngfx
}  // namespace ncore
