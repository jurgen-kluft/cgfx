#include "cgfx/mock/mock_swapchain.h"
#include "cgfx/mock/mock_device.h"
#include "cgfx/mock/mock_texture.h"
// #include "utils/fmt.h"

namespace ncore
{
    namespace ngfx
    {

        MockSwapchain::MockSwapchain(MockDevice* pDevice, const GfxSwapchainDesc& desc, const char* name)
        {
            m_pDevice = pDevice;
            m_desc    = desc;
            m_name    = name;
        }

        MockSwapchain::~MockSwapchain()
        {
            for (s32 i = 0; i < m_numBackBufferCount; ++i)
            {
                delete m_backBuffers[i];
            }
            m_numBackBufferCount = 0;
        }

        bool MockSwapchain::Create() { return CreateTextures(); }

        void MockSwapchain::Present() {}

        void* MockSwapchain::GetHandle() const { return nullptr; }

        void MockSwapchain::AcquireNextBackBuffer() { m_currentBackBuffer = (m_currentBackBuffer + 1) % m_desc.backbuffer_count; }

        IGfxTexture* MockSwapchain::GetBackBuffer() const { return m_backBuffers[m_currentBackBuffer]; }

        bool MockSwapchain::Resize(u32 width, u32 height)
        {
            if (m_desc.width == width && m_desc.height == height)
            {
                return false;
            }

            m_desc.width        = width;
            m_desc.height       = height;
            m_currentBackBuffer = 0;

            for (s32 i = 0; i < m_numBackBufferCount; ++i)
            {
                delete m_backBuffers[i];
            }
            m_numBackBufferCount = 0;

            return CreateTextures();
        }

        void MockSwapchain::SetVSyncEnabled(bool value) {}

        bool MockSwapchain::CreateTextures()
        {
            GfxTextureDesc textureDesc;
            textureDesc.width  = m_desc.width;
            textureDesc.height = m_desc.height;
            textureDesc.format = m_desc.backbuffer_format;
            textureDesc.usage  = GfxTextureUsage::RenderTarget;

            for (u32 i = 0; i < m_desc.backbuffer_count; ++i)
            {
                const char* name = "back-buffer";  // = fmt::format("{} texture {}", m_name, i).c_str();
                MockTexture*  texture                 = new MockTexture((MockDevice*)m_pDevice, textureDesc, name);
                m_backBuffers[m_numBackBufferCount++] = texture;
            }

            return true;
        }

    }  // namespace ngfx
}  // namespace ncore
