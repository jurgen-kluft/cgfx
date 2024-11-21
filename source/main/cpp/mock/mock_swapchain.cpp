#include "cgfx/mock/mock_swapchain.h"
#include "cgfx/mock/mock_device.h"
#include "cgfx/mock/mock_texture.h"
// #include "utils/fmt.h"

namespace ncore
{
    namespace ngfx
    {
        namespace nmock
        {
            struct swapchain_t
            {
                s32        m_currentBackBuffer  = -1;
                s32        m_maxBackBufferCount = 8;
                s32        m_numBackBufferCount = 0;
                texture_t* m_backBuffers[8];
            };

            ngfx::swapchain_t* CreateSwapchain(device_t* device, ngfx::swapchain_t* swapchain, const swapchain_desc_t& desc)
            {
                nmock::swapchain_t* msc   = ngfx::CreateComponent<ngfx::swapchain_t, nmock::swapchain_t>(device, swapchain);
                msc->m_currentBackBuffer  = -1;
                msc->m_maxBackBufferCount = 8;
                msc->m_numBackBufferCount = 0;
                for (s32 i = 0; i < msc->m_maxBackBufferCount; ++i)
                {
                    msc->m_backBuffers[i] = nullptr;
                }
            }

            bool Create(device_t* device, ngfx::swapchain_t* sc)
            {
                nmock::swapchain_t* msc = ngfx::GetComponent<ngfx::swapchain_t, nmock::swapchain_t>(device, sc);
                return CreateTextures(device, sc, msc);
            }

            void Destroy(device_t* device, ngfx::swapchain_t* sc)
            {
                nmock::swapchain_t* msc = ngfx::CreateComponent<ngfx::swapchain_t, nmock::swapchain_t>(device, sc);
                for (s32 i = 0; i < msc->m_numBackBufferCount; ++i)
                {
                    ngfx::Destroy(device, msc->m_backBuffers[i]);
                }
                msc->m_numBackBufferCount = 0;
            }

            bool CreateTextures(device_t* device, ngfx::swapchain_t* sc, nmock::swapchain_t* msc)
            {
                texture_desc_t textureDesc;
                textureDesc.width  = sc->m_desc.width;
                textureDesc.height = sc->m_desc.height;
                textureDesc.format = sc->m_desc.backbuffer_format;
                textureDesc.usage  = enums::TextureUsageRenderTarget;

                for (u32 i = 0; i < sc->m_desc.backbuffer_count; ++i)
                {
                    const char* name                                = "back-buffer";  // = fmt::format("{} texture {}", m_name, i).c_str();
                    msc->m_backBuffers[msc->m_numBackBufferCount++] = CreateTexture(device, textureDesc, name);
                }

                return true;
            }

            void Present(device_t* device, ngfx::swapchain_t* sc) {}

            void* GetHandle(device_t* device, ngfx::swapchain_t* sc) { return nullptr; }

            void AcquireNextBackBuffer(device_t* device, ngfx::swapchain_t* sc)
            {
                nmock::swapchain_t* msc  = ngfx::GetComponent<ngfx::swapchain_t, nmock::swapchain_t>(device, sc);
                msc->m_currentBackBuffer = (msc->m_currentBackBuffer + 1) % sc->m_desc.backbuffer_count;
            }

            texture_t* GetBackBuffer(device_t* device, ngfx::swapchain_t* sc)
            {
                nmock::swapchain_t* msc = ngfx::GetComponent<ngfx::swapchain_t, nmock::swapchain_t>(device, sc);
                return msc->m_backBuffers[msc->m_currentBackBuffer];
            }

            bool Resize(device_t* device, ngfx::swapchain_t* sc, u32 width, u32 height)
            {
                if (sc->m_desc.width == width && sc->m_desc.height == height)
                {
                    return false;
                }

                sc->m_desc.width  = width;
                sc->m_desc.height = height;

                nmock::swapchain_t* msc  = ngfx::GetComponent<ngfx::swapchain_t, nmock::swapchain_t>(device, sc);
                msc->m_currentBackBuffer = 0;

                for (s32 i = 0; i < msc->m_numBackBufferCount; ++i)
                {
                    ngfx::Destroy(device, msc->m_backBuffers[i]);
                }
                msc->m_numBackBufferCount = 0;
                return CreateTextures(device, sc, msc);
            }

            void SetVSyncEnabled(device_t* device, ngfx::swapchain_t* sc, bool value) {}

        }  // namespace nmock
    }  // namespace ngfx
}  // namespace ncore
