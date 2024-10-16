#include "cgfx/metal/metal_fence.h"
#include "cgfx/metal/metal_device.h"

namespace ncore
{
    namespace ngfx
    {

        MetalFence::MetalFence(MetalDevice* pDevice, const eastl::string& name)
        {
            m_pDevice = pDevice;
            m_name    = name;
        }

        MetalFence::~MetalFence() { m_pEvent->release(); }

        bool MetalFence::Create()
        {
            MTL::Device* device = (MTL::Device*)m_pDevice->GetHandle();

            m_pEvent = device->newSharedEvent();
            if (m_pEvent == nullptr)
            {
                // RE_ERROR("[MetalFence] failed to create {}", m_name);
                return false;
            }

            SetDebugLabel(m_pEvent, m_name.c_str());

            return true;
        }

        void MetalFence::Wait(u64 value) { m_pEvent->waitUntilSignaledValue(value, UINT64_MAX); }

        void MetalFence::Signal(u64 value) { m_pEvent->setSignaledValue(value); }

    }  // namespace ngfx
}  // namespace ncore
