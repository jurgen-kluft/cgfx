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
#ifdef TARGET_MAC
        namespace nmetal
        {
            struct graphics_pipeline_state_t
            {
                D_GFX_OCS_COMPONENT_SET(enums::ComponentMetalGraphicsPipelineState);
                graphics_pipeline_desc_t  m_desc;
                MTL::RenderPipelineState* m_pPSO               = nullptr;
                MTL::DepthStencilState*   m_pDepthStencilState = nullptr;
                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            struct meshshading_pipeline_state_t
            {
                D_GFX_OCS_COMPONENT_SET(enums::ComponentMetalMeshShadingPipelineState);
                mesh_shading_pipeline_desc_t m_desc;
                MTL::RenderPipelineState*    m_pPSO                        = nullptr;
                MTL::DepthStencilState*      m_pDepthStencilState          = nullptr;
                MTL::Size                    m_threadsPerObjectThreadgroup = {};
                MTL::Size                    m_threadsPerMeshThreadgroup   = {};
                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            struct compute_pipeline_state_t
            {
                D_GFX_OCS_COMPONENT_SET(enums::ComponentMetalComputePipelineState);
                compute_pipeline_desc_t    m_desc;
                MTL::ComputePipelineState* m_pPSO = nullptr;
                MTL::Size                  m_threadsPerThreadgroup;
                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            void CreateGraphicsPipelineState(ngfx::device_t* device, pipeline_state_t* ps, const graphics_pipeline_desc_t& desc);
            void CreateMeshShadingPipelineState(ngfx::device_t* device, pipeline_state_t* ps, const mesh_shading_pipeline_desc_t& desc);
            void CreateComputePipelineState(ngfx::device_t* device, pipeline_state_t* ps, const compute_pipeline_desc_t& desc);
            bool Create(ngfx::device_t* device, ngfx::pipeline_state_t* ps);
            void Destroy(ngfx::device_t* device, ngfx::pipeline_state_t* ps);

        }  // namespace nmetal
#else
        namespace nmetal
        {
            void CreateGraphicsPipelineState(ngfx::device_t* device, pipeline_state_t* ps, const graphics_pipeline_desc_t& desc) {}
            void CreateMeshShadingPipelineState(ngfx::device_t* device, pipeline_state_t* ps, const mesh_shading_pipeline_desc_t& desc) {}
            void CreateComputePipelineState(ngfx::device_t* device, pipeline_state_t* ps, const compute_pipeline_desc_t& desc) {}
            bool Create(ngfx::device_t* device, ngfx::pipeline_state_t* ps) { return false; }
            void Destroy(ngfx::device_t* device, ngfx::pipeline_state_t* ps) {}
        }  // namespace nmetal
#endif
    }  // namespace ngfx
}  // namespace ncore
#endif
