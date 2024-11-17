#ifndef __CGFX_METAL_HEAP_H__
#define __CGFX_METAL_HEAP_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
    #pragma once
#endif

#include "cgfx/metal/metal_utils.h"
#include "cgfx/gfx_heap.h"

namespace ncore
{
    namespace ngfx
    {
        namespace nmetal
        {
            struct mheap_t
            {
                MTL::Heap* m_pHeap = nullptr;
            };

            heap_t* CreateHeap(device_t* pDevice, resource_t* resource, heap_t* heap);
            bool    Create(device_t* pDevice, heap_t* pHeap);
            void    Destroy(device_t* pDevice, heap_t* pHeap);
            void*   GetHandle(device_t* pDevice, const heap_t* pHeap);
        }  // namespace nmetal
    }  // namespace ngfx
}  // namespace ncore
#endif
