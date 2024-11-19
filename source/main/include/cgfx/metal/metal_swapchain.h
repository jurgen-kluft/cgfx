#ifndef __CGFX_METAL_SWAPCHAIN_H__
#define __CGFX_METAL_SWAPCHAIN_H__

#include "cgfx/metal/metal_utils.h"
#include "cgfx/gfx_swapchain.h"

namespace ncore
{
    namespace ngfx
    {
        namespace nmetal
        {
            struct swapchain_t
            {
                D_GFX_OCS_COMPONENT_SET(enums::ComponentMetalSwapchain);
                MTK::View*       m_pView    = nullptr;
                ngfx::texture_t* m_pTexture = nullptr;
                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            ngfx::swapchain_t* CreateSwapchain(ngfx::device_t* device, ngfx::swapchain_t* swapchain, const swapchain_desc_t& desc);
            bool               Create(ngfx::device_t* device, ngfx::swapchain_t* swapchain);
            void               Destroy(ngfx::device_t* device, ngfx::swapchain_t* swapchain);
            MTK::View*         GetHandle(ngfx::device_t* device, ngfx::swapchain_t* swapchain);
            void               AcquireNextBackBuffer(ngfx::device_t* device, ngfx::swapchain_t* swapchain);
            ngfx::texture_t*   GetBackBuffer(ngfx::device_t* device, ngfx::swapchain_t* swapchain);
            bool               Resize(ngfx::device_t* device, ngfx::swapchain_t* swapchain, u32 width, u32 height);
            void               SetVSyncEnabled(ngfx::device_t* device, ngfx::swapchain_t* swapchain, bool value);
            MTL::Drawable*     GetDrawable(ngfx::device_t* device, ngfx::swapchain_t* swapchain);
        }  // namespace nmetal

    }  // namespace ngfx
}  // namespace ncore
#endif
