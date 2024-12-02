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
            void CreateTexture(ngfx::device_t* device, ngfx::texture_t* texture, const texture_desc_t& desc) {}

            void DestroyTexture(ngfx::device_t* device, ngfx::texture_t* texture) {}

            bool Create(ngfx::device_t* pDevice, ngfx::texture_t* pTexture) { return true; }
            void Destroy(ngfx::device_t* pDevice, ngfx::texture_t* pTexture) {}

            u32   GetRequiredStagingBufferSize(ngfx::device_t* pDevice, ngfx::texture_t* pTexture) { return ngfx::GetAllocationSize(pDevice, pTexture->m_desc); }

            u32 GetRowPitch(ngfx::device_t* pDevice, ngfx::texture_t* pTexture, u32 mip_level)
            {
                u32 const min_width = enums::GetFormatBlockWidth(pTexture->m_desc.format);
                u32 const width     = math::g_max(pTexture->m_desc.width >> mip_level, min_width);
                return enums::GetFormatRowPitch(pTexture->m_desc.format, width) * enums::GetFormatBlockHeight(pTexture->m_desc.format);
            }

            tiling_desc_t             GetTilingDesc(ngfx::device_t* pDevice, ngfx::texture_t* pTexture) { return tiling_desc_t(); }
            subresource_tiling_desc_t GetSubResourceTilingDesc(ngfx::device_t* pDevice, ngfx::texture_t* pTexture, u32 subresource) { return subresource_tiling_desc_t(); }
            void*                     GetSharedHandle(ngfx::device_t* pDevice, ngfx::texture_t* pTexture) { return nullptr; }

        }  // namespace nmock
    }      // namespace ngfx
}  // namespace ncore
