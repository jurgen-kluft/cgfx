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

            ngfx::swapchain_t* Alloc(device_t* pDevice, const swapchain_desc_t& desc, const char* name)
            {
                // m_pDevice = pDevice;
                // m_desc    = desc;
                // m_name    = name;
                ngfx::swapchain_t* sc = mock::CreateSwapchain(pDevice, desc, name);

                mock::swapchain_t* msc    = ngfx::AddComponent<ngfx::swapchain_t, mock::swapchain_t>(pDevice, sc);
                msc->m_currentBackBuffer  = -1;
                msc->m_maxBackBufferCount = 8;
                msc->m_numBackBufferCount = 0;
                for (s32 i = 0; i < msc->m_maxBackBufferCount; ++i)
                {
                    msc->m_backBuffers[i] = nullptr;
                }
            }

            void Destroy(ngfx::swapchain_t* pSwapchain)
            {
                mock::swapchain_t* msc = ngfx::AddComponent<ngfx::swapchain_t, mock::swapchain_t>(pSwapchain->m_device, pSwapchain);
                for (s32 i = 0; i < msc->m_numBackBufferCount; ++i)
                {
                    Destroy(msc->m_backBuffers[i]);
                }
                msc->m_numBackBufferCount = 0;
            }

            bool CreateTextures(ngfx::swapchain_t* sc, mock::swapchain_t* msc)
            {
                texture_desc_t textureDesc;
                textureDesc.width  = sc->m_desc.width;
                textureDesc.height = sc->m_desc.height;
                textureDesc.format = sc->m_desc.backbuffer_format;
                textureDesc.usage  = enums::TextureUsageRenderTarget;

                for (u32 i = 0; i < sc->m_desc.backbuffer_count; ++i)
                {
                    const char* name                                = "back-buffer";  // = fmt::format("{} texture {}", m_name, i).c_str();
                    msc->m_backBuffers[msc->m_numBackBufferCount++] = Alloc(sc->m_device, textureDesc, name);
                }

                return true;
            }

            bool Create(ngfx::swapchain_t* sc)
            {
                mock::swapchain_t* msc = ngfx::GetComponent<ngfx::swapchain_t, mock::swapchain_t>(sc->m_device, sc);
                return CreateTextures(sc, msc);
            }

            void Present(ngfx::swapchain_t* sc) {}

            void* GetHandle(ngfx::swapchain_t* sc) { return nullptr; }

            void AcquireNextBackBuffer(ngfx::swapchain_t* sc)
            {
                mock::swapchain_t* msc   = ngfx::GetComponent<ngfx::swapchain_t, mock::swapchain_t>(sc->m_device, sc);
                msc->m_currentBackBuffer = (msc->m_currentBackBuffer + 1) % sc->m_desc.backbuffer_count;
            }

            texture_t* GetBackBuffer(ngfx::swapchain_t* sc)
            {
                mock::swapchain_t* msc = ngfx::GetComponent<ngfx::swapchain_t, mock::swapchain_t>(sc->m_device, sc);
                return msc->m_backBuffers[msc->m_currentBackBuffer];
            }

            bool Resize(ngfx::swapchain_t* sc, u32 width, u32 height)
            {
                if (sc->m_desc.width == width && sc->m_desc.height == height)
                {
                    return false;
                }

                sc->m_desc.width  = width;
                sc->m_desc.height = height;

                mock::swapchain_t* msc   = ngfx::GetComponent<ngfx::swapchain_t, mock::swapchain_t>(sc->m_device, sc);
                msc->m_currentBackBuffer = 0;

                for (s32 i = 0; i < msc->m_numBackBufferCount; ++i)
                {
                    Destroy(msc->m_backBuffers[i]);
                }
                msc->m_numBackBufferCount = 0;
                return CreateTextures(sc, msc);
            }

            void SetVSyncEnabled(ngfx::swapchain_t* sc, bool value) {}

        }  // namespace nmock
    }  // namespace ngfx
}  // namespace ncore
