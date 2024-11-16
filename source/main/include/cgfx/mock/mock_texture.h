#ifndef __CGFX_MOCK_TEXTURE_H__
#define __CGFX_MOCK_TEXTURE_H__

#include "cgfx/gfx_texture.h"

namespace ncore
{
    namespace ngfx
    {
        namespace nmock
        {
            ngfx::texture_t*         CreateTexture(device_t* device, ngfx::texture_t* texture, const texture_desc_t& desc);
            bool                     Create(device_t* device, ngfx::texture_t* texture);
            void                     Destroy(device_t* device, ngfx::texture_t* texture);
            void*                    GetHandle(device_t* device, ngfx::texture_t* texture);
            u32                      GetRequiredStagingBufferSize(device_t* device, ngfx::texture_t* texture);
            u32                      GetRowPitch(device_t* device, ngfx::texture_t* texture, u32 mip_level = 0);
            tiling_desc_t            GetTilingDesc(device_t* device, ngfx::texture_t* texture);
            subresource_tiling_desc_t GetSubResourceTilingDesc(device_t* device, ngfx::texture_t* texture, u32 subresource = 0);
            void*                    GetSharedHandle(device_t* device, ngfx::texture_t* texture);
        }  // namespace nmock
    }  // namespace ngfx
}  // namespace ncore
#endif
