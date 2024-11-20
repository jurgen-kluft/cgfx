#include "cgfx/metal/metal_fence.h"
#include "cgfx/metal/metal_device.h"

namespace ncore
{
    namespace ngfx
    {
        namespace nmetal
        {
            ngfx::fence_t* CreateFence(ngfx::device_t* device, ngfx::fence_t* fence)
            {
                nmetal::fence_t* mfence = CreateComponent<ngfx::fence_t, nmetal::fence_t>(device, fence);
                mfence->m_pEvent        = nullptr;
                return fence;
            }

            bool Create(ngfx::device_t* device, ngfx::fence_t* fence)
            {
                nmetal::fence_t* mfence    = GetComponent<ngfx::fence_t, nmetal::fence_t>(device, fence);
                MTL::Device*     mtlDevice = nmetal::GetHandle(device);
                mfence->m_pEvent           = mtlDevice->newSharedEvent();
                if (mfence->m_pEvent == nullptr)
                {
                    // RE_ERROR("[MetalFence] failed to create {}", m_name);
                    return false;
                }

                name_t const* name = GetComponent<ngfx::fence_t, name_t>(device, fence);
                SetDebugLabel(mfence->m_pEvent, name->m_name);
                return true;
            }

            void Destroy(ngfx::device_t* device, ngfx::fence_t* fence)
            {
                nmetal::fence_t* mfence = GetComponent<ngfx::fence_t, nmetal::fence_t>(device, fence);
                mfence->m_pEvent->release();
            }

            void Wait(ngfx::device_t* device, ngfx::fence_t* fence, u64 value)
            {
                nmetal::fence_t* mfence = GetComponent<ngfx::fence_t, nmetal::fence_t>(device, fence);
                mfence->m_pEvent->waitUntilSignaledValue(value, UINT64_MAX);
            }

            void Signal(ngfx::device_t* device, ngfx::fence_t* fence, u64 value)
            {
                nmetal::fence_t* mfence = GetComponent<ngfx::fence_t, nmetal::fence_t>(device, fence);
                mfence->m_pEvent->setSignaledValue(value);
            }
        }  // namespace nmetal
    }  // namespace ngfx
}  // namespace ncore
