#include "cgfx/metal/metal_swapchain.h"
#include "cgfx/metal/metal_device.h"
#include "cgfx/metal/metal_texture.h"
#include "cgfx/metal/metal_utils.h"

namespace ncore
{
    namespace ngfx
    {
        namespace nmetal
        {
            struct swapchain_t
            {
                MTK::View*       m_pView    = nullptr;
                ngfx::texture_t* m_pTexture = nullptr;
            };

            ngfx::swapchain_t* CreateSwapchain(ngfx::device_t* device, ngfx::swapchain_t* swapchain, const swapchain_desc_t& desc)
            {
                nmetal::swapchain_t* msc = AddAnotherComponent<ngfx::swapchain_t, nmetal::swapchain_t>(device, swapchain);

                return swapchain;
            }

            bool Create(ngfx::device_t* device, ngfx::swapchain_t* swapchain)
            {
                nmetal::swapchain_t* msc = GetOtherComponent<ngfx::swapchain_t, nmetal::swapchain_t>(device, swapchain);
                nmetal::device_t*    md  = GetOtherComponent<ngfx::device_t, nmetal::device_t>(device, device);

                msc->m_pView = (MTK::View*)swapchain->m_desc.window_handle;
                msc->m_pView->setDevice(md->m_pDevice);
                msc->m_pView->setColorPixelFormat(ToPixelFormat(swapchain->m_desc.backbuffer_format));
                msc->m_pView->retain();

                texture_desc_t textureDesc;
                textureDesc.width  = swapchain->m_desc.width;
                textureDesc.height = swapchain->m_desc.height;
                textureDesc.format = swapchain->m_desc.backbuffer_format;
                textureDesc.usage  = enums::TextureUsageRenderTarget;

                // msc->m_pTexture = new MetalTexture((MetalDevice*)m_pDevice, textureDesc, m_name);
                msc->m_pTexture = ngfx::CreateTexture(device, textureDesc, "MetalTexture");
            }

            void Destroy(ngfx::device_t* device, ngfx::swapchain_t* swapchain)
            {
                nmetal::swapchain_t* msc = GetOtherComponent<ngfx::swapchain_t, nmetal::swapchain_t>(device, swapchain);
                ngfx::Destroy(device, msc->m_pTexture);
                msc->m_pView->release();
            }

            void*            GetHandle(ngfx::device_t* device, ngfx::swapchain_t* swapchain);
            void             AcquireNextBackBuffer(ngfx::device_t* device, ngfx::swapchain_t* swapchain);
            ngfx::texture_t* GetBackBuffer(ngfx::device_t* device, ngfx::swapchain_t* swapchain);
            bool             Resize(ngfx::device_t* device, ngfx::swapchain_t* swapchain, u32 width, u32 height);
            void             SetVSyncEnabled(ngfx::device_t* device, ngfx::swapchain_t* swapchain, bool value);
        }  // namespace nmetal

        // MTL::Drawable* MetalSwapchain::GetDrawable() { return m_pView->currentDrawable(); }

        // void MetalSwapchain::AcquireNextBackBuffer()
        // {
        //     CA::MetalDrawable* drawable = m_pView->currentDrawable();  // this invokes CAMetalLayer::nextDrawable
        //     MTL::Texture*      texture  = drawable->texture();

        //     m_pTexture->SetSwapchainTexture(texture);
        // }

        // texture_t* MetalSwapchain::GetBackBuffer() const { return m_pTexture; }

        // bool MetalSwapchain::Resize(u32 width, u32 height)
        // {
        //     if (m_desc.width == width && m_desc.height == height)
        //     {
        //         return false;
        //     }

        //     m_desc.width  = width;
        //     m_desc.height = height;

        //     return true;
        // }

        // void MetalSwapchain::SetVSyncEnabled(bool value) {}

    }  // namespace ngfx
}  // namespace ncore
