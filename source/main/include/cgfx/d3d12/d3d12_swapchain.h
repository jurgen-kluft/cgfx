#ifndef __CGFX_D3D12_SWAPCHAIN_H__
#define __CGFX_D3D12_SWAPCHAIN_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
    #pragma once
#endif

#include "cbase/c_carray.h"
#include "cgfx/gfx_swapchain.h"
#include "cgfx/d3d12/d3d12_header.h"

namespace ncore
{
    namespace ngfx
    {
#ifdef TARGET_PC
        namespace nd3d12
        {
            struct swapchain_t
            {
                D_GFX_CS_COMPONENT_SET(enums::ComponentD3D12Swapchain);
                IDXGISwapChain3*     m_pSwapChain         = nullptr;
                bool                 m_bEnableVsync       = true;
                bool                 m_bSupportTearing    = false;
                bool                 m_bWindowMode        = true;
                u32                  m_nCurrentBackBuffer = 0;
                carray_t<ngfx::texture_t*> m_backBuffers;
                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            void             CreateSwapchain(ngfx::device_t* device, ngfx::swapchain_t* swapchain);
            void             DestroySwapchain(ngfx::device_t* device, ngfx::swapchain_t* swapchain);
            bool             Create(ngfx::device_t* device, ngfx::swapchain_t* swapchain);
            void             Destroy(ngfx::device_t* device, ngfx::swapchain_t* swapchain);
            bool             Present(ngfx::device_t* device, ngfx::swapchain_t* swapchain);
            void             AcquireNextBackBuffer(ngfx::device_t* device, ngfx::swapchain_t* swapchain);
            ngfx::texture_t* GetBackBuffer(ngfx::device_t* device, ngfx::swapchain_t* swapchain);
            bool             Resize(ngfx::device_t* device, ngfx::swapchain_t* swapchain, u32 width, u32 height);
            void             SetVSyncEnabled(ngfx::device_t* device, ngfx::swapchain_t* swapchain, bool value);
        }  // namespace nd3d12
#else

        namespace nd3d12
        {
            inline void             CreateSwapchain(ngfx::device_t* device, ngfx::swapchain_t* swapchain) {}
            inline void             DestroySwapchain(ngfx::device_t* device, ngfx::swapchain_t* swapchain) {}
            inline bool             Create(ngfx::device_t* device, ngfx::swapchain_t* swapchain) { return false; }
            inline void             Destroy(ngfx::device_t* device, ngfx::swapchain_t* swapchain) {}
            inline void             Present(ngfx::device_t* device, ngfx::swapchain_t* swapchain) {}
            inline void             AcquireNextBackBuffer(ngfx::device_t* device, ngfx::swapchain_t* swapchain) {}
            inline ngfx::texture_t* GetBackBuffer(ngfx::device_t* device, ngfx::swapchain_t* swapchain) { return nullptr; }
            inline bool             Resize(ngfx::device_t* device, ngfx::swapchain_t* swapchain, u32 width, u32 height) { return false; }
            inline void             SetVSyncEnabled(ngfx::device_t* device, ngfx::swapchain_t* swapchain, bool value) {}
        }  // namespace nd3d12

#endif
    }  // namespace ngfx
}  // namespace ncore
#endif
