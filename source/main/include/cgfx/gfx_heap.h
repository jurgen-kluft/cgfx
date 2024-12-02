#ifndef __CGFX_GFX_HEAP_H__
#define __CGFX_GFX_HEAP_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
    #pragma once
#endif

#include "cgfx/gfx_resource.h"

namespace ncore
{
    namespace ngfx
    {
        struct heap_t
        {
            D_GFX_CS_COMPONENT_SET(enums::ComponentHeap);
            heap_desc_t m_desc = {};
            DCORE_CLASS_PLACEMENT_NEW_DELETE
        };

        heap_t* CreateHeap(device_t* device, const heap_desc_t& desc, const char* name);
        bool    Create(device_t* device, heap_t* heap);
        void    Destroy(device_t* device, heap_t* heap);
    }  // namespace ngfx
}  // namespace ncore

#endif
