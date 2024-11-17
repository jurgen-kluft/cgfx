#include "cgfx/metal/metal_fence.h"
#include "cgfx/metal/metal_device.h"

namespace ncore
{
    namespace ngfx
    {
        // MetalFence::MetalFence(MetalDevice* pDevice, const char* name)
        // {
        //     m_pDevice = pDevice;
        //     m_name    = name;
        // }
        // MetalFence::~MetalFence() { m_pEvent->release(); }
        // bool MetalFence::Create()
        // {
        //     MTL::Device* device = (MTL::Device*)m_pDevice->GetHandle();
        //     m_pEvent = device->newSharedEvent();
        //     if (m_pEvent == nullptr)
        //     {
        //         // RE_ERROR("[MetalFence] failed to create {}", m_name);
        //         return false;
        //     }
        //     SetDebugLabel(m_pEvent, m_name);
        //     return true;
        // }

        // void MetalFence::Wait(u64 value) { m_pEvent->waitUntilSignaledValue(value, UINT64_MAX); }
        // void MetalFence::Signal(u64 value) { m_pEvent->setSignaledValue(value); }

        namespace nmetal
        {
            ngfx::fence_t* CreateFence(ngfx::device_t* device, resource_t* resource, ngfx::fence_t* fence);
            void           Destroy(ngfx::device_t* device, ngfx::fence_t* fence);
            void           Wait(ngfx::device_t* device, ngfx::fence_t* fence, u64 value);
            void           Signal(ngfx::device_t* device, ngfx::fence_t* fence, u64 value);
        }  // namespace nmetal

    }  // namespace ngfx
}  // namespace ncore
