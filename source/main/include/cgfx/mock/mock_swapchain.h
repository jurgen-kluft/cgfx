#ifndef __CGFX_MOCK_SWAPCHAIN_H__
#define __CGFX_MOCK_SWAPCHAIN_H__

#include "cgfx/gfx_swapchain.h"

namespace ncore
{
    namespace ngfx
    {

        class MockDevice;

        class MockSwapchain : public IGfxSwapchain
        {
        public:
            MockSwapchain(MockDevice* pDevice, const GfxSwapchainDesc& desc, const char* name);
            ~MockSwapchain();

            bool Create();
            void Present();

            virtual void*        GetHandle() const override;
            virtual void         AcquireNextBackBuffer() override;
            virtual IGfxTexture* GetBackBuffer() const override;
            virtual bool         Resize(u32 width, u32 height) override;
            virtual void         SetVSyncEnabled(bool value) override;

        private:
            bool CreateTextures();

        private:
            s32          m_currentBackBuffer  = -1;
            s32          m_maxBackBufferCount = 8;
            s32          m_numBackBufferCount = 0;
            IGfxTexture* m_backBuffers[8];
        };

    }  // namespace ngfx
}  // namespace ncore
#endif
