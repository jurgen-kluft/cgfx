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
            ngfx::swapchain_t* CreateSwapchain(device_t* device, ngfx::swapchain_t* swapchain, const swapchain_desc_t& desc);
            bool               Create(device_t* device, ngfx::swapchain_t* swapchain);
            void               Destroy(device_t* device, ngfx::swapchain_t* swapchain);
            void               Present(device_t* device, ngfx::swapchain_t* swapchain);
            void*              GetHandle(device_t* device, ngfx::swapchain_t* swapchain);
            void               AcquireNextBackBuffer(device_t* device, ngfx::swapchain_t* swapchain);
            ngfx::texture_t*   GetBackBuffer(device_t* device, ngfx::swapchain_t* swapchain);
            bool               Resize(device_t* device, ngfx::swapchain_t* swapchain, u32 width, u32 height);
            void               SetVSyncEnabled(device_t* device, ngfx::swapchain_t* swapchain, bool value);
        }  // namespace nmock

        // class MetalDevice;
        // class MetalTexture;

        // class MetalSwapchain : public swapchain_t
        // {
        // public:
        //     MetalSwapchain(MetalDevice* pDevice, const swapchain_desc_t& desc, const char* name);
        //     ~MetalSwapchain();

        //     bool           Create();
        //     MTL::Drawable* GetDrawable();

        //     virtual void*        GetHandle() const override { return m_pView; }
        //     virtual void         AcquireNextBackBuffer() override;
        //     virtual texture_t* GetBackBuffer() const override;
        //     virtual bool         Resize(u32 width, u32 height) override;
        //     virtual void         SetVSyncEnabled(bool value) override;

        // private:
        //     MTK::View*    m_pView    = nullptr;
        //     MetalTexture* m_pTexture = nullptr;
        // };

    }  // namespace ngfx
}  // namespace ncore
#endif
