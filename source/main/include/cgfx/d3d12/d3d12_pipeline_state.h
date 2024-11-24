#ifndef __CGFX_D3D12_PIPELINE_STATE_H__
#define __CGFX_D3D12_PIPELINE_STATE_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
    #pragma once
#endif

#include "cgfx/gfx_pipeline_state.h"
#include "cgfx/d3d12/d3d12_header.h"

namespace ncore
{
    namespace ngfx
    {
#ifdef TARGET_PC
        namespace nd3d12
        {
            struct graphics_pipeline_state_t
            {
                D_GFX_OCS_COMPONENT_SET(enums::ComponentD3D12GraphicsPipelineState);
                ID3D12PipelineState*     m_pPipelineState = nullptr;
                graphics_pipeline_desc_t m_desc;
                D3D_PRIMITIVE_TOPOLOGY   m_primitiveTopology = D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            struct meshshading_pipeline_state_t
            {
                D_GFX_OCS_COMPONENT_SET(enums::ComponentD3D12MeshShadingPipelineState);
                ID3D12PipelineState*         m_pPipelineState = nullptr;
                mesh_shading_pipeline_desc_t m_desc;
                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            struct compute_pipeline_state_t
            {
                D_GFX_OCS_COMPONENT_SET(enums::ComponentD3D12ComputePipelineState);
                ID3D12PipelineState*    m_pPipelineState = nullptr;
                compute_pipeline_desc_t m_desc;
                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            void  CreateGraphicsPipelineState(ngfx::device_t* device, ngfx::pipeline_state_t* ps, const graphics_pipeline_desc_t& desc);
            void  CreateMeshShadingPipelineState(ngfx::device_t* device, ngfx::pipeline_state_t* ps, const mesh_shading_pipeline_desc_t& desc);
            void  CreateComputePipelineState(ngfx::device_t* device, ngfx::pipeline_state_t* ps, const compute_pipeline_desc_t& desc);
            void  DestroyPipelineState(ngfx::device_t* device, ngfx::pipeline_state_t* ps);
            bool  Create(ngfx::device_t* device, ngfx::pipeline_state_t* ps);
            void  Destroy(ngfx::device_t* device, ngfx::pipeline_state_t* ps);
            void* GetHandle(ngfx::device_t* device, ngfx::pipeline_state_t* ps);
        }  // namespace nd3d12
#else
        namespace nd3d12
        {
            void  CreateGraphicsPipelineState(ngfx::device_t* device, ngfx::pipeline_state_t* ps, const graphics_pipeline_desc_t& desc) {}
            void  CreateMeshShadingPipelineState(ngfx::device_t* device, ngfx::pipeline_state_t* ps, const mesh_shading_pipeline_desc_t& desc) {}
            void  CreateComputePipelineState(ngfx::device_t* device, ngfx::pipeline_state_t* ps, const compute_pipeline_desc_t& desc) {}
            void  DestroyPipelineState(ngfx::device_t* device, ngfx::pipeline_state_t* ps) {}
            bool  Create(ngfx::device_t* device, ngfx::pipeline_state_t* ps) { return false; }
            void  Destroy(ngfx::device_t* device, ngfx::pipeline_state_t* ps) {}
            void* GetHandle(ngfx::device_t* device, ngfx::pipeline_state_t* ps) { return nullptr; }
        }  // namespace nd3d12
#endif
    }  // namespace ngfx
}  // namespace ncore
#endif
