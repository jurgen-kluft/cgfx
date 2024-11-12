#ifndef __CGFX_MOCK_RT_TLAS_H__
#define __CGFX_MOCK_RT_TLAS_H__

#include "cgfx/gfx_rt_tlas.h"

namespace ncore
{
    namespace ngfx
    {

        class MockDevice;

        class MockRayTracingTLAS : public IGfxRayTracingTLAS
        {
        public:
            MockRayTracingTLAS(MockDevice* pDevice, const GfxRayTracingTLASDesc& desc, const char* name);
            ~MockRayTracingTLAS();

            bool Create();

            virtual void* GetHandle() const override;
        };

    }  // namespace ngfx
}  // namespace ncore
#endif
