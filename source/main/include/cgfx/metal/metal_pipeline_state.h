#ifndef __CGFX_METAL_PIPELINE_STATE_H__
#define __CGFX_METAL_PIPELINE_STATE_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
    #pragma once
#endif

#include "cgfx/metal/metal_utils.h"
#include "cgfx/gfx_pipeline_state.h"

namespace ncore
{
    namespace ngfx
    {
        namespace nmetal
        {
            struct graphics_pipeline_state_t
            {
                graphics_pipeline_desc_t  m_desc;
                MTL::RenderPipelineState* m_pPSO               = nullptr;
                MTL::DepthStencilState*   m_pDepthStencilState = nullptr;
            };

            struct meshshading_pipeline_state_t
            {
                mesh_shading_pipeline_desc_t m_desc;
                MTL::RenderPipelineState*    m_pPSO                        = nullptr;
                MTL::DepthStencilState*      m_pDepthStencilState          = nullptr;
                MTL::Size                    m_threadsPerObjectThreadgroup = {};
                MTL::Size                    m_threadsPerMeshThreadgroup   = {};
            };

            struct compute_pipeline_state_t
            {
                compute_pipeline_desc_t    m_desc;
                MTL::ComputePipelineState* m_pPSO = nullptr;
                MTL::Size                  m_threadsPerThreadgroup;
            };

            pipeline_state_t* CreateGraphicsPipelineState(ngfx::device_t* device, resource_t* resource, pipeline_state_t* ps, const graphics_pipeline_desc_t& desc);
            pipeline_state_t* CreateMeshShadingPipelineState(ngfx::device_t* device, resource_t* resource, pipeline_state_t* ps, const mesh_shading_pipeline_desc_t& desc);
            pipeline_state_t* CreateComputePipelineState(ngfx::device_t* device, resource_t* resource, pipeline_state_t* ps, const compute_pipeline_desc_t& desc);
            bool              Create(ngfx::device_t* device, ngfx::pipeline_state_t* ps);
            void              Destroy(ngfx::device_t* device, ngfx::pipeline_state_t* ps);
            void*             GetHandle(ngfx::device_t* device, ngfx::pipeline_state_t* ps);

        }  // namespace nmetal
    }  // namespace ngfx
}  // namespace ncore
#endif
