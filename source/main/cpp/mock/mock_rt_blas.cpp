#include "cgfx/mock/mock_rt_blas.h"
#include "cgfx/mock/mock_device.h"

namespace ncore
{
    namespace ngfx
    {
        namespace nmock
        {
            void  Destroy(ngfx::device_t* pDevice, ngfx::blas_t* pTLAS) {}
            bool  Create(ngfx::device_t* pDevice, ngfx::blas_t* pTLAS) { return true; }
            void* GetHandle(ngfx::device_t* pDevice, const ngfx::blas_t* pTLAS) { return nullptr; }
        }  // namespace nmock
    }  // namespace ngfx
}  // namespace ncore
