#include "cgfx/gfx.h"

#include "cgfx/d3d12/d3d12_device.h"
#include "cgfx/metal/metal_device.h"
#include "cgfx/mock/mock_device.h"

namespace ncore
{
    namespace ngfx
    {
        void RegisterComponents(cs_alloc_t* cs_alloc, u32 max_instances)
        {
            // TODO: tune the component count:
            // - max textures
            // - max buffers
            // - max descriptors
            // ...
            cs_alloc->register_component<name_t>(max_instances);
            cs_alloc->register_component<resource_t>(max_instances);
            cs_alloc->register_component<device_t>(1);
            cs_alloc->register_component<texture_t>(max_instances);
            cs_alloc->register_component<buffer_t>(max_instances);
            cs_alloc->register_component<fence_t>(max_instances);
            cs_alloc->register_component<swapchain_t>(8);
            cs_alloc->register_component<command_list_t>(64);
            cs_alloc->register_component<resource_t>(max_instances);
            cs_alloc->register_component<shader_t>(max_instances);
            cs_alloc->register_component<pipeline_state_t>(max_instances);
            cs_alloc->register_component<descriptor_t>(max_instances);
            cs_alloc->register_component<heap_t>(max_instances); // ?
            cs_alloc->register_component<blas_t>(max_instances);
            cs_alloc->register_component<tlas_t>(max_instances);
        }

        device_t* CreateDevice(alloc_t* main_alloc, stack_alloc_t* stack_alloc, frame_alloc_t* frame_alloc, const device_desc_t& desc)
        {
            const u32 max_instances = 4096;

            cs_alloc_t* cs_alloc = g_allocate<cs_alloc_t>(main_alloc);
            RegisterComponents(cs_alloc, max_instances);

            ngfx::resource_t* device_resource = cs_alloc->new_instance<ngfx::resource_t>();
            ngfx::device_t*   pDevice         = cs_alloc->create_component<ngfx::resource_t, ngfx::device_t>(device_resource);
            pDevice->m_frameID                = 0;
            pDevice->m_desc                   = desc;
            pDevice->m_stack_alloc            = stack_alloc;
            pDevice->m_frame_alloc            = frame_alloc;
            pDevice->m_main_alloc             = main_alloc;
            pDevice->m_cs_alloc               = cs_alloc;

            // NOTE: The switch statement here are currently here for convenience, once we have
            //       refactored all the backend specific code into their respective files, and
            //       all is functional can we lockdown the compilation to only allow one backend.
            //       Furthermore, all gfx objects have this switch statement, so we can refactor
            //       them all at once.

            switch (desc.backend)
            {
                case enums::Backend_D3D12:
                    pDevice->m_vendor = enums::VendorNvidia;
                    AttachName(pDevice, device_resource, "D3D12 Device");
                    nd3d12::CreateDevice(pDevice, max_instances);
                    break;
                case enums::Backend_Metal:
                    pDevice->m_vendor = enums::VendorApple;
                    AttachName(pDevice, device_resource, "Metal Device");
                    nmetal::CreateDevice(pDevice, max_instances);
                    break;
                case enums::Backend_Mock:
                    pDevice->m_vendor = enums::VendorMock;
                    AttachName(pDevice, device_resource, "Mock Device");
                    nmock::CreateDevice(pDevice, max_instances);
                    break;

                default: break;
            }

            if (pDevice && !Create(pDevice))
            {
                g_deallocate(main_alloc, pDevice);
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

        bool DumpMemoryStats(device_t* device, const char* file)
        {
            switch (device->m_desc.backend)
            {
                case enums::Backend_D3D12: return nd3d12::DumpMemoryStats(device, file);
                case enums::Backend_Metal: return nmetal::DumpMemoryStats(device, file);
                case enums::Backend_Mock: return nmock::DumpMemoryStats(device, file);
            }
            return false;
        }

    }  // namespace ngfx
}  // namespace ncore
