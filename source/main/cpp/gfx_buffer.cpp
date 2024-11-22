#include "cgfx/gfx.h"

#include "cgfx/d3d12/d3d12_device.h"
#include "cgfx/d3d12/d3d12_buffer.h"

#include "cgfx/metal/metal_device.h"
#include "cgfx/metal/metal_buffer.h"

#include "cgfx/mock/mock_device.h"
#include "cgfx/mock/mock_buffer.h"

namespace ncore
{
    namespace ngfx
    {
        buffer_t* CreateBuffer(device_t* device, const buffer_desc_t& desc, const char* name)
        {
            resource_t* resource = CreateInstance<resource_t>(device, name);
            buffer_t*   buffer   = CreateComponent<resource_t, buffer_t>(device, resource);
            buffer->m_desc       = desc;
            switch (device->m_desc.backend)
            {
                case enums::Backend_D3D12: return nd3d12::CreateBuffer(device, buffer);
                case enums::Backend_Metal: return nmetal::CreateBuffer(device, buffer);
                case enums::Backend_Mock: return nmock::CreateBuffer(device, buffer);
            }
            return buffer;
        }

        bool Create(device_t* device, buffer_t* buffer)
        {
            switch (device->m_desc.backend)
            {
                case enums::Backend_D3D12: return nd3d12::Create(device, buffer);
                case enums::Backend_Metal: return nmetal::Create(device, buffer);
                case enums::Backend_Mock: return nmock::Create(device, buffer);
            }
            return false;
        }

        void Destroy(device_t* device, buffer_t* buffer)
        {
            switch (device->m_desc.backend)
            {
                case enums::Backend_D3D12: nd3d12::Destroy(device, buffer); break;
                case enums::Backend_Metal: nmetal::Destroy(device, buffer); break;
                case enums::Backend_Mock: nmock::Destroy(device, buffer); break;
            }
        }

        void* GetCpuAddress(device_t* device, buffer_t* buffer)
        {
            switch (device->m_desc.backend)
            {
                case enums::Backend_D3D12: return nd3d12::GetCpuAddress(device, buffer);
                case enums::Backend_Metal: return nmetal::GetCpuAddress(device, buffer);
                case enums::Backend_Mock: return nmock::GetCpuAddress(device, buffer);
            }
            return nullptr;
        }

        u64 GetGpuAddress(device_t* device, buffer_t* buffer)
        {
            switch (device->m_desc.backend)
            {
                case enums::Backend_D3D12: return nd3d12::GetGpuAddress(device, buffer);
                case enums::Backend_Metal: return nmetal::GetGpuAddress(device, buffer);
                case enums::Backend_Mock: return nmock::GetGpuAddress(device, buffer);
            }
            return 0;
        }

        u32 GetRequiredStagingBufferSize(device_t* device, buffer_t* buffer)
        {
            switch (device->m_desc.backend)
            {
                case enums::Backend_D3D12: return nd3d12::GetRequiredStagingBufferSize(device, buffer);
                case enums::Backend_Metal: return nmetal::GetRequiredStagingBufferSize(device, buffer);
                case enums::Backend_Mock: return nmock::GetRequiredStagingBufferSize(device, buffer);
            }
            return 0;
        }

    }  // namespace ngfx
}  // namespace ncore
