#ifndef __CGFX_GFX_FENCE_H__
#define __CGFX_GFX_FENCE_H__

#include "cgfx/gfx_resource.h"

namespace ncore
{
    namespace ngfx
    {
        class IGfxFence : public IGfxResource
        {
        public:
            virtual ~IGfxFence() {}

            virtual void Wait(u64 value)   = 0;
            virtual void Signal(u64 value) = 0;
        };

    }  // namespace ngfx
}  // namespace ncore

#endif
