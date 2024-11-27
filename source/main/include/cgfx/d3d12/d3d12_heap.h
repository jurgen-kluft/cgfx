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
#ifdef TARGET_PC
        namespace nd3d12
        {
            struct heap_t
            {
                D_GFX_OCS_COMPONENT_SET(enums::ComponentD3D12Heap);
                D3D12MA::Allocation* m_pAllocation = nullptr;
                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            void                 CreateHeap(ngfx::device_t* pDevice, ngfx::heap_t* heap);
            void                 DestroyHeap(ngfx::device_t* pDevice, ngfx::heap_t* heap);
            bool                 Create(ngfx::device_t* pDevice, ngfx::heap_t* pHeap);
            void                 Destroy(ngfx::device_t* pDevice, ngfx::heap_t* pHeap);
        }  // namespace nd3d12
#else
        namespace nd3d12
        {
            void  CreateHeap(ngfx::device_t* pDevice, ngfx::heap_t* heap) {}
            void  DestroyHeap(ngfx::device_t* pDevice, ngfx::heap_t* heap) {}
            bool  Create(ngfx::device_t* pDevice, ngfx::heap_t* pHeap) { return false; }
            void  Destroy(ngfx::device_t* pDevice, ngfx::heap_t* pHeap) {}
        }  // namespace nd3d12
#endif
    }  // namespace ngfx
}  // namespace ncore
#endif
