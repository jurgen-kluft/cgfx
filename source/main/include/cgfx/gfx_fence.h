#ifndef __CGFX_GFX_FENCE_H__
#define __CGFX_GFX_FENCE_H__

#include "cgfx/gfx_resource.h"

namespace ncore
{
    namespace ngfx
    {
        struct fence_t
        {
            D_GFX_OCS_COMPONENT;
        };

        fence_t* CreateFence(device_t* device, const char* name);
        void     Destroy(device_t* device, fence_t* resource);
        void     Wait(device_t* device, fence_t* fence, u64 value);
        void     Signal(device_t* device, fence_t* fence, u64 value);

    }  // namespace ngfx
}  // namespace ncore

#endif
