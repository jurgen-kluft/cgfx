#ifndef __CGFX_METAL_SWAPCHAIN_H__
#define __CGFX_METAL_SWAPCHAIN_H__

#include "cgfx/gfx_swapchain.h"

namespace ncore
{
    namespace ngfx
    {
        namespace nmetal
        {
            struct swapchain_t
            {
                D_GFX_OCS_COMPONENT;
                MTK::View*       m_pView    = nullptr;
                ngfx::texture_t* m_pTexture = nullptr;
            };

            ngfx::swapchain_t* CreateSwapchain(device_t* device, ngfx::swapchain_t* swapchain, const swapchain_desc_t& desc);
            bool               Create(device_t* device, ngfx::swapchain_t* swapchain);
            void               Destroy(device_t* device, ngfx::swapchain_t* swapchain);
            void*              GetHandle(device_t* device, ngfx::swapchain_t* swapchain);
            void               AcquireNextBackBuffer(device_t* device, ngfx::swapchain_t* swapchain);
            ngfx::texture_t*   GetBackBuffer(device_t* device, ngfx::swapchain_t* swapchain);
            bool               Resize(device_t* device, ngfx::swapchain_t* swapchain, u32 width, u32 height);
            void               SetVSyncEnabled(device_t* device, ngfx::swapchain_t* swapchain, bool value);
        }  // namespace nmetal

    }  // namespace ngfx
}  // namespace ncore
#endif
