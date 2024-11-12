#ifndef __CGFX_METAL_SWAPCHAIN_H__
#define __CGFX_METAL_SWAPCHAIN_H__

#include "cgfx/metal/metal_utils.h"
#include "cgfx/gfx_swapchain.h"

namespace ncore
{
    namespace ngfx
    {

        class MetalDevice;
        class MetalTexture;

        class MetalSwapchain : public IGfxSwapchain
        {
        public:
            MetalSwapchain(MetalDevice* pDevice, const GfxSwapchainDesc& desc, const char* name);
            ~MetalSwapchain();

            bool           Create();
            MTL::Drawable* GetDrawable();

            virtual void*        GetHandle() const override { return m_pView; }
            virtual void         AcquireNextBackBuffer() override;
            virtual IGfxTexture* GetBackBuffer() const override;
            virtual bool         Resize(u32 width, u32 height) override;
            virtual void         SetVSyncEnabled(bool value) override;

        private:
            MTK::View*    m_pView    = nullptr;
            MetalTexture* m_pTexture = nullptr;
        };

    }  // namespace ngfx
}  // namespace ncore
#endif
