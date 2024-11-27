#include "cbase/c_debug.h"
#include "cgfx/gfx_defines.h"
#include "cgfx/gfx_texture.h"
#include "cgfx/d3d12/d3d12_swapchain.h"
#include "cgfx/d3d12/d3d12_device.h"
#include "cgfx/d3d12/d3d12_texture.h"

namespace ncore
{
    namespace ngfx
    {
#ifdef TARGET_PC
        namespace nd3d12
        {
            bool CreateTextures(ngfx::device_t* device, nd3d12::swapchain_t* dxswapchain, const swapchain_desc_t& desc)
            {
                texture_desc_t textureDesc;
                textureDesc.width  = desc.width;
                textureDesc.height = desc.height;
                textureDesc.format = desc.backbuffer_format;
                textureDesc.usage  = enums::TextureUsageRenderTarget;

                const char* name = "backbuffer";
                for (u32 i = 0; i < dxswapchain->m_desc.backbuffer_count; ++i)
                {
                    ID3D12Resource* pBackbuffer = NULL;
                    HRESULT         hr          = dxswapchain->m_pSwapChain->GetBuffer(i, IID_PPV_ARGS(&pBackbuffer));
                    if (FAILED(hr))
                    {
                        return false;
                    }
                    ngfx::texture_t*   texture         = ngfx::CreateTexture(device, textureDesc, name);
                    nd3d12::texture_t* dxtexture       = GetComponent<ngfx::texture_t, nd3d12::texture_t>(device, texture);
                    dxswapchain->m_backBuffers.data[i] = texture;
                    dxtexture->m_pTexture              = pBackbuffer;
                }
                dxswapchain->m_backBuffers.size = dxswapchain->m_desc.backbuffer_count;

                return true;
            }

            void CreateSwapchain(ngfx::device_t* device, ngfx::swapchain_t* swapchain) { nd3d12::swapchain_t* pSwapchain = CreateComponent<ngfx::swapchain_t, nd3d12::swapchain_t>(device, swapchain); }

            void DestroySwapchain(ngfx::device_t* device, ngfx::swapchain_t* swapchain)
            {
                nd3d12::swapchain_t* dxswapchain = GetComponent<ngfx::swapchain_t, nd3d12::swapchain_t>(device, swapchain);
                if (dxswapchain)
                {
                    nd3d12::Destroy(device, swapchain);
                    DestroyComponent<ngfx::swapchain_t, nd3d12::swapchain_t>(device, swapchain);
                }
            }

            bool Create(ngfx::device_t* device, ngfx::swapchain_t* swapchain)
            {
                nd3d12::swapchain_t* dxswapchain = GetComponent<ngfx::swapchain_t, nd3d12::swapchain_t>(device, swapchain);
                nd3d12::device_t*    dxdevice    = GetComponent<ngfx::device_t, nd3d12::device_t>(device, device);

                IDXGIFactory5* pFactory = dxdevice->m_pDxgiFactory;

                BOOL allowTearing = FALSE;
                pFactory->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allowTearing, sizeof(allowTearing));
                dxswapchain->m_bSupportTearing = allowTearing;

                DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
                swapChainDesc.BufferCount           = swapchain->m_desc.backbuffer_count;
                swapChainDesc.Width                 = swapchain->m_desc.width;
                swapChainDesc.Height                = swapchain->m_desc.height;
                swapChainDesc.Format                = swapchain->m_desc.backbuffer_format == GfxFormat::RGBA8SRGB ? DXGI_FORMAT_R8G8B8A8_UNORM : dxgi_format(swapchain->m_desc.backbuffer_format);
                swapChainDesc.BufferUsage           = DXGI_USAGE_RENDER_TARGET_OUTPUT;
                swapChainDesc.SwapEffect            = DXGI_SWAP_EFFECT_FLIP_DISCARD;
                swapChainDesc.SampleDesc.Count      = 1;
                swapChainDesc.Flags                 = allowTearing ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;

                // Swap chain needs the queue so that it can force a flush on it.
                IDXGISwapChain1* pSwapChain = NULL;
                HRESULT          hr         = pFactory->CreateSwapChainForHwnd(pDevice->GetGraphicsQueue(), (HWND)swapchain->m_desc.window_handle, &swapChainDesc, nullptr, nullptr, &pSwapChain);
                if (FAILED(hr))
                {
                    // RE_ERROR("[D3D12Swapchain] failed to create {}", m_name);
                    return false;
                }

                pSwapChain->QueryInterface(&dxswapchain->m_pSwapChain);
                SAFE_RELEASE(pSwapChain);

                return CreateTextures();
            }

            void Destroy(ngfx::device_t* device, ngfx::swapchain_t* swapchain)
            {
                nd3d12::swapchain_t* dxswapchain = GetComponent<ngfx::swapchain_t, nd3d12::swapchain_t>(device, swapchain);
                for (s32 i = 0; i < dxswapchain->m_backBuffers.size; ++i)
                {
                    ngfx::DestroyTexture(device, dxswapchain->m_backBuffers.data[i]);
                }
                dxswapchain->m_backBuffers.size = 0;

                nd3d12::device_t* dxdevice = GetComponent<ngfx::device_t, nd3d12::device_t>(device, device);
                nd3d12::Delete(dxdevice, dxswapchain->m_pSwapChain);
            }

            void Present(ngfx::device_t* device, ngfx::swapchain_t* swapchain)
            {
                // CPU_EVENT("Render", "D3D12Swapchain::Present");
                nd3d12::swapchain_t* dxswapchain = GetComponent<ngfx::swapchain_t, nd3d12::swapchain_t>(device, swapchain);

                UINT interval = 1;
                UINT flags    = 0;
                if (!dxswapchain->m_bEnableVsync)
                {
                    interval = 0;
                    flags    = dxswapchain->m_bSupportTearing && dxswapchain->m_bWindowMode ? DXGI_PRESENT_ALLOW_TEARING : 0;
                }

                HRESULT hr = dxswapchain->m_pSwapChain->Present(interval, flags);
                return SUCCEEDED(hr);
            }

            void AcquireNextBackBuffer(ngfx::device_t* device, ngfx::swapchain_t* swapchain)
            {
                nd3d12::swapchain_t* dxswapchain  = GetComponent<ngfx::swapchain_t, nd3d12::swapchain_t>(device, swapchain);
                dxswapchain->m_nCurrentBackBuffer = dxswapchain->m_pSwapChain->GetCurrentBackBufferIndex();
            }

            ngfx::texture_t* GetBackBuffer(ngfx::device_t* device, ngfx::swapchain_t* swapchain)
            {
                nd3d12::swapchain_t* dxswapchain = GetComponent<ngfx::swapchain_t, nd3d12::swapchain_t>(device, swapchain);
                return dxswapchain->m_backBuffers.data[dxswapchain->m_nCurrentBackBuffer];
            }

            bool Resize(ngfx::device_t* device, ngfx::swapchain_t* swapchain, u32 width, u32 height)
            {
                nd3d12::swapchain_t* dxswapchain = GetComponent<ngfx::swapchain_t, nd3d12::swapchain_t>(device, swapchain);
                if (dxswapchain->m_desc.width == width && dxswapchain->m_desc.height == height)
                {
                    return false;
                }

                dxswapchain->m_desc.width         = width;
                dxswapchain->m_desc.height        = height;
                dxswapchain->m_nCurrentBackBuffer = 0;

                for (size_t i = 0; i < dxswapchain->m_backBuffers.size; ++i)
                {
                    ngfx::DestroyTexture(device, dxswapchain->m_backBuffers.data[i]);
                }
                dxswapchain->m_backBuffers.size = 0;

                nd3d12::device_t* dxdevice = GetComponent<ngfx::device_t, nd3d12::device_t>(device, device);
                dxdevice->FlushDeferredDeletions();

                DXGI_SWAP_CHAIN_DESC desc = {};
                dxswapchain->m_pSwapChain->GetDesc(&desc);
                HRESULT hr = dxswapchain->m_pSwapChain->ResizeBuffers(swapchain->m_desc.backbuffer_count, width, height, desc.BufferDesc.Format, desc.Flags);
                if (!SUCCEEDED(hr))
                {
                    return false;
                }

                BOOL fullscreenState;
                dxswapchain->m_pSwapChain->GetFullscreenState(&fullscreenState, nullptr);
                dxswapchain->m_bWindowMode = !fullscreenState;

                return CreateTextures(device, dxswapchain, swapchain->m_desc);
            }

            void SetVSyncEnabled(ngfx::device_t* device, ngfx::swapchain_t* swapchain, bool value)
            {
                nd3d12::swapchain_t* dxswapchain = GetComponent<ngfx::swapchain_t, nd3d12::swapchain_t>(device, swapchain);
                dxswapchain->m_bEnableVsync      = value;
            }

        }  // namespace nd3d12
#endif
    }  // namespace ngfx
}  // namespace ncore
