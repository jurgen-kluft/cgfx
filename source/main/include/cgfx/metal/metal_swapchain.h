#ifndef __CGFX_METAL_SWAPCHAIN_H__
#define __CGFX_METAL_SWAPCHAIN_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
    #pragma once
#endif

#include "cgfx/metal/metal_utils.h"
#include "cgfx/gfx_swapchain.h"

namespace ncore
{
    namespace ngfx
    {
#ifdef TARGET_MAC
        namespace nmetal
        {
            struct swapchain_t
            {
                D_GFX_OCS_COMPONENT_SET(enums::ComponentMetalSwapchain);
                MTK::View*       m_pView    = nullptr;
                ngfx::texture_t* m_pTexture = nullptr;
                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            void             CreateSwapchain(ngfx::device_t* device, ngfx::swapchain_t* swapchain);
            bool             Create(ngfx::device_t* device, ngfx::swapchain_t* swapchain);
            void             Destroy(ngfx::device_t* device, ngfx::swapchain_t* swapchain);
            MTK::View*       GetHandle(ngfx::device_t* device, ngfx::swapchain_t* swapchain);
            void             AcquireNextBackBuffer(ngfx::device_t* device, ngfx::swapchain_t* swapchain);
            ngfx::texture_t* GetBackBuffer(ngfx::device_t* device, ngfx::swapchain_t* swapchain);
            bool             Resize(ngfx::device_t* device, ngfx::swapchain_t* swapchain, u32 width, u32 height);
            void             SetVSyncEnabled(ngfx::device_t* device, ngfx::swapchain_t* swapchain, bool value);
            MTL::Drawable*   GetDrawable(ngfx::device_t* device, ngfx::swapchain_t* swapchain);
        }  // namespace nmetal
#else
        namespace nmetal
        {
            inline void             CreateSwapchain(ngfx::device_t* device, ngfx::swapchain_t* swapchain) {}
            inline bool             Create(ngfx::device_t* device, ngfx::swapchain_t* swapchain) { return false; }
            inline void             Destroy(ngfx::device_t* device, ngfx::swapchain_t* swapchain) {}
            inline void*            GetHandle(ngfx::device_t* device, ngfx::swapchain_t* swapchain) { return nullptr; }
            inline void             AcquireNextBackBuffer(ngfx::device_t* device, ngfx::swapchain_t* swapchain) {}
            inline ngfx::texture_t* GetBackBuffer(ngfx::device_t* device, ngfx::swapchain_t* swapchain) { return nullptr; }
            inline bool             Resize(ngfx::device_t* device, ngfx::swapchain_t* swapchain, u32 width, u32 height) { return false; }
            inline void             SetVSyncEnabled(ngfx::device_t* device, ngfx::swapchain_t* swapchain, bool value) {}
            inline void*            GetDrawable(ngfx::device_t* device, ngfx::swapchain_t* swapchain) { return nullptr; }
        }  // namespace nmetal
#endif
    }  // namespace ngfx
}  // namespace ncore

#endif
