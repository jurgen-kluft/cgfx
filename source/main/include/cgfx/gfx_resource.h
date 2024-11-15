#ifndef __CGFX_GFX_RESOURCE_H__
#define __CGFX_GFX_RESOURCE_H__

#include "cgfx/gfx_defines.h"

namespace ncore
{
    namespace ngfx
    {
        struct resource_t
        {
            D_GFX_OCS_OBJECT;
        };

        struct name_t
        {
            D_GFX_OCS_COMPONENT;
            char m_name[128];
        };

        void SetName(name_t* name, const char* set);

    }  // namespace ngfx
}  // namespace ncore

#endif
