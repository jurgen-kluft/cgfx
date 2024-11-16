#ifndef __CGFX_D3D12_HEAP_H__
#define __CGFX_D3D12_HEAP_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
    #pragma once
#endif

// #include "cgfx/d3d12/d3d12_header.h"
#include "cgfx/gfx_heap.h"

namespace ncore
{
    namespace ngfx
    {
        namespace nd3d12
        {
            heap_t* CreateHeap(device_t* pDevice, resource_t* resource, heap_t* heap);
            bool    Create(device_t* pDevice, heap_t* pHeap);
            void    Destroy(device_t* pDevice, heap_t* pHeap);
            void*   GetHandle(device_t* pDevice, const heap_t* pHeap);
        }  // namespace nd3d12

        // class D3D12Device;

        // namespace D3D12MA
        // {
        //     class Allocation;
        // }

        // class D3D12Heap : public heap_t
        // {
        // public:
        //     D3D12Heap(D3D12Device* pDevice, const heap_desc_t& desc, const char* name);
        //     ~D3D12Heap();

        //     virtual void* GetHandle() const override;
        //     ID3D12Heap*   GetHeap() const;

        //     bool Create();

        // private:
        //     D3D12MA::Allocation* m_pAllocation = nullptr;
        // };
    }  // namespace ngfx
}  // namespace ncore
#endif
