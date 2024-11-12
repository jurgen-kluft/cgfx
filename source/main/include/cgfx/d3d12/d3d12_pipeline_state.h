#ifndef __CGFX_D3D12_PIPELINE_STATE_H__
#define __CGFX_D3D12_PIPELINE_STATE_H__

#include "cgfx/d3d12/d3d12_header.h"
#include "cgfx/gfx_pipeline_state.h"

namespace ncore
{
    namespace ngfx
    {
        class D3D12Device;

        class D3D12GraphicsPipelineState : public IGfxPipelineState
        {
        public:
            D3D12GraphicsPipelineState(D3D12Device* pDevice, const GfxGraphicsPipelineDesc& desc, const char* name);
            ~D3D12GraphicsPipelineState();

            virtual void* GetHandle() const { return m_pPipelineState; }
            virtual bool  Create() override;

            D3D_PRIMITIVE_TOPOLOGY GetPrimitiveTopology() const { return m_primitiveTopology; }

        private:
            ID3D12PipelineState*    m_pPipelineState = nullptr;
            GfxGraphicsPipelineDesc m_desc;
            D3D_PRIMITIVE_TOPOLOGY  m_primitiveTopology = D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
        };

        class D3D12MeshShadingPipelineState : public IGfxPipelineState
        {
        public:
            D3D12MeshShadingPipelineState(D3D12Device* pDevice, const GfxMeshShadingPipelineDesc& desc, const char* name);
            ~D3D12MeshShadingPipelineState();

            virtual void* GetHandle() const { return m_pPipelineState; }
            virtual bool  Create() override;

        private:
            ID3D12PipelineState*       m_pPipelineState = nullptr;
            GfxMeshShadingPipelineDesc m_desc;
        };

        class D3D12ComputePipelineState : public IGfxPipelineState
        {
        public:
            D3D12ComputePipelineState(D3D12Device* pDevice, const GfxComputePipelineDesc& desc, const char* name);
            ~D3D12ComputePipelineState();

            virtual void* GetHandle() const { return m_pPipelineState; }
            virtual bool  Create() override;

        private:
            ID3D12PipelineState*   m_pPipelineState = nullptr;
            GfxComputePipelineDesc m_desc;
        };
    }  // namespace ngfx
}  // namespace ncore
#endif
