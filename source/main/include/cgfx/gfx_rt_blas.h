#ifndef __CGFX_GFX_RT_BLAS_H__
#define __CGFX_GFX_RT_BLAS_H__

#include "cgfx/gfx_resource.h"

namespace ncore
{
    namespace ngfx
    {
        struct blas_t
        {
            D_GFX_OCS_COMPONENT;
            blas_desc_t m_desc;
        };

        blas_t* CreateRayTracingBLAS(device_t* device, const blas_desc_t& desc, const char* name);
        void    Destroy(device_t* device, blas_t* resource);

    }  // namespace ngfx
}  // namespace ncore

#endif
