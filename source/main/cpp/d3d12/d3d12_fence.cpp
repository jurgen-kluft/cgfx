#include "cgfx/d3d12/d3d12_fence.h"
#include "cgfx/d3d12/d3d12_device.h"

namespace ncore
{
    namespace ngfx
    {
        D3D12Fence::D3D12Fence(D3D12Device* pDevice, const char* name)
        {
            m_pDevice = pDevice;
            m_name    = name;
        }

        D3D12Fence::~D3D12Fence()
        {
            D3D12Device* pDevice = (D3D12Device*)m_pDevice;
            pDevice->Delete(m_pFence);

            CloseHandle(m_hEvent);
        }

        void D3D12Fence::Wait(u64 value)
        {
            if (m_pFence->GetCompletedValue() != value)
            {
                m_pFence->SetEventOnCompletion(value, m_hEvent);
                WaitForSingleObjectEx(m_hEvent, INFINITE, FALSE);
            }
        }

        void D3D12Fence::Signal(u64 value) { m_pFence->Signal(value); }

        bool D3D12Fence::Create()
        {
            ID3D12Device* pDevice = (ID3D12Device*)m_pDevice->GetHandle();

            HRESULT hr = pDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_pFence));
            if (FAILED(hr))
            {
                // RE_ERROR("[D3D12Fence] failed to create {}", m_name);
                return false;
            }

            m_pFence->SetName(string_to_wstring(m_name).c_str());

            m_hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

            return true;
        }
    }  // namespace ngfx
}  // namespace ncore
