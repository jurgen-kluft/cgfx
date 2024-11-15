#ifndef __CGFX_GFX_RT_TLAS_H__
#define __CGFX_GFX_RT_TLAS_H__

#include "cgfx/gfx_resource.h"

namespace ncore
{
    namespace ngfx
    {
        struct tlas_t
        {
            D_GFX_OCS_COMPONENT;
            tlas_desc_t m_desc;
        };

        tlas_t* CreateRayTracingTLAS(device_t* device, const tlas_desc_t& desc, const char* name);
        void    Destroy(device_t* device, tlas_t* resource);

    }  // namespace ngfx
}  // namespace ncore

#endif
