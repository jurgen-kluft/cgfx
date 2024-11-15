#ifndef __CGFX_MOCK_FENCE_H__
#define __CGFX_MOCK_FENCE_H__

#include "cgfx/gfx_fence.h"

namespace ncore
{
    namespace ngfx
    {
        struct device_t;
        namespace nmock
        {
            fence_t* CreateFence(device_t* device, resource_t* resource, fence_t* fence);
            void     Destroy(device_t* device, fence_t* fence);
            void     Wait(device_t* device, fence_t* fence, u64 value);
            void     Signal(device_t* device, fence_t* fence, u64 value);
        }  // namespace nmock
    }  // namespace ngfx
}  // namespace ncore
#endif
