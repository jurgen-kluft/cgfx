#include "cgfx/gfx.h"

#include "cgfx/d3d12/d3d12_device.h"
#include "cgfx/d3d12/d3d12_descriptor.h"

#include "cgfx/metal/metal_device.h"
#include "cgfx/metal/metal_descriptor.h"

#include "cgfx/mock/mock_device.h"
#include "cgfx/mock/mock_descriptor.h"

namespace ncore
{
    namespace ngfx
    {
        descriptor_t* CreateShaderResourceView(device_t* device, texture_t* texture, const srv_desc_t& desc, const char* name)
        {
            resource_t*   resource   = CreateInstance<resource_t>(device, name);
            descriptor_t* descriptor = CreateComponent<resource_t, descriptor_t>(device, resource);
            switch (device->m_desc.backend)
            {
                case enums::Backend_D3D12: return nd3d12::CreateSrv(device, resource, descriptor, texture, desc);
                case enums::Backend_Metal: return nmetal::CreateSrv(device, resource, descriptor, texture, desc);
                case enums::Backend_Mock: return nmock::CreateSrv(device, resource, descriptor, texture, desc);
            }
            return descriptor;
        }

        descriptor_t* CreateShaderResourceView(device_t* device, buffer_t* buffer, const srv_desc_t& desc, const char* name)
        {
            resource_t*   resource   = CreateInstance<resource_t>(device, name);
            descriptor_t* descriptor = CreateComponent<resource_t, descriptor_t>(device, resource);
            switch (device->m_desc.backend)
            {
                case enums::Backend_D3D12: return nd3d12::CreateSrv(device, resource, descriptor, buffer, desc);
                case enums::Backend_Metal: return nmetal::CreateSrv(device, resource, descriptor, buffer, desc);
                case enums::Backend_Mock: return nmock::CreateSrv(device, resource, descriptor, buffer, desc);
            }
            return descriptor;
        }

        descriptor_t* CreateUnorderedAccessView(device_t* device, texture_t* texture, const uav_desc_t& desc, const char* name)
        {
            resource_t*   resource   = CreateInstance<resource_t>(device, name);
            descriptor_t* descriptor = CreateComponent<resource_t, descriptor_t>(device, resource);
            switch (device->m_desc.backend)
            {
                case enums::Backend_D3D12: return nd3d12::CreateUav(device, resource, descriptor, texture, desc);
                case enums::Backend_Metal: return nmetal::CreateUav(device, resource, descriptor, texture, desc);
                case enums::Backend_Mock: return nmock::CreateUav(device, resource, descriptor, texture, desc);
            }
            return descriptor;
        }

        descriptor_t* CreateUnorderedAccessView(device_t* device, buffer_t* buffer, const uav_desc_t& desc, const char* name)
        {
            resource_t*   resource   = CreateInstance<resource_t>(device, name);
            descriptor_t* descriptor = CreateComponent<resource_t, descriptor_t>(device, resource);
            switch (device->m_desc.backend)
            {
                case enums::Backend_D3D12: return nd3d12::CreateUav(device, resource, descriptor, buffer, desc);
                case enums::Backend_Metal: return nmetal::CreateUav(device, resource, descriptor, buffer, desc);
                case enums::Backend_Mock: return nmock::CreateUav(device, resource, descriptor, buffer, desc);
            }
            return descriptor;
        }

        descriptor_t* CreateConstantBufferView(device_t* device, buffer_t* buffer, const cbv_desc_t& desc, const char* name)
        {
            resource_t*   resource   = CreateInstance<resource_t>(device, name);
            descriptor_t* descriptor = CreateComponent<resource_t, descriptor_t>(device, resource);
            switch (device->m_desc.backend)
            {
                case enums::Backend_D3D12: return nd3d12::CreateCbv(device, resource, descriptor, buffer, desc);
                case enums::Backend_Metal: return nmetal::CreateCbv(device, resource, descriptor, buffer, desc);
                case enums::Backend_Mock: return nmock::CreateCbv(device, resource, descriptor, buffer, desc);
            }
            return descriptor;
        }

        descriptor_t* CreateSampler(device_t* device, const sampler_desc_t& desc, const char* name)
        {
            resource_t*   resource   = CreateInstance<resource_t>(device, name);
            descriptor_t* descriptor = CreateComponent<resource_t, descriptor_t>(device, resource);
            switch (device->m_desc.backend)
            {
                case enums::Backend_D3D12: return nd3d12::CreateSampler(device, resource, descriptor, desc);
                case enums::Backend_Metal: return nmetal::CreateSampler(device, resource, descriptor, desc);
                case enums::Backend_Mock: return nmock::CreateSampler(device, resource, descriptor, desc);
            }
            return descriptor;
        }

        void Destroy(device_t* device, descriptor_t* descriptor)
        {
            switch (device->m_desc.backend)
            {
                case enums::Backend_D3D12: nd3d12::Destroy(device, descriptor); break;
                case enums::Backend_Metal: nmetal::Destroy(device, descriptor); break;
                case enums::Backend_Mock: nmock::Destroy(device, descriptor); break;
            }
        }

        u32  GetHeapIndex(device_t* device, descriptor_t* descriptor)
        {
            switch (device->m_desc.backend)
            {
                case enums::Backend_D3D12: return nd3d12::GetHeapIndex(device, descriptor);
                case enums::Backend_Metal: return nmetal::GetHeapIndex(device, descriptor);
                case enums::Backend_Mock: return nmock::GetHeapIndex(device, descriptor);
            }
            return 0;
        }

    }  // namespace ngfx
}  // namespace ncore
