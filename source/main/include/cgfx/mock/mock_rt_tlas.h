#ifndef __CGFX_MOCK_RT_TLAS_H__
#define __CGFX_MOCK_RT_TLAS_H__

#include "cgfx/gfx_rt_tlas.h"

namespace ncore
{
    namespace ngfx
    {
        namespace nmock
        {
            ngfx::tlas_t* CreateRayTracingTLAS(ngfx::device_t* device, ngfx::tlas_t* tlas);
            void          Destroy(ngfx::device_t* device, ngfx::tlas_t* tlas);
            bool          Create(ngfx::device_t* device, ngfx::tlas_t* tlas);
        }  // namespace nmock
    }  // namespace ngfx
}  // namespace ncore
#endif
