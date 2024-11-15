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

        heap_t* CreateHeap(device_t* device, const heap_desc_t& desc, const char* name)
        {
            resource_t* resource = CreateObject<resource_t>(device, name);
            heap_t*     heap     = AddComponent<resource_t, heap_t>(device, resource);
            return heap;
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

        void* GetHandle(device_t* pDevice, const heap_t* heap)
        {
            switch (pDevice->m_desc.backend)
            {
                case enums::Backend_D3D12: return nd3d12::GetHandle(pDevice, heap);
                case enums::Backend_Metal: return nmetal::GetHandle(pDevice, heap);
                case enums::Backend_Mock: return nmock::GetHandle(pDevice, heap);
            }
            return nullptr;
        }

    }  // namespace ngfx
}  // namespace ncore

#endif
