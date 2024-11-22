#ifndef __CGFX_GFX_RT_TLAS_H__
#define __CGFX_GFX_RT_TLAS_H__

#include "cgfx/gfx_resource.h"

namespace ncore
{
    namespace ngfx
    {
        struct tlas_t
        {
            D_GFX_OCS_COMPONENT_SET(enums::ComponentTlas);
            tlas_desc_t m_desc;
            DCORE_CLASS_PLACEMENT_NEW_DELETE
        };

        tlas_t* CreateRayTracingTLAS(device_t* device, const tlas_desc_t& desc, const char* name);
        bool    Create(device_t* device, tlas_t* tlas);
        void    Destroy(device_t* device, tlas_t* tlas);
        void*   GetHandle(ngfx::device_t* device, const ngfx::tlas_t* tlas);

    }  // namespace ngfx
}  // namespace ncore

#endif
