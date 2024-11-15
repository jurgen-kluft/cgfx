#ifndef __CGFX_MOCK_TEXTURE_H__
#define __CGFX_MOCK_TEXTURE_H__

#include "cgfx/gfx_texture.h"

namespace ncore
{
    namespace ngfx
    {
        namespace nmock
        {
            bool                     Create(device_t* pDevice, ngfx::texture_t*);
            void                     Destroy(device_t* pDevice, ngfx::texture_t*);
            void*                    GetHandle(device_t* pDevice, ngfx::texture_t*);
            u32                      GetRequiredStagingBufferSize(device_t* pDevice, ngfx::texture_t*);
            u32                      GetRowPitch(device_t* pDevice, ngfx::texture_t* texture, u32 mip_level = 0);
            GfxTilingDesc            GetTilingDesc(device_t* pDevice, ngfx::texture_t* texture);
            GfxSubresourceTilingDesc GetTilingDesc(device_t* pDevice, ngfx::texture_t* texture, u32 subresource = 0);
            void*                    GetSharedHandle(device_t* pDevice, ngfx::texture_t* texture);

        }  // namespace nmock

    }  // namespace ngfx
}  // namespace ncore
#endif
