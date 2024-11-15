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
        buffer_t* Alloc(device_t* pDevice, const buffer_desc_t& desc, const char* name)
        {
            resource_t* pResource = CreateObject<resource_t>(pDevice, name);
            buffer_t*   pBuffer   = AddComponent<resource_t, buffer_t>(pDevice, pResource);
            return pBuffer;
        }

        bool Create(device_t* pDevice, buffer_t* pBuffer)
        {
            switch (pDevice->m_desc.backend)
            {
                case enums::Backend_D3D12: return nd3d12::Create(pDevice, pBuffer);
                case enums::Backend_Metal: return nmetal::Create(pDevice, pBuffer);
                case enums::Backend_Mock: return nmock::Create(pDevice, pBuffer);
            }
            return false;
        }

        void Destroy(device_t* pDevice, buffer_t* pBuffer)
        {
            switch (pDevice->m_desc.backend)
            {
                case enums::Backend_D3D12: nd3d12::Destroy(pDevice, pBuffer); break;
                case enums::Backend_Metal: nmetal::Destroy(pDevice, pBuffer); break;
                case enums::Backend_Mock: nmock::Destroy(pDevice, pBuffer); break;
            }
        }

        void* GetCpuAddress(device_t* pDevice, buffer_t* buffer)
        {
            switch (pDevice->m_desc.backend)
            {
                case enums::Backend_D3D12: return nd3d12::GetCpuAddress(pDevice, buffer);
                case enums::Backend_Metal: return nmetal::GetCpuAddress(pDevice, buffer);
                case enums::Backend_Mock: return nmock::GetCpuAddress(pDevice, buffer);
            }
            return nullptr;
        }

        u64 GetGpuAddress(device_t* pDevice, buffer_t* buffer)
        {
            switch (pDevice->m_desc.backend)
            {
                case enums::Backend_D3D12: return nd3d12::GetGpuAddress(pDevice, buffer);
                case enums::Backend_Metal: return nmetal::GetGpuAddress(pDevice, buffer);
                case enums::Backend_Mock: return nmock::GetGpuAddress(pDevice, buffer);
            }
            return 0;
        }

        u32 GetRequiredStagingBufferSize(device_t* pDevice, buffer_t* buffer)
        {
            switch (pDevice->m_desc.backend)
            {
                case enums::Backend_D3D12: return nd3d12::GetRequiredStagingBufferSize(pDevice, buffer);
                case enums::Backend_Metal: return nmetal::GetRequiredStagingBufferSize(pDevice, buffer);
                case enums::Backend_Mock: return nmock::GetRequiredStagingBufferSize(pDevice, buffer);
            }
            return 0;
        }

    }  // namespace ngfx
}  // namespace ncore
