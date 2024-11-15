#include "cgfx/gfx.h"

#include "cgfx/d3d12/d3d12_device.h"
#include "cgfx/d3d12/d3d12_heap.h"

#include "cgfx/metal/metal_device.h"
#include "cgfx/metal/metal_heap.h"

#include "cgfx/mock/mock_device.h"
#include "cgfx/mock/mock_heap.h"

namespace ncore
{
    namespace ngfx
    {
        heap_t* CreateHeap(device_t* device, const heap_desc_t& desc, const char* name);
        void    Destroy(device_t* device, heap_t* resource);
        void*   GetHandle(device_t* pDevice, const heap_t* pHeap);
    }  // namespace ngfx
}  // namespace ncore
