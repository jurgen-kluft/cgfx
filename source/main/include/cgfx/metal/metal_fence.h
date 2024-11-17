#ifndef __CGFX_METAL_FENCE_H__
#define __CGFX_METAL_FENCE_H__

#include "cgfx/metal/metal_utils.h"
#include "cgfx/gfx_fence.h"

namespace ncore
{
    namespace ngfx
    {
        namespace nmetal
        {
            struct fence_t
            {
                MTL::SharedEvent* m_pEvent = nullptr;
            };

            ngfx::fence_t* CreateFence(ngfx::device_t* device, resource_t* resource, ngfx::fence_t* fence);
            void           Destroy(ngfx::device_t* device, ngfx::fence_t* fence);
            void           Wait(ngfx::device_t* device, ngfx::fence_t* fence, u64 value);
            void           Signal(ngfx::device_t* device, ngfx::fence_t* fence, u64 value);
        }  // namespace nmetal
    }  // namespace ngfx
}  // namespace ncore
#endif
