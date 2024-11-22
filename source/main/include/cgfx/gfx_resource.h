#ifndef __CGFX_GFX_RESOURCE_H__
#define __CGFX_GFX_RESOURCE_H__

#include "ccore/c_allocator.h"
#include "cgfx/gfx_defines.h"

namespace ncore
{
    namespace ngfx
    {
        struct resource_t
        {
            D_GFX_OCS_COMPONENT_SET(enums::ComponentResource);
            DCORE_CLASS_PLACEMENT_NEW_DELETE
        };

        struct name_t
        {
            D_GFX_OCS_COMPONENT_SET(enums::ComponentName);
            char m_name[128];
            DCORE_CLASS_PLACEMENT_NEW_DELETE
        };

        void SetName(name_t* name, const char* set);

    }  // namespace ngfx
}  // namespace ncore

#endif
