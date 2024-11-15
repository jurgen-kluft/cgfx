#ifndef __CGFX_GFX_SWAPCHAIN_H__
#define __CGFX_GFX_SWAPCHAIN_H__

#include "cgfx/gfx_resource.h"

namespace ncore
{
    namespace ngfx
    {
        struct texture_t;

        struct swapchain_t
        {
            D_GFX_OCS_COMPONENT;
            swapchain_desc_t m_desc = {};
        };

        swapchain_t* CreateSwapchain(device_t* device, const swapchain_desc_t& desc, const char* name);
        void         Destroy(device_t* device, swapchain_t* resource);
        void         AcquireNextBackBuffer(swapchain_t* sc);
        texture_t*   GetBackBuffer(swapchain_t* sc);
        bool         Resize(swapchain_t* sc, u32 width, u32 height);
        void         SetVSyncEnabled(swapchain_t* sc, bool value);

    }  // namespace ngfx
}  // namespace ncore

#endif
