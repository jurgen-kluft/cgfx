#include "cgfx/mock/mock_rt_tlas.h"
#include "cgfx/mock/mock_device.h"

namespace ncore
{
    namespace ngfx
    {
        MockRayTracingTLAS::MockRayTracingTLAS(MockDevice* pDevice, const GfxRayTracingTLASDesc& desc, const eastl::string& name)
        {
            m_pDevice = pDevice;
            m_desc    = desc;
            m_name    = name;
        }

        MockRayTracingTLAS::~MockRayTracingTLAS() {}

        bool MockRayTracingTLAS::Create() { return true; }

        void* MockRayTracingTLAS::GetHandle() const { return nullptr; }

    }  // namespace ngfx
}  // namespace ncore
