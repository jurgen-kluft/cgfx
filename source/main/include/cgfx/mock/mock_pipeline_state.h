#ifndef __CGFX_MOCK_PIPELINE_STATE_H__
#define __CGFX_MOCK_PIPELINE_STATE_H__

#include "cgfx/gfx_pipeline_state.h"

namespace ncore
{
    namespace ngfx
    {
        namespace nmock
        {
            ngfx::pipeline_state_t* CreateGraphicsPipelineState(device_t* device, resource_t* resource, pipeline_state_t* state, const graphics_pipeline_desc_t& desc);
            ngfx::pipeline_state_t* CreateMeshShadingPipelineState(device_t* device, resource_t* resource, pipeline_state_t* state, const mesh_shading_pipeline_desc_t& desc);
            ngfx::pipeline_state_t* CreateComputePipelineState(device_t* device, resource_t* resource, pipeline_state_t* state, const compute_pipeline_desc_t& desc);
            bool                    Create(device_t* device, ngfx::pipeline_state_t* state);
            void                    Destroy(device_t* device, ngfx::pipeline_state_t* state);
            void*                   GetHandle(device_t* device, ngfx::pipeline_state_t* state);

        }  // namespace nmock

    }  // namespace ngfx
}  // namespace ncore
#endif
