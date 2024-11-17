#include "cgfx/gfx.h"

#include "cgfx/d3d12/d3d12_pipeline_state.h"
#include "cgfx/metal/metal_pipeline_state.h"
#include "cgfx/mock/mock_pipeline_state.h"

namespace ncore
{
    namespace ngfx
    {
        pipeline_state_t* CreateGraphicsPipelineState(device_t* device, const graphics_pipeline_desc_t& desc, const char* name)
        {
            resource_t*       resource = CreateObject<resource_t>(device, name);
            pipeline_state_t* ps       = AddComponent<resource_t, pipeline_state_t>(device, resource);

            switch (device->m_desc.backend)
            {
                case enums::Backend_D3D12: nd3d12::CreateGraphicsPipelineState(device, resource, ps, desc); break;
                case enums::Backend_Metal: nmetal::CreateGraphicsPipelineState(device, resource, ps, desc); break;
                case enums::Backend_Mock: nmock::CreateGraphicsPipelineState(device, resource, ps, desc); break;
            }
            return ps;
        }

        pipeline_state_t* CreateMeshShadingPipelineState(device_t* device, const mesh_shading_pipeline_desc_t& desc, const char* name)
        {
            resource_t*       resource = CreateObject<resource_t>(device, name);
            pipeline_state_t* ps       = AddComponent<resource_t, pipeline_state_t>(device, resource);

            switch (device->m_desc.backend)
            {
                case enums::Backend_D3D12: nd3d12::CreateMeshShadingPipelineState(device, resource, ps, desc); break;
                case enums::Backend_Metal: nmetal::CreateMeshShadingPipelineState(device, resource, ps, desc); break;
                case enums::Backend_Mock: nmock::CreateMeshShadingPipelineState(device, resource, ps, desc); break;
            }
            return ps;
        }

        pipeline_state_t* CreateComputePipelineState(device_t* device, const compute_pipeline_desc_t& desc, const char* name)
        {
            resource_t*       resource = CreateObject<resource_t>(device, name);
            pipeline_state_t* ps       = AddComponent<resource_t, pipeline_state_t>(device, resource);

            switch (device->m_desc.backend)
            {
                case enums::Backend_D3D12: nd3d12::CreateComputePipelineState(device, resource, ps, desc); break;
                case enums::Backend_Metal: nmetal::CreateComputePipelineState(device, resource, ps, desc); break;
                case enums::Backend_Mock: nmock::CreateComputePipelineState(device, resource, ps, desc); break;
            }
            return ps;
        }

        bool Create(device_t* device, pipeline_state_t* ps)
        {
            switch (device->m_desc.backend)
            {
                case enums::Backend_D3D12: return nd3d12::Create(device, ps);
                case enums::Backend_Metal: return nmetal::Create(device, ps);
                case enums::Backend_Mock: return nmock::Create(device, ps);
            }
            return false;
        }

        void Destroy(device_t* device, pipeline_state_t* ps)
        {
            switch (device->m_desc.backend)
            {
                case enums::Backend_D3D12: nd3d12::Destroy(device, ps); break;
                case enums::Backend_Metal: nmetal::Destroy(device, ps); break;
                case enums::Backend_Mock: nmock::Destroy(device, ps); break;
            }
        }

        void* GetHandle(device_t* device, pipeline_state_t* ps)
        {
            switch (device->m_desc.backend)
            {
                case enums::Backend_D3D12: return nd3d12::GetHandle(device, ps);
                case enums::Backend_Metal: return nmetal::GetHandle(device, ps);
                case enums::Backend_Mock: return nmock::GetHandle(device, ps);
            }
            return nullptr;
        }

    }  // namespace ngfx
}  // namespace ncore