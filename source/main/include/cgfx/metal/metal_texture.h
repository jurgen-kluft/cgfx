#ifndef __CGFX_METAL_TEXTURE_H__
#define __CGFX_METAL_TEXTURE_H__

#include "cgfx/gfx_texture.h"

namespace ncore
{
    namespace ngfx
    {
        namespace nmetal
        {
            ngfx::texture_t*          CreateTexture(ngfx::device_t* device, ngfx::texture_t* texture, const texture_desc_t& desc);
            bool                      Create(ngfx::device_t* device, ngfx::texture_t* texture);
            void                      Destroy(ngfx::device_t* device, ngfx::texture_t* texture);
            void*                     GetHandle(ngfx::device_t* device, ngfx::texture_t* texture);
            u32                       GetRequiredStagingBufferSize(ngfx::device_t* device, ngfx::texture_t* texture);
            u32                       GetRowPitch(ngfx::device_t* device, ngfx::texture_t* texture, u32 mip_level = 0);
            tiling_desc_t             GetTilingDesc(ngfx::device_t* device, ngfx::texture_t* texture);
            subresource_tiling_desc_t GetSubResourceTilingDesc(ngfx::device_t* device, ngfx::texture_t* texture, u32 subresource = 0);
            void*                     GetSharedHandle(ngfx::device_t* device, ngfx::texture_t* texture);
        }  // namespace nmetal
    }  // namespace ngfx
}  // namespace ncore
#endif
