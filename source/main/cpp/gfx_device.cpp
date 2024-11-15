#include "cgfx/gfx.h"

#include "cgfx/d3d12/d3d12_device.h"
#include "cgfx/metal/metal_device.h"
#include "cgfx/mock/mock_device.h"

namespace ncore
{
    namespace ngfx
    {
        u16 const resource_t::object_index   = 0;
        u16 const name_t::component_index    = 0;
        u16 const texture_t::component_index = 0;
        u16 const buffer_t::component_index  = 1;
        u16 const device_t::component_index  = 1;

        // ... TODO: other components

        void InitializeOcs(nocs::allocator_t* ocs)
        {
            // register object
            ocs->register_object<resource_t>(2048, 256, 64);

            // register components
            ocs->register_component<resource_t, name_t>(2048, "name");
            ocs->register_component<resource_t, texture_t>(2048, "texture");
            ocs->register_component<resource_t, buffer_t>(2048, "buffer");
            ocs->register_component<resource_t, device_t>(4, "device");

            // ... TODO: register other components
        }

        device_t* CreateDevice(alloc_t* allocator, const device_desc_t& desc)
        {
            nocs::allocator_t* ocs = g_allocate<nocs::allocator_t>(allocator);
            InitializeOcs(ocs);

            ngfx::resource_t* device_resource = ocs->create_object<ngfx::resource_t>();
            ngfx::device_t*   pDevice         = ocs->add_component<ngfx::device_t>(device_resource);
            pDevice->m_frameID                = 0;
            pDevice->m_desc                   = desc;
            pDevice->m_allocator              = allocator;
            pDevice->m_allocatorOCS           = ocs;

            // NOTE: The switch statement here are currently here for convenience, once we have
            //       refactored all the backend specific code into their respective files, and
            //       all is functional can we lockdown the compilation to only allow one backend.
            //       Furthermore, all gfx objects have this switch statement, so we can refactor
            //       them all at once.

            switch (desc.backend)
            {
#ifdef TARGET_PC
                case enums::BackendD3D12:
                    pDevice->m_vendor = enums::Vendor_NVIDIA;
                    // pDevice           = nd3d12::AllocDevice(allocator, desc);
                    AttachName(pDevice, device_resource, "D3D12 Device");
                    break;
#endif
#ifdef TARGET_MAC
                case enums::Backend_Metal:
                    pDevice->m_vendor = enums::VendorApple;
                    // pDevice           = nmetal::AllocDevice(allocator, desc);
                    AttachName(pDevice, device_resource, "Metal Device");
                    break;
#endif
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
