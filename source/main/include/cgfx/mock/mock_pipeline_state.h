#ifndef __CGFX_MOCK_PIPELINE_STATE_H__
#define __CGFX_MOCK_PIPELINE_STATE_H__

#include "cgfx/gfx_pipeline_state.h"

namespace ncore
{
    namespace ngfx
    {
        namespace nmock
        {
            struct graphics_ps_t
            {
                D_GFX_CS_COMPONENT_SET(enums::ComponentMockGraphicsPipelineState);
                graphics_pipeline_desc_t desc;
                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            struct mesh_shading_ps_t
            {
                D_GFX_CS_COMPONENT_SET(enums::ComponentMockMeshShadingPipelineState);
                mesh_shading_pipeline_desc_t desc;
                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            struct compute_ps_t
            {
                D_GFX_CS_COMPONENT_SET(enums::ComponentMockComputePipelineState);
                compute_pipeline_desc_t desc;
                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            ngfx::pipeline_state_t* CreateGraphicsPipelineState(device_t* device, pipeline_state_t* ps, const graphics_pipeline_desc_t& desc);
            ngfx::pipeline_state_t* CreateMeshShadingPipelineState(device_t* device, pipeline_state_t* ps, const mesh_shading_pipeline_desc_t& desc);
            ngfx::pipeline_state_t* CreateComputePipelineState(device_t* device, pipeline_state_t* ps, const compute_pipeline_desc_t& desc);
            bool                    Create(device_t* device, ngfx::pipeline_state_t* ps);
            void                    Destroy(device_t* device, ngfx::pipeline_state_t* ps);
        }  // namespace nmock
    }  // namespace ngfx
}  // namespace ncore
#endif
