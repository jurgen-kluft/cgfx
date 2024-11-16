#include "cgfx/mock/mock_rt_tlas.h"
#include "cgfx/mock/mock_device.h"

namespace ncore
{
    namespace ngfx
    {
        namespace nmock
        {
            ngfx::tlas_t* CreateRayTracingTLAS(ngfx::device_t* device, ngfx::tlas_t* tlas) { return tlas; }
            void  Destroy(ngfx::device_t* pDevice, ngfx::tlas_t* pTLAS) {}
            bool  Create(ngfx::device_t* pDevice, ngfx::tlas_t* pTLAS) { return true; }
            void* GetHandle(ngfx::device_t* pDevice, const ngfx::tlas_t* pTLAS) { return nullptr; }
        }  // namespace nmock
    }  // namespace ngfx
}  // namespace ncore
