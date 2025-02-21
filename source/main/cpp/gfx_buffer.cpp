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
            buffer_t*   buffer   = CreateInstance<buffer_t>(device, name);
            buffer->m_desc       = desc;
            switch (device->m_desc.backend)
            {
                case enums::Backend_D3D12: nd3d12::CreateBuffer(device, buffer); break;
                case enums::Backend_Metal: nmetal::CreateBuffer(device, buffer); break;
                case enums::Backend_Mock: nmock::CreateBuffer(device, buffer); break;
            }
            return buffer;
        }

        bool Create(device_t* device, buffer_t* buffer)
        {
            switch (device->m_desc.backend)
            {
                case enums::Backend_D3D12: nd3d12::Create(device, buffer); break;
                case enums::Backend_Metal: nmetal::Create(device, buffer); break;
                case enums::Backend_Mock: nmock::Create(device, buffer); break;
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
                case enums::Backend_D3D12: nd3d12::GetCpuAddress(device, buffer); break;
                case enums::Backend_Metal: nmetal::GetCpuAddress(device, buffer); break;
                case enums::Backend_Mock: nmock::GetCpuAddress(device, buffer); break;
            }
            return nullptr;
        }

        u64 GetGpuAddress(device_t* device, buffer_t* buffer)
        {
            switch (device->m_desc.backend)
            {
                case enums::Backend_D3D12: nd3d12::GetGpuAddress(device, buffer); break;
                case enums::Backend_Metal: nmetal::GetGpuAddress(device, buffer); break;
                case enums::Backend_Mock: nmock::GetGpuAddress(device, buffer); break;
            }
            return 0;
        }

        u32 GetRequiredStagingBufferSize(device_t* device, buffer_t* buffer)
        {
            switch (device->m_desc.backend)
            {
                case enums::Backend_D3D12: nd3d12::GetRequiredStagingBufferSize(device, buffer); break;
                case enums::Backend_Metal: nmetal::GetRequiredStagingBufferSize(device, buffer); break;
                case enums::Backend_Mock: nmock::GetRequiredStagingBufferSize(device, buffer); break;
            }
            return 0;
        }

    }  // namespace ngfx
}  // namespace ncore
