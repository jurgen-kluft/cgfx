#include "cgfx/mock/mock_pipeline_state.h"
#include "cgfx/mock/mock_device.h"

namespace ncore
{
    namespace ngfx
    {
        namespace nmock
        {
            ngfx::pipeline_state_t* CreateGraphicsPipelineState(device_t* device, pipeline_state_t* ps, const graphics_pipeline_desc_t& desc)
            {
                graphics_ps_t* gpstate = CreateComponent<pipeline_state_t, graphics_ps_t>(device, ps);
                gpstate->desc          = desc;
                return ps;
            }

            ngfx::pipeline_state_t* CreateMeshShadingPipelineState(device_t* device, pipeline_state_t* ps, const mesh_shading_pipeline_desc_t& desc)
            {
                mesh_shading_ps_t* mspstate = CreateComponent<pipeline_state_t, mesh_shading_ps_t>(device, ps);
                mspstate->desc              = desc;
                return ps;
            }

            ngfx::pipeline_state_t* CreateComputePipelineState(device_t* device, pipeline_state_t* ps, const compute_pipeline_desc_t& desc)
            {
                compute_ps_t* cpstate = CreateComponent<pipeline_state_t, compute_ps_t>(device, ps);
                cpstate->desc         = desc;
                return ps;
            }

            bool Create(device_t* device, ngfx::pipeline_state_t* ps)
            {
                switch (ps->m_type)
                {
                    case enums::PipelineGraphics:
                        {
                            graphics_ps_t* gpstate = GetComponent<pipeline_state_t, graphics_ps_t>(device, ps);
                            return gpstate != nullptr;
                        }
                    case enums::PipelineMeshShading:
                        {
                            mesh_shading_ps_t* mspstate = GetComponent<pipeline_state_t, mesh_shading_ps_t>(device, ps);
                            return mspstate != nullptr;
                        }
                    case enums::PipelineCompute:
                        {
                            compute_ps_t* cpstate = GetComponent<pipeline_state_t, compute_ps_t>(device, ps);
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
                            graphics_ps_t* gpstate = GetComponent<pipeline_state_t, graphics_ps_t>(device, ps);
                            if (gpstate != nullptr)
                            {
                                DestroyComponent<pipeline_state_t, graphics_ps_t>(device, ps);
                            }
                        }
                        break;
                    case enums::PipelineMeshShading:
                        {
                            mesh_shading_ps_t* mspstate = GetComponent<pipeline_state_t, mesh_shading_ps_t>(device, ps);
                            if (mspstate != nullptr)
                            {
                                DestroyComponent<pipeline_state_t, mesh_shading_ps_t>(device, ps);
                            }
                        }
                        break;
                    case enums::PipelineCompute:
                        {
                            compute_ps_t* cpstate = GetComponent<pipeline_state_t, compute_ps_t>(device, ps);
                            if (cpstate != nullptr)
                            {
                                DestroyComponent<pipeline_state_t, compute_ps_t>(device, ps);
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
                            graphics_ps_t* gpstate = GetComponent<pipeline_state_t, graphics_ps_t>(device, ps);
                            break;
                        }
                    case enums::PipelineMeshShading:
                        {
                            mesh_shading_ps_t* mspstate = GetComponent<pipeline_state_t, mesh_shading_ps_t>(device, ps);
                            break;
                        }
                    case enums::PipelineCompute:
                        {
                            compute_ps_t* cpstate = GetComponent<pipeline_state_t, compute_ps_t>(device, ps);
                            break;
                        }
                }
                return nullptr;
            }

        }  // namespace nmock
    }  // namespace ngfx
}  // namespace ncore
