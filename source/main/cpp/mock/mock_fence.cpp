#include "cgfx/mock/mock_fence.h"
#include "cgfx/mock/mock_device.h"

namespace ncore
{
    namespace ngfx
    {
        namespace nmock
        {
            fence_t* CreateFence(device_t* device, fence_t* fence)
            {
                return fence;
            }

            void     Destroy(device_t* device, fence_t* fence)
            {
            }
            
            void     Wait(device_t* device, fence_t* fence, u64 value)
            {
            }

            void     Signal(device_t* device, fence_t* fence, u64 value)
            {
            }
        }  // namespace nmock

    }  // namespace ngfx
}  // namespace ncore
