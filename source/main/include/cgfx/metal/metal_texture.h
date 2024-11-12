#ifndef __CGFX_METAL_TEXTURE_H__
#define __CGFX_METAL_TEXTURE_H__

#include "cgfx/metal/metal_utils.h"
#include "cgfx/gfx_texture.h"

namespace ncore
{
    namespace ngfx
    {

        class MetalDevice;

        class MetalTexture : public IGfxTexture
        {
        public:
            MetalTexture(MetalDevice* pDevice, const GfxTextureDesc& desc, const char* name);
            ~MetalTexture();

            bool Create();
            void SetSwapchainTexture(MTL::Texture* texture);

            virtual void*                    GetHandle() const override { return m_pTexture; }
            virtual u32                      GetRequiredStagingBufferSize() const override;
            virtual u32                      GetRowPitch(u32 mip_level = 0) const override;
            virtual GfxTilingDesc            GetTilingDesc() const override;
            virtual GfxSubresourceTilingDesc GetTilingDesc(u32 subresource = 0) const override;
            virtual void*                    GetSharedHandle() const override;

        private:
            MTL::Texture* m_pTexture          = nullptr;
            bool          m_bSwapchainTexture = false;
        };

    }  // namespace ngfx
}  // namespace ncore
#endif
