#include "cgfx/mock/mock_fence.h"
#include "cgfx/mock/mock_device.h"

namespace ncore
{
    namespace ngfx
    {
        MockFence::MockFence(MockDevice* pDevice, const char* name)
        {
            m_pDevice = pDevice;
            m_name    = name;
        }

        MockFence::~MockFence() {}

        bool MockFence::Create() { return true; }

        void* MockFence::GetHandle() const { return nullptr; }

        void MockFence::Wait(u64 value) {}

        void MockFence::Signal(u64 value) {}

    }  // namespace ngfx
}  // namespace ncore
