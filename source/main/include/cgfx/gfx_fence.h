#ifndef __CGFX_GFX_FENCE_H__
#define __CGFX_GFX_FENCE_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
    #pragma once
#endif

#include "cgfx/gfx_name.h"

namespace ncore
{
    namespace ngfx
    {
        struct fence_t
        {
            D_GFX_CS_COMPONENT_SET(enums::ComponentFence);
            DCORE_CLASS_PLACEMENT_NEW_DELETE
        };

        fence_t* CreateFence(device_t* device, const char* name);
        void     Destroy(device_t* device, fence_t* resource);
        void     Wait(device_t* device, fence_t* fence, u64 value);
        void     Signal(device_t* device, fence_t* fence, u64 value);

    }  // namespace ngfx
}  // namespace ncore

#endif
