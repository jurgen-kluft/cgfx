#include "cgfx/mock/mock_heap.h"
#include "cgfx/mock/mock_device.h"

namespace ncore
{
    namespace ngfx
    {
        namespace nmock
        {
            heap_t* Alloc(device_t* pDevice, resource_t* resource, heap_t* heap) { return heap; }
            bool    Create(device_t* pDevice, heap_t* pHeap) { return true; }
            void    Destroy(device_t* pDevice, heap_t* pHeap) {}
            void*   GetHandle(device_t* pDevice, const heap_t* pHeap) { return nullptr; }
        }  // namespace nmock

    }  // namespace ngfx
}  // namespace ncore
