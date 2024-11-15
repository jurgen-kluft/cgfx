#include "cgfx/gfx.h"
#include "cgfx/gfx_resource.h"

#include "cgfx/d3d12/d3d12_rt_tlas.h"
#include "cgfx/metal/metal_rt_tlas.h"
#include "cgfx/mock/mock_rt_tlas.h"

#include "ccore/c_math.h"

namespace ncore
{
    namespace ngfx
    {
        tlas_t* CreateRayTracingBLAS(device_t* device, const tlas_desc_t& desc, const char* name)
        {
            resource_t* resource = CreateObject<resource_t>(device, name);
            tlas_t*     tlas     = AddComponent<resource_t, tlas_t>(device, resource);
            tlas->m_desc         = desc;
            return tlas;
        }

        bool Create(device_t* device, tlas_t* tlas)
        {
            resource_t* resource = GetObject<resource_t>(device, tlas);
            switch (device->m_desc.backend)
            {
                case enums::Backend_D3D12: nd3d12::Create(device, tlas); break;
                case enums::Backend_Metal: nmetal::Create(device, tlas); break;
                case enums::Backend_Mock: nmock::Create(device, tlas); break;
            }
            return false;
        }

        void Destroy(device_t* device, tlas_t* tlas)
        {
            switch (device->m_desc.backend)
            {
                case enums::Backend_D3D12: nd3d12::Destroy(device, tlas); break;
                case enums::Backend_Metal: nmetal::Destroy(device, tlas); break;
                case enums::Backend_Mock: nmock::Destroy(device, tlas); break;
            }
        }
    }  // namespace ngfx
}  // namespace ncore
