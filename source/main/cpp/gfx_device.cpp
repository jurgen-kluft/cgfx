#include "cgfx/gfx.h"

#include "cgfx/d3d12/d3d12_device.h"
#include "cgfx/metal/metal_device.h"
#include "cgfx/mock/mock_device.h"

namespace ncore
{
    namespace ngfx
    {
        void InitializeOcs(ncs::allocator_t* cs)
        {
            // register components
            cs->register_component<name_t>(2048);
            cs->register_component<texture_t>(2048);
            cs->register_component<buffer_t>(2048);
            cs->register_component<device_t>(4);

            // ... TODO: register other components
        }

        device_t* CreateDevice(alloc_t* allocator, const device_desc_t& desc)
        {
            ncs::allocator_t* cs = g_allocate<ncs::allocator_t>(allocator);
            InitializeOcs(cs);

            ngfx::resource_t* device_resource = cs->new_instance<ngfx::resource_t>();
            ngfx::device_t*   pDevice         = cs->create_component<ngfx::resource_t, ngfx::device_t>(device_resource);
            pDevice->m_frameID                = 0;
            pDevice->m_desc                   = desc;
            pDevice->m_allocator              = allocator;
            pDevice->m_allocatorCS            = cs;

            // NOTE: The switch statement here are currently here for convenience, once we have
            //       refactored all the backend specific code into their respective files, and
            //       all is functional can we lockdown the compilation to only allow one backend.
            //       Furthermore, all gfx objects have this switch statement, so we can refactor
            //       them all at once.

            switch (desc.backend)
            {
                case enums::Backend_D3D12:
                    pDevice->m_vendor = enums::VendorNvidia;
                    // pDevice           = nd3d12::AllocDevice(allocator, desc);
                    AttachName(pDevice, device_resource, "D3D12 Device");
                    break;
                case enums::Backend_Metal:
                    pDevice->m_vendor = enums::VendorApple;
                    // pDevice           = nmetal::AllocDevice(allocator, desc);
                    AttachName(pDevice, device_resource, "Metal Device");
                    break;
                case enums::Backend_Mock:
                    pDevice->m_vendor = enums::VendorMock;
                    // pDevice           = nmock::AllocDevice(allocator, desc);
                    AttachName(pDevice, device_resource, "Mock Device");
                    break;

                default: break;
            }

            if (pDevice && !Create(pDevice))
            {
                g_deallocate(allocator, pDevice);
                pDevice = nullptr;
            }

            return pDevice;
        }

        bool Create(device_t* pDevice)
        {
            switch (pDevice->m_desc.backend)
            {
                case enums::Backend_D3D12: return nd3d12::Create(pDevice);
                case enums::Backend_Metal: return nmetal::Create(pDevice);
                case enums::Backend_Mock: return nmock::Create(pDevice);
            }
            return false;
        }

        void* GetHandle(device_t* device)
        {
            switch (device->m_desc.backend)
            {
                case enums::Backend_D3D12: return nd3d12::GetHandle(device);
                case enums::Backend_Metal: return nmetal::GetHandle(device);
                case enums::Backend_Mock: return nmock::GetHandle(device);
            }
            return nullptr;
        }

        void BeginFrame(device_t* device)
        {
            switch (device->m_desc.backend)
            {
                case enums::Backend_D3D12: nd3d12::BeginFrame(device); break;
                case enums::Backend_Metal: nmetal::BeginFrame(device); break;
                case enums::Backend_Mock: nmock::BeginFrame(device); break;
            }
        }

        void EndFrame(device_t* device)
        {
            switch (device->m_desc.backend)
            {
                case enums::Backend_D3D12: nd3d12::EndFrame(device); break;
                case enums::Backend_Metal: nmetal::EndFrame(device); break;
                case enums::Backend_Mock: nmock::EndFrame(device); break;
            }
        }

    }  // namespace ngfx
}  // namespace ncore
