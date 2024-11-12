#ifndef __CGFX_MOCK_TEXTURE_H__
#define __CGFX_MOCK_TEXTURE_H__

#include "cgfx/gfx_texture.h"

namespace ncore
{
    namespace ngfx
    {
        class MockDevice;

        class MockTexture : public IGfxTexture
        {
        public:
            MockTexture(MockDevice* pDevice, const GfxTextureDesc& desc, const char* name);
            ~MockTexture();

            bool Create();

            virtual void*                    GetHandle() const override;
            virtual u32                      GetRequiredStagingBufferSize() const override;
            virtual u32                      GetRowPitch(u32 mip_level = 0) const override;
            virtual GfxTilingDesc            GetTilingDesc() const override;
            virtual GfxSubresourceTilingDesc GetTilingDesc(u32 subresource = 0) const override;
            virtual void*                    GetSharedHandle() const override;
        };

    }  // namespace ngfx
}  // namespace ncore
#endif
