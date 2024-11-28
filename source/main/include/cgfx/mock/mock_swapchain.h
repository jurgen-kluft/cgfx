#ifndef __CGFX_MOCK_SWAPCHAIN_H__
#define __CGFX_MOCK_SWAPCHAIN_H__

#include "cgfx/gfx_swapchain.h"

namespace ncore
{
    namespace ngfx
    {
        namespace nmock
        {
            struct swapchain_t
            {
                D_GFX_CS_COMPONENT_SET(enums::ComponentMockSwapchain);
                s32        m_currentBackBuffer  = -1;
                s32        m_maxBackBufferCount = 8;
                s32        m_numBackBufferCount = 0;
                texture_t* m_backBuffers[8];
                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            void                CreateSwapchain(device_t* device, ngfx::swapchain_t* swapchain, const swapchain_desc_t& desc);
            bool               Create(device_t* device, ngfx::swapchain_t* swapchain);
            void               Destroy(device_t* device, ngfx::swapchain_t* swapchain);
            void               Present(device_t* device, ngfx::swapchain_t* swapchain);
            void*              GetHandle(device_t* device, ngfx::swapchain_t* swapchain);
            void               AcquireNextBackBuffer(device_t* device, ngfx::swapchain_t* swapchain);
            ngfx::texture_t*   GetBackBuffer(device_t* device, ngfx::swapchain_t* swapchain);
            bool               Resize(device_t* device, ngfx::swapchain_t* swapchain, u32 width, u32 height);
            void               SetVSyncEnabled(device_t* device, ngfx::swapchain_t* swapchain, bool value);
        }  // namespace nmock
    }  // namespace ngfx
}  // namespace ncore
#endif
