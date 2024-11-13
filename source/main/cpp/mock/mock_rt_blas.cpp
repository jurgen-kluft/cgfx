#include "cgfx/mock/mock_rt_blas.h"
#include "cgfx/mock/mock_device.h"

namespace ncore
{
    namespace ngfx
    {

        MockRayTracingBLAS::MockRayTracingBLAS(MockDevice* pDevice, const GfxRayTracing::BLASDesc& desc, const char* name)
        {
            m_pDevice = pDevice;
            m_desc    = desc;
            m_name    = name;
        }

        MockRayTracingBLAS::~MockRayTracingBLAS() {}

        bool MockRayTracingBLAS::Create() { return true; }

        void* MockRayTracingBLAS::GetHandle() const { return nullptr; }

    }  // namespace ngfx
}  // namespace ncore
