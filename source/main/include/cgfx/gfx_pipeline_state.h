#ifndef __CGFX_GFX_PIPELINE_STATE_H__
#define __CGFX_GFX_PIPELINE_STATE_H__

#include "cgfx/gfx_resource.h"

namespace ncore
{
    namespace ngfx
    {
        struct pipeline_state_t
        {
            D_GFX_OCS_COMPONENT;
            enums::pipeline_t m_type;
        };

        pipeline_state_t* CreateGraphicsPipelineState(device_t* device, const graphics_pipeline_desc_t& desc, const char* name);
        pipeline_state_t* CreateMeshShadingPipelineState(device_t* device, const mesh_shading_pipeline_desc_t& desc, const char* name);
        pipeline_state_t* CreateComputePipelineState(device_t* device, const compute_pipeline_desc_t& desc, const char* name);
        void              Destroy(device_t* device, pipeline_state_t* resource);
        bool              Create(device_t* device, pipeline_state_t*);

    }  // namespace ngfx
}  // namespace ncore

#endif
