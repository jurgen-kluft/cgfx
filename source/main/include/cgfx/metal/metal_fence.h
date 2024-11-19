#ifndef __CGFX_METAL_FENCE_H__
#define __CGFX_METAL_FENCE_H__

#include "cgfx/metal/metal_utils.h"
#include "cgfx/gfx_fence.h"

namespace ncore
{
    namespace ngfx
    {
#ifdef TARGET_MAC
        namespace nmetal
        {
            struct fence_t
            {
                D_GFX_OCS_COMPONENT_SET(enums::ComponentMetalFence);
                MTL::SharedEvent* m_pEvent = nullptr;
                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            ngfx::fence_t* CreateFence(ngfx::device_t* device, resource_t* resource, ngfx::fence_t* fence);
            void           Destroy(ngfx::device_t* device, ngfx::fence_t* fence);
            void           Wait(ngfx::device_t* device, ngfx::fence_t* fence, u64 value);
            void           Signal(ngfx::device_t* device, ngfx::fence_t* fence, u64 value);
        }  // namespace nmetal
#else
        namespace nmetal
        {
            ngfx::fence_t* CreateFence(ngfx::device_t* device, resource_t* resource, ngfx::fence_t* fence) { return fence; }
            void           Destroy(ngfx::device_t* device, ngfx::fence_t* fence) {}
            void           Wait(ngfx::device_t* device, ngfx::fence_t* fence, u64 value) {}
            void           Signal(ngfx::device_t* device, ngfx::fence_t* fence, u64 value) {}
        }  // namespace nmetal
#endif
    }  // namespace ngfx
}  // namespace ncore
#endif
