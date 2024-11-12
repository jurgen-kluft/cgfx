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

        class MetalDevice;

        class MetalGraphicsPipelineState : public IGfxPipelineState
        {
        public:
            MetalGraphicsPipelineState(MetalDevice* pDevice, const GfxGraphicsPipelineDesc& desc, const char* name);
            ~MetalGraphicsPipelineState();

            const GfxGraphicsPipelineDesc& GetDesc() const { return m_desc; }
            MTL::DepthStencilState*        GetDepthStencilState() const { return m_pDepthStencilState; }

            virtual void* GetHandle() const override { return m_pPSO; }
            virtual bool  Create() override;

        private:
            GfxGraphicsPipelineDesc   m_desc;
            MTL::RenderPipelineState* m_pPSO               = nullptr;
            MTL::DepthStencilState*   m_pDepthStencilState = nullptr;
        };

        class MetalMeshShadingPipelineState : public IGfxPipelineState
        {
        public:
            MetalMeshShadingPipelineState(MetalDevice* pDevice, const GfxMeshShadingPipelineDesc& desc, const char* name);
            ~MetalMeshShadingPipelineState();

            const GfxMeshShadingPipelineDesc& GetDesc() const { return m_desc; }
            MTL::DepthStencilState*           GetDepthStencilState() const { return m_pDepthStencilState; }
            MTL::Size                         GetThreadsPerObjectThreadgroup() const { return m_threadsPerObjectThreadgroup; }
            MTL::Size                         GetThreadsPerMeshThreadgroup() const { return m_threadsPerMeshThreadgroup; }

            virtual void* GetHandle() const override { return m_pPSO; }
            virtual bool  Create() override;

        private:
            GfxMeshShadingPipelineDesc m_desc;
            MTL::RenderPipelineState*  m_pPSO                        = nullptr;
            MTL::DepthStencilState*    m_pDepthStencilState          = nullptr;
            MTL::Size                  m_threadsPerObjectThreadgroup = {};
            MTL::Size                  m_threadsPerMeshThreadgroup   = {};
        };

        class MetalComputePipelineState : public IGfxPipelineState
        {
        public:
            MetalComputePipelineState(MetalDevice* pDevice, const GfxComputePipelineDesc& desc, const char* name);
            ~MetalComputePipelineState();

            MTL::Size GetThreadsPerThreadgroup() const { return m_threadsPerThreadgroup; }

            virtual void* GetHandle() const override { return m_pPSO; }
            virtual bool  Create() override;

        private:
            GfxComputePipelineDesc     m_desc;
            MTL::ComputePipelineState* m_pPSO = nullptr;
            MTL::Size                  m_threadsPerThreadgroup;
        };

    }  // namespace ngfx
}  // namespace ncore
#endif
