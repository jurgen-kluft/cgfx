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
            ngfx::swapchain_t* CreateSwapchain(ngfx::device_t* device, ngfx::swapchain_t* swapchain, const swapchain_desc_t& desc)
            {
                nmetal::swapchain_t* msc = CreateComponent<ngfx::swapchain_t, nmetal::swapchain_t>(device, swapchain);

                return swapchain;
            }

            bool Create(ngfx::device_t* device, ngfx::swapchain_t* swapchain)
            {
                nmetal::swapchain_t* msc = GetComponent<ngfx::swapchain_t, nmetal::swapchain_t>(device, swapchain);
                nmetal::device_t*    md  = GetComponent<ngfx::device_t, nmetal::device_t>(device, device);

                msc->m_pView = (MTK::View*)swapchain->m_desc.window_handle;
                msc->m_pView->setDevice(md->m_pDevice);
                msc->m_pView->setColorPixelFormat(ToPixelFormat(swapchain->m_desc.backbuffer_format));
                msc->m_pView->retain();

                texture_desc_t textureDesc;
                textureDesc.width  = swapchain->m_desc.width;
                textureDesc.height = swapchain->m_desc.height;
                textureDesc.format = swapchain->m_desc.backbuffer_format;
                textureDesc.usage  = enums::TextureUsageRenderTarget;

                msc->m_pTexture = ngfx::CreateTexture(device, textureDesc, "MetalTexture");
            }

            void Destroy(ngfx::device_t* device, ngfx::swapchain_t* swapchain)
            {
                nmetal::swapchain_t* msc = GetComponent<ngfx::swapchain_t, nmetal::swapchain_t>(device, swapchain);
                ngfx::Destroy(device, msc->m_pTexture);
                msc->m_pView->release();
            }

            MTK::View* GetHandle(ngfx::device_t* device, ngfx::swapchain_t* swapchain)
            {
                nmetal::swapchain_t* msc = GetComponent<ngfx::swapchain_t, nmetal::swapchain_t>(device, swapchain);
                return msc->m_pView;
            }

            // Metal SPECIFIC
            void SetSwapchainTexture(ngfx::device_t* device, ngfx::texture_t* texture, MTL::Texture* mtlTexture)
            {
                nmetal::mtexture_t* mtexture = GetComponent<ngfx::texture_t, nmetal::mtexture_t>(device, texture);

                mtexture->m_pTexture          = mtlTexture;
                mtexture->m_bSwapchainTexture = true;

                texture->m_desc.width  = (u32)mtlTexture->width();
                texture->m_desc.height = (u32)mtlTexture->height();
            }

            void AcquireNextBackBuffer(ngfx::device_t* device, ngfx::swapchain_t* swapchain)
            {
                nmetal::swapchain_t* msc        = GetComponent<ngfx::swapchain_t, nmetal::swapchain_t>(device, swapchain);
                CA::MetalDrawable*   drawable   = msc->m_pView->currentDrawable();  // this invokes CAMetalLayer::nextDrawable
                MTL::Texture*        mtltexture = drawable->texture();
                SetSwapchainTexture(device, msc->m_pTexture, mtltexture);
            }

            ngfx::texture_t* GetBackBuffer(ngfx::device_t* device, ngfx::swapchain_t* swapchain)
            {
                nmetal::swapchain_t* msc = GetComponent<ngfx::swapchain_t, nmetal::swapchain_t>(device, swapchain);
                return msc->m_pTexture;
            }

            bool Resize(ngfx::device_t* device, ngfx::swapchain_t* swapchain, u32 width, u32 height)
            {
                if (swapchain->m_desc.width == width && swapchain->m_desc.height == height)
                {
                    return false;
                }

                swapchain->m_desc.width  = width;
                swapchain->m_desc.height = height;

                return true;
            }

            void SetVSyncEnabled(ngfx::device_t* device, ngfx::swapchain_t* swapchain, bool value)
            {
                nmetal::swapchain_t* msc = GetComponent<ngfx::swapchain_t, nmetal::swapchain_t>(device, swapchain);
                // msc->m_pView->setPreferredFramesPerSecond(value ? 60 : 0);
            }

            MTL::Drawable* GetDrawable(ngfx::device_t* device, ngfx::swapchain_t* swapchain)
            {
                nmetal::swapchain_t* msc = GetComponent<ngfx::swapchain_t, nmetal::swapchain_t>(device, swapchain);
                return msc->m_pView->currentDrawable();
            }

        }  // namespace nmetal
    }  // namespace ngfx
}  // namespace ncore
