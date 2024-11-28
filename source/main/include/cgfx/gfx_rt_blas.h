#ifndef __CGFX_GFX_RT_BLAS_H__
#define __CGFX_GFX_RT_BLAS_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
    #pragma once
#endif

#include "cgfx/gfx_resource.h"

namespace ncore
{
    namespace ngfx
    {
        struct blas_t
        {
            D_GFX_CS_COMPONENT_SET(enums::ComponentBlas);
            blas_desc_t m_desc;
            DCORE_CLASS_PLACEMENT_NEW_DELETE
        };

        blas_t* CreateRayTracingBLAS(device_t* device, const blas_desc_t& desc, const char* name);
        bool    Create(device_t* device, blas_t* blas);
        void    Destroy(device_t* device, blas_t* blas);
        void*   GetHandle(ngfx::device_t* pDevice, const ngfx::blas_t* pBLAS);

    }  // namespace ngfx
}  // namespace ncore

#endif
