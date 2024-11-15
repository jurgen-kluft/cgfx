#ifndef __CGFX_MOCK_RT_BLAS_H__
#define __CGFX_MOCK_RT_BLAS_H__

#include "cgfx/gfx_rt_blas.h"

namespace ncore
{
    namespace ngfx
    {
        namespace nmock
        {
            void  Destroy(ngfx::device_t* pDevice, ngfx::blas_t* pBLAS);
            bool  Create(ngfx::device_t* pDevice, ngfx::blas_t* pBLAS);
            void* GetHandle(ngfx::device_t* pDevice, const ngfx::blas_t* pBLAS);
        }  // namespace nmock
    }  // namespace ngfx
}  // namespace ncore

#endif
