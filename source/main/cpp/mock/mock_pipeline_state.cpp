#include "cgfx/mock/mock_pipeline_state.h"
#include "cgfx/mock/mock_device.h"

namespace ncore
{
    namespace ngfx
    {
        namespace nmock
        {
            struct graphics_pipeline_state_t
            {
                D_GFX_OCS_COMPONENT_SET(enums::ComponentBackEndGraphicsPipelineState);
                graphics_pipeline_desc_t desc;
            };

            struct mesh_shading_pipeline_state_t
            {
                D_GFX_OCS_COMPONENT_SET(enums::ComponentBackEndMeshShadingPipelineState);
                mesh_shading_pipeline_desc_t desc;
            };

            struct compute_pipeline_state_t
            {
                D_GFX_OCS_COMPONENT_SET(enums::ComponentBackEndComputePipelineState);
                compute_pipeline_desc_t desc;
            };

            ngfx::pipeline_state_t* Alloc(device_t* device, resource_t* resource, pipeline_state_t* state, const graphics_pipeline_desc_t& desc)
            {
                graphics_pipeline_state_t* gpstate = AddComponent<resource_t, graphics_pipeline_state_t>(device, resource);
                gpstate->desc                      = desc;
                return state;
            }

            ngfx::pipeline_state_t* Alloc(device_t* device, resource_t* resource, pipeline_state_t* state, const mesh_shading_pipeline_desc_t& desc)
            {
                mesh_shading_pipeline_state_t* mspstate = AddComponent<resource_t, mesh_shading_pipeline_state_t>(device, resource);
                mspstate->desc                          = desc;
                return state;
            }

            ngfx::pipeline_state_t* Alloc(device_t* device, resource_t* resource, pipeline_state_t* state, const compute_pipeline_desc_t& desc)
            {
                compute_pipeline_state_t* cpstate = AddComponent<resource_t, compute_pipeline_state_t>(device, resource);
                cpstate->desc                     = desc;
                return state;
            }

            bool Create(device_t* device, ngfx::pipeline_state_t* state)
            {
                switch (state->m_type)
                {
                    case enums::PipelineGraphics:
                        {
                            graphics_pipeline_state_t* gpstate = GetOtherComponent<pipeline_state_t, graphics_pipeline_state_t>(device, state);
                            return gpstate != nullptr;
                        }
                    case enums::PipelineMeshShading:
                        {
                            mesh_shading_pipeline_state_t* mspstate = GetOtherComponent<pipeline_state_t, mesh_shading_pipeline_state_t>(device, state);
                            return mspstate != nullptr;
                        }
                    case enums::PipelineCompute:
                        {
                            compute_pipeline_state_t* cpstate = GetOtherComponent<pipeline_state_t, compute_pipeline_state_t>(device, state);
                            return cpstate != nullptr;
                        }
                }
                return false;
            }

            void Destroy(device_t* device, ngfx::pipeline_state_t* state)
            {
                switch (state->m_type)
                {
                    case enums::PipelineGraphics:
                        {
                            graphics_pipeline_state_t* gpstate = GetOtherComponent<pipeline_state_t, graphics_pipeline_state_t>(device, state);
                            if (gpstate != nullptr)
                            {
                                RemoveOtherComponent<pipeline_state_t, graphics_pipeline_state_t>(device, state);
                            }
                        }
                        break;
                    case enums::PipelineMeshShading:
                        {
                            mesh_shading_pipeline_state_t* mspstate = GetOtherComponent<pipeline_state_t, mesh_shading_pipeline_state_t>(device, state);
                            if (mspstate != nullptr)
                            {
                                RemoveOtherComponent<pipeline_state_t, mesh_shading_pipeline_state_t>(device, state);
                            }
                        }
                        break;
                    case enums::PipelineCompute:
                        {
                            compute_pipeline_state_t* cpstate = GetOtherComponent<pipeline_state_t, compute_pipeline_state_t>(device, state);
                            if (cpstate != nullptr)
                            {
                                RemoveOtherComponent<pipeline_state_t, compute_pipeline_state_t>(device, state);
                            }
                        }
                        break;
                }
            }

            void* GetHandle(device_t* device, ngfx::pipeline_state_t* state)
            {
                switch (state->m_type)
                {
                    case enums::PipelineGraphics:
                        {
                            graphics_pipeline_state_t* gpstate = GetOtherComponent<pipeline_state_t, graphics_pipeline_state_t>(device, state);
                            break;
                        }
                    case enums::PipelineMeshShading:
                        {
                            mesh_shading_pipeline_state_t* mspstate = GetOtherComponent<pipeline_state_t, mesh_shading_pipeline_state_t>(device, state);
                            break;
                        }
                    case enums::PipelineCompute:
                        {
                            compute_pipeline_state_t* cpstate = GetOtherComponent<pipeline_state_t, compute_pipeline_state_t>(device, state);
                            break;
                        }
                }
                return nullptr;
            }

        }  // namespace nmock
    }  // namespace ngfx
}  // namespace ncore
