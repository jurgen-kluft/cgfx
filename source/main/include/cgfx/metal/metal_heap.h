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
#ifdef TARGET_MAC
        namespace nmetal
        {
            struct mheap_t
            {
                D_GFX_CS_COMPONENT_SET(enums::ComponentMetalHeap);
                MTL::Heap* m_pHeap = nullptr;
                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            void CreateHeap(ngfx::device_t* pDevice, ngfx::heap_t* heap);
            void DestroyHeap(ngfx::device_t* pDevice, ngfx::heap_t* pHeap);
            bool Create(ngfx::device_t* pDevice, ngfx::heap_t* pHeap);
            void Destroy(ngfx::device_t* pDevice, ngfx::heap_t* pHeap);
        }  // namespace nmetal
#else
        namespace nmetal
        {
            inline void  CreateHeap(ngfx::device_t* pDevice, ngfx::heap_t* heap) {}
            inline void  DestroyHeap(ngfx::device_t* pDevice, ngfx::heap_t* pHeap) {}
            inline bool  Create(ngfx::device_t* pDevice, ngfx::heap_t* pHeap) { return false; }
            inline void  Destroy(ngfx::device_t* pDevice, ngfx::heap_t* pHeap) {}
        }  // namespace nmetal
#endif
    }  // namespace ngfx
}  // namespace ncore
#endif
