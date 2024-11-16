#ifndef __CGFX_D3D12_PIPELINE_STATE_H__
#define __CGFX_D3D12_PIPELINE_STATE_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
    #pragma once
#endif

// #include "cgfx/d3d12/d3d12_header.h"
#include "cgfx/gfx_pipeline_state.h"

namespace ncore
{
    namespace ngfx
    {
        namespace nd3d12
        {
            ngfx::pipeline_state_t* CreateGraphicsPipelineState(device_t* device, resource_t* resource, pipeline_state_t* ps, const graphics_pipeline_desc_t& desc);
            ngfx::pipeline_state_t* CreateMeshShadingPipelineState(device_t* device, resource_t* resource, pipeline_state_t* ps, const mesh_shading_pipeline_desc_t& desc);
            ngfx::pipeline_state_t* CreateComputePipelineState(device_t* device, resource_t* resource, pipeline_state_t* ps, const compute_pipeline_desc_t& desc);
            bool                    Create(device_t* device, ngfx::pipeline_state_t* ps);
            void                    Destroy(device_t* device, ngfx::pipeline_state_t* ps);
            void*                   GetHandle(device_t* device, ngfx::pipeline_state_t* ps);

        }  // namespace nd3d12

        // class D3D12Device;

        // class D3D12GraphicsPipelineState : public pipeline_state_t
        // {
        // public:
        //     D3D12GraphicsPipelineState(D3D12Device* pDevice, const graphics_pipeline_desc_t& desc, const char* name);
        //     ~D3D12GraphicsPipelineState();

        //     virtual void* GetHandle() const { return m_pPipelineState; }
        //     virtual bool  Create() override;

        //     D3D_PRIMITIVE_TOPOLOGY GetPrimitiveTopology() const { return m_primitiveTopology; }

        // private:
        //     ID3D12PipelineState*    m_pPipelineState = nullptr;
        //     graphics_pipeline_desc_t m_desc;
        //     D3D_PRIMITIVE_TOPOLOGY  m_primitiveTopology = D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
        // };

        // class D3D12MeshShadingPipelineState : public pipeline_state_t
        // {
        // public:
        //     D3D12MeshShadingPipelineState(D3D12Device* pDevice, const mesh_shading_pipeline_desc_t& desc, const char* name);
        //     ~D3D12MeshShadingPipelineState();

        //     virtual void* GetHandle() const { return m_pPipelineState; }
        //     virtual bool  Create() override;

        // private:
        //     ID3D12PipelineState*       m_pPipelineState = nullptr;
        //     mesh_shading_pipeline_desc_t m_desc;
        // };

        // class D3D12ComputePipelineState : public pipeline_state_t
        // {
        // public:
        //     D3D12ComputePipelineState(D3D12Device* pDevice, const compute_pipeline_desc_t& desc, const char* name);
        //     ~D3D12ComputePipelineState();

        //     virtual void* GetHandle() const { return m_pPipelineState; }
        //     virtual bool  Create() override;

        // private:
        //     ID3D12PipelineState*   m_pPipelineState = nullptr;
        //     compute_pipeline_desc_t m_desc;
        // };
    }  // namespace ngfx
}  // namespace ncore
#endif
