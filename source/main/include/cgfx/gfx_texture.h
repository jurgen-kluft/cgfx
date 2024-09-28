#ifndef __CGFX_GFX_TEXTURE_H__
#define __CGFX_GFX_TEXTURE_H__

#include "cgfx/gfx_resource.h"

namespace ncore
{
    namespace ngfx
    {

        class IGfxTexture : public IGfxResource
        {
        public:
            const GfxTextureDesc& GetDesc() const { return m_desc; }

            virtual bool                     IsTexture() const { return true; }
            virtual u32                      GetRequiredStagingBufferSize() const     = 0;
            virtual u32                      GetRowPitch(u32 mip_level = 0) const     = 0;
            virtual GfxTilingDesc            GetTilingDesc() const                    = 0;
            virtual GfxSubresourceTilingDesc GetTilingDesc(u32 subresource = 0) const = 0;
            virtual void*                    GetSharedHandle() const                  = 0;

        protected:
            GfxTextureDesc m_desc = {};
        };

    }  // namespace ngfx
}  // namespace ncore

#endif
