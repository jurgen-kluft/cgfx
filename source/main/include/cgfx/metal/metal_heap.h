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
        class MetalDevice;

        class MetalHeap : public IGfxHeap
        {
        public:
            MetalHeap(MetalDevice* pDevice, const GfxHeapDesc& desc, const char* name);
            ~MetalHeap();

            bool Create();

            virtual void* GetHandle() const override { return m_pHeap; }

        private:
            MTL::Heap* m_pHeap = nullptr;
        };

    }  // namespace ngfx
}  // namespace ncore
#endif
