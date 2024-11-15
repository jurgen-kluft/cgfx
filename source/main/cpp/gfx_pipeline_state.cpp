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
            pipeline_state_t* state    = AddComponent<resource_t, pipeline_state_t>(device, resource);

            switch (device->m_desc.backend)
            {
                case enums::Backend_D3D12: nd3d12::CreateGraphicsPipelineState(device, resource, state, desc); break;
                case enums::Backend_Metal: nmetal::CreateGraphicsPipelineState(device, resource, state, desc); break;
                case enums::Backend_Mock: nmock::CreateGraphicsPipelineState(device, resource, state, desc); break;
            }
            return state;
        }

        pipeline_state_t* CreateMeshShadingPipelineState(device_t* device, const mesh_shading_pipeline_desc_t& desc, const char* name)
        {
            resource_t*       resource = CreateObject<resource_t>(device, name);
            pipeline_state_t* state    = AddComponent<resource_t, pipeline_state_t>(device, resource);

            switch (device->m_desc.backend)
            {
                case enums::Backend_D3D12: nd3d12::CreateMeshShadingPipelineState(device, resource, state, desc); break;
                case enums::Backend_Metal: nmetal::CreateMeshShadingPipelineState(device, resource, state, desc); break;
                case enums::Backend_Mock: nmock::CreateMeshShadingPipelineState(device, resource, state, desc); break;
            }
            return state;
        }

        pipeline_state_t* CreateComputePipelineState(device_t* device, const compute_pipeline_desc_t& desc, const char* name)
        {
            resource_t*       resource = CreateObject<resource_t>(device, name);
            pipeline_state_t* state    = AddComponent<resource_t, pipeline_state_t>(device, resource);

            switch (device->m_desc.backend)
            {
                case enums::Backend_D3D12: nd3d12::CreateComputePipelineState(device, resource, state, desc); break;
                case enums::Backend_Metal: nmetal::CreateComputePipelineState(device, resource, state, desc); break;
                case enums::Backend_Mock: nmock::CreateComputePipelineState(device, resource, state, desc); break;
            }
            return state;
        }

    }  // namespace ngfx
}  // namespace ncore
