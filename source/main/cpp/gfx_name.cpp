#include "cgfx/gfx.h"
#include "cgfx/gfx_name.h"

#include "ccore/c_math.h"

namespace ncore
{
    namespace ngfx
    {
        void SetName(name_t* name, const char* set)
        {
            // ASCII
            u32 i = 0;
            for (; i < g_array_size(name->m_name) - 1; ++i)
            {
                name->m_name[i] = set[i];
                if (set[i] == '\0')
                    break;
            }
            name->m_name[i] = '\0';

            // UNICODE (wide char)
            for (i = 0; i < g_array_size(name->m_wname) - 1; ++i)
            {
                name->m_wname[i] = set[i];
                if (set[i] == '\0')
                    break;
            }
        }
    }  // namespace ngfx
}  // namespace ncore
