#include "cgfx/gfx.h"

#include "cgfx/d3d12/d3d12_device.h"
#include "cgfx/d3d12/d3d12_swapchain.h"

#include "cgfx/metal/metal_device.h"
#include "cgfx/metal/metal_swapchain.h"

#include "cgfx/mock/mock_device.h"
#include "cgfx/mock/mock_swapchain.h"

#include "ccore/c_math.h"

namespace ncore
{
    namespace ngfx
    {
        swapchain_t* CreateSwapchain(device_t* device, const swapchain_desc_t& desc, const char* name)
        {
            resource_t*  resource = CreateInstance<resource_t>(device, name);
            swapchain_t* sc       = CreateComponent<resource_t, swapchain_t>(device, resource);
            sc->m_desc            = desc;
            switch (device->m_desc.backend)
            {
                case enums::Backend_D3D12: return nd3d12::CreateSwapchain(device, sc, desc);
                case enums::Backend_Metal: return nmetal::CreateSwapchain(device, sc, desc);
                case enums::Backend_Mock: return nmock::CreateSwapchain(device, sc, desc);
            }
            return sc;
        }

        bool Create(device_t* device, swapchain_t* resource)
        {
            switch (device->m_desc.backend)
            {
                case enums::Backend_D3D12: return nd3d12::Create(device, resource);
                case enums::Backend_Metal: return nmetal::Create(device, resource);
                case enums::Backend_Mock: return nmock::Create(device, resource);
            }
            return false;
        }

        void Destroy(device_t* device, swapchain_t* resource)
        {
            switch (device->m_desc.backend)
            {
                case enums::Backend_D3D12: nd3d12::Destroy(device, resource); break;
                case enums::Backend_Metal: nmetal::Destroy(device, resource); break;
                case enums::Backend_Mock: nmock::Destroy(device, resource); break;
            }
        }

        void AcquireNextBackBuffer(device_t* device, swapchain_t* sc)
        {
            switch (device->m_desc.backend)
            {
                case enums::Backend_D3D12: nd3d12::AcquireNextBackBuffer(device, sc); break;
                case enums::Backend_Metal: nmetal::AcquireNextBackBuffer(device, sc); break;
                case enums::Backend_Mock: nmock::AcquireNextBackBuffer(device, sc); break;
            }
        }

        texture_t* GetBackBuffer(device_t* device, swapchain_t* sc)
        {
            switch (device->m_desc.backend)
            {
                case enums::Backend_D3D12: return nd3d12::GetBackBuffer(device, sc);
                case enums::Backend_Metal: return nmetal::GetBackBuffer(device, sc);
                case enums::Backend_Mock: return nmock::GetBackBuffer(device, sc);
            }
            return nullptr;
        }

        bool Resize(device_t* device, swapchain_t* sc, u32 width, u32 height)
        {
            switch (device->m_desc.backend)
            {
                case enums::Backend_D3D12: return nd3d12::Resize(device, sc, width, height);
                case enums::Backend_Metal: return nmetal::Resize(device, sc, width, height);
                case enums::Backend_Mock: return nmock::Resize(device, sc, width, height);
            }
            return false;
        }

        void SetVSyncEnabled(device_t* device, swapchain_t* sc, bool value)
        {
            switch (device->m_desc.backend)
            {
                case enums::Backend_D3D12: nd3d12::SetVSyncEnabled(device, sc, value); break;
                case enums::Backend_Metal: nmetal::SetVSyncEnabled(device, sc, value); break;
                case enums::Backend_Mock: nmock::SetVSyncEnabled(device, sc, value); break;
            }
        }

    }  // namespace ngfx
}  // namespace ncore
