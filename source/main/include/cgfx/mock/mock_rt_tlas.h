#ifndef __CGFX_MOCK_RT_TLAS_H__
#define __CGFX_MOCK_RT_TLAS_H__

#include "cgfx/gfx_rt_tlas.h"

namespace ncore
{
    namespace ngfx
    {
        namespace nmock
        {
            void  Destroy(ngfx::device_t* pDevice, ngfx::tlas_t* pTLAS);
            bool  Create(ngfx::device_t* pDevice, ngfx::tlas_t* pTLAS);
            void* GetHandle(ngfx::device_t* pDevice, const ngfx::tlas_t* pTLAS);
        }  // namespace nmock
    }  // namespace ngfx
}  // namespace ncore
#endif
