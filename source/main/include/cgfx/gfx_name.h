#ifndef __CGFX_GFX_NAMEe_H__
#define __CGFX_GFX_NAME_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
    #pragma once
#endif

#include "ccore/c_allocator.h"
#include "cgfx/gfx_defines.h"

namespace ncore
{
    namespace ngfx
    {
        struct name_t
        {
            enum
            {
                eMaxNameLength = 128
            };

            D_GFX_CS_COMPONENT_SET(enums::ComponentName);
            char    m_name[eMaxNameLength];
            wchar_t m_wname[eMaxNameLength];

            char const*    c_str() const { return &m_name[0]; }
            wchar_t const* w_str() const { return &m_wname[0]; }

            DCORE_CLASS_PLACEMENT_NEW_DELETE
        };

        void SetName(name_t* name, const char* set);

    }  // namespace ngfx
}  // namespace ncore

#endif
