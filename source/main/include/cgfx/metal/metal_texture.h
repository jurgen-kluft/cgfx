#ifndef __CGFX_METAL_TEXTURE_H__
#define __CGFX_METAL_TEXTURE_H__

//#include "cgfx/metal/metal_utils.h"
#include "cgfx/gfx_texture.h"

namespace ncore
{
    namespace ngfx
    {
        namespace nmetal
        {
            ngfx::texture_t*         CreateTexture(device_t* device, ngfx::texture_t* texture, const texture_desc_t& desc);
            bool                     Create(device_t* device, ngfx::texture_t* texture);
            void                     Destroy(device_t* device, ngfx::texture_t* texture);
            void*                    GetHandle(device_t* device, ngfx::texture_t* texture);
            u32                      GetRequiredStagingBufferSize(device_t* device, ngfx::texture_t* texture);
            u32                      GetRowPitch(device_t* device, ngfx::texture_t* texture, u32 mip_level = 0);
            GfxTilingDesc            GetTilingDesc(device_t* device, ngfx::texture_t* texture);
            GfxSubresourceTilingDesc GetSubResourceTilingDesc(device_t* device, ngfx::texture_t* texture, u32 subresource = 0);
            void*                    GetSharedHandle(device_t* device, ngfx::texture_t* texture);
        }  // namespace nmock

        // class MetalDevice;

        // class MetalTexture : public texture_t
        // {
        // public:
        //     MetalTexture(MetalDevice* pDevice, const texture_desc_t& desc, const char* name);
        //     ~MetalTexture();

        //     bool Create();
        //     void SetSwapchainTexture(MTL::Texture* texture);

        //     virtual void*                    GetHandle() const override { return m_pTexture; }
        //     virtual u32                      GetRequiredStagingBufferSize() const override;
        //     virtual u32                      GetRowPitch(u32 mip_level = 0) const override;
        //     virtual GfxTilingDesc            GetTilingDesc() const override;
        //     virtual GfxSubresourceTilingDesc GetTilingDesc(u32 subresource = 0) const override;
        //     virtual void*                    GetSharedHandle() const override;

        // private:
        //     MTL::Texture* m_pTexture          = nullptr;
        //     bool          m_bSwapchainTexture = false;
        // };

    }  // namespace ngfx
}  // namespace ncore
#endif
