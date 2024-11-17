#include "cgfx/gfx.h"
#include "cgfx/gfx_resource.h"

#include "cgfx/d3d12/d3d12_rt_blas.h"
#include "cgfx/metal/metal_rt_blas.h"
#include "cgfx/mock/mock_rt_blas.h"

#include "ccore/c_math.h"

namespace ncore
{
    namespace ngfx
    {
        blas_t* CreateRayTracingBLAS(device_t* device, const blas_desc_t& desc, const char* name)
        {
            resource_t* resource = CreateObject<resource_t>(device, name);
            blas_t*     blas     = AddComponent<resource_t, blas_t>(device, resource);
            blas->m_desc         = desc;
            return blas;
        }

        bool Create(device_t* device, blas_t* blas)
        {
            switch (device->m_desc.backend)
            {
                case enums::Backend_D3D12: nd3d12::Create(device, blas); break;
                case enums::Backend_Metal: nmetal::Create(device, blas); break;
                case enums::Backend_Mock: nmock::Create(device, blas); break;
            }
            return false;
        }

        void Destroy(device_t* device, blas_t* blas)
        {
            switch (device->m_desc.backend)
            {
                case enums::Backend_D3D12: nd3d12::Destroy(device, blas); break;
                case enums::Backend_Metal: nmetal::Destroy(device, blas); break;
                case enums::Backend_Mock: nmock::Destroy(device, blas); break;
            }
        }

        void* GetHandle(ngfx::device_t* pDevice, const ngfx::blas_t* pBLAS)
        {
            switch (pDevice->m_desc.backend)
            {
                case enums::Backend_D3D12: return nd3d12::GetHandle(pDevice, pBLAS);
                case enums::Backend_Metal: return nmetal::GetHandle(pDevice, pBLAS);
                case enums::Backend_Mock: return nmock::GetHandle(pDevice, pBLAS);
            }
            return nullptr;
        }

    }  // namespace ngfx
}  // namespace ncore