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
            struct heap_t
            {
                D_GFX_OCS_COMPONENT_SET(enums::ComponentD3D12Heap);
                D3D12MA::Allocation* m_pAllocation = nullptr;
                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            ngfx::heap_t* CreateHeap(ngfx::device_t* pDevice, ngfx::heap_t* heap);
            bool          Create(ngfx::device_t* pDevice, ngfx::heap_t* pHeap);
            void          Destroy(ngfx::device_t* pDevice, ngfx::heap_t* pHeap);
            void*         GetHandle(ngfx::device_t* pDevice, const ngfx::heap_t* pHeap);
        }  // namespace nd3d12
    }  // namespace ngfx
}  // namespace ncore
#endif
