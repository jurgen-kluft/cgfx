#include "cgfx/d3d12/d3d12_fence.h"
#include "cgfx/d3d12/d3d12_device.h"

namespace ncore
{
    namespace ngfx
    {
#ifdef TARGET_PC
        namespace nd3d12
        {
            void CreateFence(ngfx::device_t* device, ngfx::fence_t* fence) { nd3d12::fence_t* dfence = CreateComponent<ngfx::buffer_t, nd3d12::fence_t>(device, fence); }
            void DestroyFence(ngfx::device_t* device, ngfx::fence_t* fence) { DestroyComponent<ngfx::buffer_t, nd3d12::fence_t>(device, fence); }

            void Destroy(ngfx::device_t* device, ngfx::fence_t* fence)
            {
                nd3d12::fence_t*  pFence  = GetComponent<ngfx::fence_t, nd3d12::fence_t>(device, fence);
                nd3d12::device_t* pDevice = GetComponent<ngfx::device_t, nd3d12::device_t>(device, device);
                pDevice->m_pDevice->Delete(pFence->m_pFence);
                CloseHandle(pFence->m_hEvent);
            }

            void Wait(ngfx::device_t* device, ngfx::fence_t* fence, u64 value)
            {
                nd3d12::fence_t* pFence = GetComponent<ngfx::fence_t, nd3d12::fence_t>(device, fence);
                if (pFence->m_pFence->GetCompletedValue() != value)
                {
                    pFence->m_pFence->SetEventOnCompletion(value, pFence->m_hEvent);
                    WaitForSingleObjectEx(pFence->m_hEvent, INFINITE, FALSE);
                }
            }

            void Signal(ngfx::device_t* device, ngfx::fence_t* fence, u64 value)
            {
                nd3d12::fence_t* pFence = GetComponent<ngfx::fence_t, nd3d12::fence_t>(device, fence);
                pFence->m_pFence->Signal(value);
            }
        }  // namespace nd3d12
#endif

    }  // namespace ngfx
}  // namespace ncore
