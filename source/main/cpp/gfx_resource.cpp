#include "cgfx/gfx.h"
#include "cgfx/gfx_resource.h"

#include "ccore/c_math.h"

namespace ncore
{
    namespace ngfx
    {
        void SetName(name_t* name, const char* set)
        {
            u32 i = 0;
            for (; i < g_array_size(name->m_name) - 1; ++i)
            {
                name->m_name[i] = set[i];
                if (set[i] == '\0')
                    break;
            }
            name->m_name[i] = '\0';
        }
    }  // namespace ngfx
}  // namespace ncore
