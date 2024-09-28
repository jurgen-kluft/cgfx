#ifndef __CGFX_GFX_SWAPCHAIN_H__
#define __CGFX_GFX_SWAPCHAIN_H__

#include "cgfx/gfx_resource.h"

namespace ncore
{
    namespace ngfx
    {

        class IGfxTexture;

        class IGfxSwapchain : public IGfxResource
        {
        public:
            virtual ~IGfxSwapchain() {}

            virtual void         AcquireNextBackBuffer()       = 0;
            virtual IGfxTexture* GetBackBuffer() const         = 0;
            virtual bool         Resize(u32 width, u32 height) = 0;
            virtual void         SetVSyncEnabled(bool value)   = 0;

            const GfxSwapchainDesc& GetDesc() const { return m_desc; }

        protected:
            GfxSwapchainDesc m_desc = {};
        };

    }  // namespace ngfx
}  // namespace ncore

#endif
