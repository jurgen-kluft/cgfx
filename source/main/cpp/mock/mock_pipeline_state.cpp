#include "cgfx/mock/mock_pipeline_state.h"
#include "cgfx/mock/mock_device.h"

namespace ncore
{
    namespace ngfx
    {
        namespace nmock
        {
            struct graphics_ps_t
            {
                D_GFX_OCS_COMPONENT_SET(enums::ComponentBackEndGraphicsPipelineState);
                graphics_pipeline_desc_t desc;
            };

            struct mesh_shading_ps_t
            {
                D_GFX_OCS_COMPONENT_SET(enums::ComponentBackEndMeshShadingPipelineState);
                mesh_shading_pipeline_desc_t desc;
            };

            struct compute_ps_t
            {
                D_GFX_OCS_COMPONENT_SET(enums::ComponentBackEndComputePipelineState);
                compute_pipeline_desc_t desc;
            };

            ngfx::pipeline_state_t* CreateGraphicsPipelineState(device_t* device, resource_t* resource, pipeline_state_t* ps, const graphics_pipeline_desc_t& desc)
            {
                graphics_ps_t* gpstate = AddComponent<resource_t, graphics_ps_t>(device, resource);
                gpstate->desc          = desc;
                return ps;
            }

            ngfx::pipeline_state_t* CreateMeshShadingPipelineState(device_t* device, resource_t* resource, pipeline_state_t* ps, const mesh_shading_pipeline_desc_t& desc)
            {
                mesh_shading_ps_t* mspstate = AddComponent<resource_t, mesh_shading_ps_t>(device, resource);
                mspstate->desc              = desc;
                return ps;
            }

            ngfx::pipeline_state_t* CreateComputePipelineState(device_t* device, resource_t* resource, pipeline_state_t* ps, const compute_pipeline_desc_t& desc)
            {
                compute_ps_t* cpstate = AddComponent<resource_t, compute_ps_t>(device, resource);
                cpstate->desc         = desc;
                return ps;
            }

            bool Create(device_t* device, ngfx::pipeline_state_t* ps)
            {
                switch (ps->m_type)
                {
                    case enums::PipelineGraphics:
                        {
                            graphics_ps_t* gpstate = GetOtherComponent<pipeline_state_t, graphics_ps_t>(device, ps);
                            return gpstate != nullptr;
                        }
                    case enums::PipelineMeshShading:
                        {
                            mesh_shading_ps_t* mspstate = GetOtherComponent<pipeline_state_t, mesh_shading_ps_t>(device, ps);
                            return mspstate != nullptr;
                        }
                    case enums::PipelineCompute:
                        {
                            compute_ps_t* cpstate = GetOtherComponent<pipeline_state_t, compute_ps_t>(device, ps);
                            return cpstate != nullptr;
                        }
                }
                return false;
            }

            void Destroy(device_t* device, ngfx::pipeline_state_t* ps)
            {
                switch (ps->m_type)
                {
                    case enums::PipelineGraphics:
                        {
                            graphics_ps_t* gpstate = GetOtherComponent<pipeline_state_t, graphics_ps_t>(device, ps);
                            if (gpstate != nullptr)
                            {
                                RemoveOtherComponent<pipeline_state_t, graphics_ps_t>(device, ps);
                            }
                        }
                        break;
                    case enums::PipelineMeshShading:
                        {
                            mesh_shading_ps_t* mspstate = GetOtherComponent<pipeline_state_t, mesh_shading_ps_t>(device, ps);
                            if (mspstate != nullptr)
                            {
                                RemoveOtherComponent<pipeline_state_t, mesh_shading_ps_t>(device, ps);
                            }
                        }
                        break;
                    case enums::PipelineCompute:
                        {
                            compute_ps_t* cpstate = GetOtherComponent<pipeline_state_t, compute_ps_t>(device, ps);
                            if (cpstate != nullptr)
                            {
                                RemoveOtherComponent<pipeline_state_t, compute_ps_t>(device, ps);
                            }
                        }
                        break;
                }
            }

            void* GetHandle(device_t* device, ngfx::pipeline_state_t* ps)
            {
                switch (ps->m_type)
                {
                    case enums::PipelineGraphics:
                        {
                            graphics_ps_t* gpstate = GetOtherComponent<pipeline_state_t, graphics_ps_t>(device, ps);
                            break;
                        }
                    case enums::PipelineMeshShading:
                        {
                            mesh_shading_ps_t* mspstate = GetOtherComponent<pipeline_state_t, mesh_shading_ps_t>(device, ps);
                            break;
                        }
                    case enums::PipelineCompute:
                        {
                            compute_ps_t* cpstate = GetOtherComponent<pipeline_state_t, compute_ps_t>(device, ps);
                            break;
                        }
                }
                return nullptr;
            }

        }  // namespace nmock
    }  // namespace ngfx
}  // namespace ncore
