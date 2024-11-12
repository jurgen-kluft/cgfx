#ifndef __CGFX_MOCK_HEAP_H__
#define __CGFX_MOCK_HEAP_H__

#include "cgfx/gfx_heap.h"

namespace ncore
{
    namespace ngfx
    {
        class MockDevice;

        class MockHeap : public IGfxHeap
        {
        public:
            MockHeap(MockDevice* pDevice, const GfxHeapDesc& desc, const char* name);
            ~MockHeap();

            bool Create();

            virtual void* GetHandle() const override;
        };

    }  // namespace ngfx
}  // namespace ncore
#endif
