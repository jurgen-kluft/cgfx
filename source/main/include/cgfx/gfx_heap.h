#ifndef __CGFX_GFX_HEAP_H__
#define __CGFX_GFX_HEAP_H__

#include "cgfx/gfx_resource.h"

namespace ncore
{
    namespace ngfx
    {
        class IGfxHeap : public IGfxResource
        {
        public:
            const GfxHeapDesc& GetDesc() const { return m_desc; }

        protected:
            GfxHeapDesc m_desc = {};
        };
    }  // namespace ngfx
}  // namespace ncore

#endif
