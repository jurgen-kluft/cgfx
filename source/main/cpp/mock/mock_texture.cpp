#include "cgfx/mock/mock_texture.h"
#include "cgfx/mock/mock_device.h"
#include "cgfx/gfx.h"

#include "ccore/c_math.h"

namespace ncore
{
    namespace ngfx
    {
        namespace nmock
        {
            bool Create(ngfx::device_t* pDevice, texture_t* pTexture) { return true; }
            void Destroy(ngfx::device_t* pDevice, ngfx::texture_t* pTexture) {}

            void* GetHandle(ngfx::device_t* pDevice, ngfx::texture_t* pTexture) { return nullptr; }
            u32   GetRequiredStagingBufferSize(ngfx::device_t* pDevice, ngfx::texture_t* pTexture) { return ngfx::GetAllocationSize(pDevice, pTexture->m_desc); }

            u32 GetRowPitch(ngfx::device_t* pDevice, ngfx::texture_t* pTexture, u32 mip_level)
            {
                u32 min_width = ngfx::GetFormatBlockWidth(enums::cast(pTexture->m_desc.format, enums::format::FORMAT_UNKNOWN));
                u32 width     = math::g_max(pTexture->m_desc.width >> mip_level, min_width);

                return GetFormatRowPitch(enums::cast(pTexture->m_desc.format, enums::format::FORMAT_UNKNOWN), width) * GetFormatBlockHeight(enums::cast(pTexture->m_desc.format, enums::format::FORMAT_UNKNOWN));
            }

            GfxTilingDesc            GetTilingDesc(ngfx::device_t* pDevice, ngfx::texture_t* pTexture) { return GfxTilingDesc(); }
            GfxSubresourceTilingDesc GetTilingDesc(ngfx::device_t* pDevice, ngfx::texture_t* pTexture, u32 subresource) { return GfxSubresourceTilingDesc(); }
            void*                    GetSharedHandle(ngfx::device_t* pDevice, ngfx::texture_t* pTexture) { return nullptr; }

        }  // namespace nmock
    }  // namespace ngfx
}  // namespace ncore
