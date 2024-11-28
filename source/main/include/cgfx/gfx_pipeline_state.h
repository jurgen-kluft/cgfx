#ifndef __CGFX_GFX_PIPELINE_STATE_H__
#define __CGFX_GFX_PIPELINE_STATE_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
    #pragma once
#endif

#include "cgfx/gfx_resource.h"

namespace ncore
{
    namespace ngfx
    {
        struct pipeline_state_t
        {
            D_GFX_CS_COMPONENT_SET(enums::ComponentPipelineState);
            enums::pipeline_t m_type;
            DCORE_CLASS_PLACEMENT_NEW_DELETE
        };

        pipeline_state_t* CreateGraphicsPipelineState(device_t* device, const graphics_pipeline_desc_t& desc, const char* name);
        pipeline_state_t* CreateMeshShadingPipelineState(device_t* device, const mesh_shading_pipeline_desc_t& desc, const char* name);
        pipeline_state_t* CreateComputePipelineState(device_t* device, const compute_pipeline_desc_t& desc, const char* name);
        bool              Create(device_t* device, pipeline_state_t*);
        void              Destroy(device_t* device, pipeline_state_t* resource);

    }  // namespace ngfx
}  // namespace ncore

#endif
