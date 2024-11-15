#ifndef __CGFX_MOCK_SWAPCHAIN_H__
#define __CGFX_MOCK_SWAPCHAIN_H__

#include "cgfx/gfx_swapchain.h"

namespace ncore
{
    namespace ngfx
    {
        namespace nmock
        {
            ngfx::swapchain_t* Alloc(device_t* pDevice, const swapchain_desc_t& desc, const char* name);
            void               Destroy(ngfx::swapchain_t* pSwapchain);
            bool               Create(ngfx::swapchain_t* pSwapchain);
            void               Present(ngfx::swapchain_t* pSwapchain);
            void*              GetHandle(ngfx::swapchain_t* pSwapchain);
            void               AcquireNextBackBuffer(ngfx::swapchain_t* pSwapchain);
            ngfx::texture_t*   GetBackBuffer(ngfx::swapchain_t* pSwapchain);
            bool               Resize(ngfx::swapchain_t* pSwapchain, u32 width, u32 height);
            void               SetVSyncEnabled(ngfx::swapchain_t* pSwapchain, bool value);

        }  // namespace nmock

    }  // namespace ngfx
}  // namespace ncore
#endif
