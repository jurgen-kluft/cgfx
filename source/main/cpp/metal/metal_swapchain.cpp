#include "cgfx/metal/metal_swapchain.h"
#include "cgfx/metal/metal_device.h"
#include "cgfx/metal/metal_texture.h"

namespace ncore
{
    namespace ngfx
    {

        MetalSwapchain::MetalSwapchain(MetalDevice* pDevice, const GfxSwapchainDesc& desc, const eastl::string& name)
        {
            m_pDevice = pDevice;
            m_desc    = desc;
            m_name    = name;
        }

        MetalSwapchain::~MetalSwapchain()
        {
            delete m_pTexture;
            m_pView->release();
        }

        bool MetalSwapchain::Create()
        {
            m_pView = (MTK::View*)m_desc.window_handle;
            m_pView->setDevice((MTL::Device*)m_pDevice->GetHandle());
            m_pView->setColorPixelFormat(ToPixelFormat(m_desc.backbuffer_format));
            m_pView->retain();

            GfxTextureDesc textureDesc;
            textureDesc.width  = m_desc.width;
            textureDesc.height = m_desc.height;
            textureDesc.format = m_desc.backbuffer_format;
            textureDesc.usage  = GfxTextureUsageRenderTarget;

            m_pTexture = new MetalTexture((MetalDevice*)m_pDevice, textureDesc, m_name);

            return true;
        }

        MTL::Drawable* MetalSwapchain::GetDrawable() { return m_pView->currentDrawable(); }

        void MetalSwapchain::AcquireNextBackBuffer()
        {
            CA::MetalDrawable* drawable = m_pView->currentDrawable();  // this invokes CAMetalLayer::nextDrawable
            MTL::Texture*      texture  = drawable->texture();

            m_pTexture->SetSwapchainTexture(texture);
        }

        IGfxTexture* MetalSwapchain::GetBackBuffer() const { return m_pTexture; }

        bool MetalSwapchain::Resize(u32 width, u32 height)
        {
            if (m_desc.width == width && m_desc.height == height)
            {
                return false;
            }

            m_desc.width  = width;
            m_desc.height = height;

            return true;
        }

        void MetalSwapchain::SetVSyncEnabled(bool value) {}

    }  // namespace ngfx
}  // namespace ncore
