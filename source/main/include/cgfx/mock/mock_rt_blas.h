#ifndef __CGFX_MOCK_RT_BLAS_H__
#define __CGFX_MOCK_RT_BLAS_H__

#include "cgfx/gfx_rt_blas.h"

namespace ncore
{
    namespace ngfx
    {
        class MockDevice;

        class MockRayTracingBLAS : public IGfxRayTracingBLAS
        {
        public:
            MockRayTracingBLAS(MockDevice* pDevice, const GfxRayTracing::BLASDesc& desc, const char* name);
            ~MockRayTracingBLAS();

            bool Create();

            virtual void* GetHandle() const override;
        };

    }  // namespace ngfx
}  // namespace ncore
#endif
