#include "cgfx/mock/mock_heap.h"
#include "cgfx/mock/mock_device.h"

namespace ncore
{
    namespace ngfx
    {
        namespace nmock
        {
            heap_t* CreateHeap(device_t* pDevice, heap_t* heap) { return heap; }
            bool    Create(device_t* pDevice, heap_t* pHeap) { return true; }
            void    Destroy(device_t* pDevice, heap_t* pHeap) {}
        }  // namespace nmock
    }  // namespace ngfx
}  // namespace ncore
