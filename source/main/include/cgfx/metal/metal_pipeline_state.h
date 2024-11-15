#ifndef __CGFX_METAL_PIPELINE_STATE_H__
#define __CGFX_METAL_PIPELINE_STATE_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
    #pragma once
#endif

// #include "cgfx/metal/metal_utils.h"
#include "cgfx/gfx_pipeline_state.h"

namespace ncore
{
    namespace ngfx
    {
        namespace nmetal
        {
            ngfx::pipeline_state_t* CreateGraphicsPipelineState(device_t* device, resource_t* resource, pipeline_state_t* state, const graphics_pipeline_desc_t& desc);
            ngfx::pipeline_state_t* CreateMeshShadingPipelineState(device_t* device, resource_t* resource, pipeline_state_t* state, const mesh_shading_pipeline_desc_t& desc);
            ngfx::pipeline_state_t* CreateComputePipelineState(device_t* device, resource_t* resource, pipeline_state_t* state, const compute_pipeline_desc_t& desc);
            bool                    Create(device_t* device, ngfx::pipeline_state_t* state);
            void                    Destroy(device_t* device, ngfx::pipeline_state_t* state);
            void*                   GetHandle(device_t* device, ngfx::pipeline_state_t* state);

        }  // namespace nmetal

        // class MetalDevice;

        // class MetalGraphicsPipelineState : public pipeline_state_t
        // {
        // public:
        //     MetalGraphicsPipelineState(MetalDevice* pDevice, const graphics_pipeline_desc_t& desc, const char* name);
        //     ~MetalGraphicsPipelineState();

        //     const graphics_pipeline_desc_t& GetDesc() const { return m_desc; }
        //     MTL::DepthStencilState*        GetDepthStencilState() const { return m_pDepthStencilState; }

        //     virtual void* GetHandle() const override { return m_pPSO; }
        //     virtual bool  Create() override;

        // private:
        //     graphics_pipeline_desc_t   m_desc;
        //     MTL::RenderPipelineState* m_pPSO               = nullptr;
        //     MTL::DepthStencilState*   m_pDepthStencilState = nullptr;
        // };

        // class MetalMeshShadingPipelineState : public pipeline_state_t
        // {
        // public:
        //     MetalMeshShadingPipelineState(MetalDevice* pDevice, const mesh_shading_pipeline_desc_t& desc, const char* name);
        //     ~MetalMeshShadingPipelineState();

        //     const mesh_shading_pipeline_desc_t& GetDesc() const { return m_desc; }
        //     MTL::DepthStencilState*           GetDepthStencilState() const { return m_pDepthStencilState; }
        //     MTL::Size                         GetThreadsPerObjectThreadgroup() const { return m_threadsPerObjectThreadgroup; }
        //     MTL::Size                         GetThreadsPerMeshThreadgroup() const { return m_threadsPerMeshThreadgroup; }

        //     virtual void* GetHandle() const override { return m_pPSO; }
        //     virtual bool  Create() override;

        // private:
        //     mesh_shading_pipeline_desc_t m_desc;
        //     MTL::RenderPipelineState*  m_pPSO                        = nullptr;
        //     MTL::DepthStencilState*    m_pDepthStencilState          = nullptr;
        //     MTL::Size                  m_threadsPerObjectThreadgroup = {};
        //     MTL::Size                  m_threadsPerMeshThreadgroup   = {};
        // };

        // class MetalComputePipelineState : public pipeline_state_t
        // {
        // public:
        //     MetalComputePipelineState(MetalDevice* pDevice, const compute_pipeline_desc_t& desc, const char* name);
        //     ~MetalComputePipelineState();

        //     MTL::Size GetThreadsPerThreadgroup() const { return m_threadsPerThreadgroup; }

        //     virtual void* GetHandle() const override { return m_pPSO; }
        //     virtual bool  Create() override;

        // private:
        //     compute_pipeline_desc_t     m_desc;
        //     MTL::ComputePipelineState* m_pPSO = nullptr;
        //     MTL::Size                  m_threadsPerThreadgroup;
        // };

    }  // namespace ngfx
}  // namespace ncore
#endif
