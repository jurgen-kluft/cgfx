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
            heap_t* CreateHeap(device_t* pDevice, resource_t* resource, heap_t* heap);
            bool    Create(device_t* pDevice, heap_t* pHeap);
            void    Destroy(device_t* pDevice, heap_t* pHeap);
            void*   GetHandle(device_t* pDevice, const heap_t* pHeap);
        }  // namespace nmetal

        // class MetalDevice;

        // class MetalHeap : public heap_t
        // {
        // public:
        //     MetalHeap(MetalDevice* pDevice, const heap_desc_t& desc, const char* name);
        //     ~MetalHeap();

        //     bool Create();

        //     virtual void* GetHandle() const override { return m_pHeap; }

        // private:
        //     MTL::Heap* m_pHeap = nullptr;
        // };

    }  // namespace ngfx
}  // namespace ncore
#endif
