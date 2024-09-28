#ifndef __CGFX_GFX_DESCRIPTOR_H__
#define __CGFX_GFX_DESCRIPTOR_H__

#include "cgfx/gfx_resource.h"

namespace ncore
{
    namespace ngfx
    {

        class IGfxDescriptor : public IGfxResource
        {
        public:
            virtual u32 GetHeapIndex() const = 0;
        };

    }  // namespace ngfx
}  // namespace ncore

#endif
