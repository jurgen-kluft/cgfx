#ifndef __CGFX_D3D12_SWAPCHAIN_H__
#define __CGFX_D3D12_SWAPCHAIN_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
    #pragma once
#endif

#include "cgfx/d3d12/d3d12_header.h"
#include "cgfx/gfx_swapchain.h"

namespace ncore
{
    namespace ngfx
    {
        class D3D12Device;

        class D3D12Swapchain : public swapchain_t
        {
        public:
            D3D12Swapchain(D3D12Device* pDevice, const swapchain_desc_t& desc, const char* name);
            ~D3D12Swapchain();

            virtual void*        GetHandle() const override { return m_pSwapChain; }
            virtual void         AcquireNextBackBuffer() override;
            virtual texture_t* GetBackBuffer() const override;
            virtual bool         Resize(u32 width, u32 height) override;
            virtual void         SetVSyncEnabled(bool value) override { m_bEnableVsync = value; }

            bool Create();
            bool Present();

        private:
            bool CreateTextures();

        private:
            IDXGISwapChain3* m_pSwapChain = nullptr;

            template <typename T>
            struct vector_t
            {
                T* data = nullptr;
				u32 size = 0;
				u32 capacity = 0;
            };

            bool                        m_bEnableVsync       = true;
            bool                        m_bSupportTearing    = false;
            bool                        m_bWindowMode        = true;
            u32                         m_nCurrentBackBuffer = 0;
            vector_t<texture_t*> m_backBuffers;
        };
    }  // namespace ngfx
}  // namespace ncore
#endif
