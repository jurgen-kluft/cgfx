#ifndef __CGFX_MOCK_HEAP_H__
#define __CGFX_MOCK_HEAP_H__

#include "cgfx/gfx_heap.h"

namespace ncore
{
    namespace ngfx
    {
        namespace nmock
        {
            heap_t* CreateHeap(device_t* pDevice, heap_t* heap);
            bool    Create(device_t* pDevice, heap_t* pHeap);
            void    Destroy(device_t* pDevice, heap_t* pHeap);
        }  // namespace nmock
    }  // namespace ngfx
}  // namespace ncore
#endif
