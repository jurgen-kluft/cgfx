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
        heap_t* CreateHeap(device_t* device, const heap_desc_t& desc, const char* name)
        {
            heap_t*     heap     = CreateInstance<heap_t>(device, name);
            switch (device->m_desc.backend)
            {
                case enums::Backend_D3D12: nd3d12::CreateHeap(device, heap); break;
                case enums::Backend_Metal: nmetal::CreateHeap(device, heap); break;
                case enums::Backend_Mock: nmock::CreateHeap(device, heap); break;
            }
            return heap;
        }

        bool Create(device_t* device, heap_t* heap)
        {
            switch (device->m_desc.backend)
            {
                case enums::Backend_D3D12: return nd3d12::Create(device, heap); break;
                case enums::Backend_Metal: return nmetal::Create(device, heap); break;
                case enums::Backend_Mock: return nmock::Create(device, heap); break;
            }
            return false;
        }

        void Destroy(device_t* device, heap_t* heap)
        {
            switch (device->m_desc.backend)
            {
                case enums::Backend_D3D12: nd3d12::Destroy(device, heap); break;
                case enums::Backend_Metal: nmetal::Destroy(device, heap); break;
                case enums::Backend_Mock: nmock::Destroy(device, heap); break;
            }
        }
    }  // namespace ngfx
}  // namespace ncore
