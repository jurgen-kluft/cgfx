#ifndef __CGFX_GFX_HEAP_H__
#define __CGFX_GFX_HEAP_H__

#include "cgfx/gfx_resource.h"

namespace ncore
{
    namespace ngfx
    {
        struct heap_t
        {
            D_GFX_OCS_COMPONENT;
            heap_desc_t m_desc = {};
        };

        heap_t* CreateHeap(device_t* device, const heap_desc_t& desc, const char* name);
        bool    Create(device_t* device, heap_t* heap);
        void    Destroy(device_t* device, heap_t* heap);
        void*   GetHandle(device_t* device, const heap_t* heap);

    }  // namespace ngfx
}  // namespace ncore

#endif
