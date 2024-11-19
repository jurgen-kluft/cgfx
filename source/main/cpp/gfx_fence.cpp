#include "cgfx/gfx.h"

#include "cgfx/d3d12/d3d12_device.h"
#include "cgfx/d3d12/d3d12_fence.h"

#include "cgfx/metal/metal_device.h"
#include "cgfx/metal/metal_fence.h"

#include "cgfx/mock/mock_device.h"
#include "cgfx/mock/mock_fence.h"

namespace ncore
{
    namespace ngfx
    {
        fence_t* CreateFence(device_t* device, const char* name)
        {
            resource_t* resource = CreateInstance<resource_t>(device, name);
            fence_t*    fence    = CreateComponent<resource_t, fence_t>(device, resource);
            return fence;
        }

        void Destroy(device_t* device, fence_t* resource)
        {
            switch (device->m_desc.backend)
            {
                case enums::Backend_D3D12: nd3d12::Destroy(device, resource); break;
                case enums::Backend_Metal: nmetal::Destroy(device, resource); break;
                case enums::Backend_Mock: nmock::Destroy(device, resource); break;
            }
        }

        void Wait(device_t* device, fence_t* fence, u64 value)
        {
            switch (device->m_desc.backend)
            {
                case enums::Backend_D3D12: nd3d12::Wait(device,fence, value); break;
                case enums::Backend_Metal: nmetal::Wait(device,fence, value); break;
                case enums::Backend_Mock: nmock::Wait(device,fence, value); break;
            }
        }

        void Signal(device_t* device, fence_t* fence, u64 value)
        {
            switch (device->m_desc.backend)
            {
                case enums::Backend_D3D12: nd3d12::Signal(device,fence, value); break;
                case enums::Backend_Metal: nmetal::Signal(device,fence, value); break;
                case enums::Backend_Mock: nmock::Signal(device,fence, value); break;
            }
        }

    }  // namespace ngfx
}  // namespace ncore
