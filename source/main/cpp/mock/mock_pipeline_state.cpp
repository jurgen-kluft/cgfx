#include "cgfx/mock/mock_pipeline_state.h"
#include "cgfx/mock/mock_device.h"

namespace ncore
{
    namespace ngfx
    {

        MockGraphicsPipelineState::MockGraphicsPipelineState(MockDevice* pDevice, const GfxGraphicsPipelineDesc& desc, const char* name)
        {
            m_pDevice = pDevice;
            m_name    = name;
            m_desc    = desc;
            m_type    = GfxPipelineType::Graphics;
        }

        MockGraphicsPipelineState::~MockGraphicsPipelineState() {}

        void* MockGraphicsPipelineState::GetHandle() const { return nullptr; }

        bool MockGraphicsPipelineState::Create() { return true; }

        MockMeshShadingPipelineState::MockMeshShadingPipelineState(MockDevice* pDevice, const GfxMeshShadingPipelineDesc& desc, const char* name)
        {
            m_pDevice = pDevice;
            m_name    = name;
            m_desc    = desc;
            m_type    = GfxPipelineType::MeshShading;
        }

        MockMeshShadingPipelineState::~MockMeshShadingPipelineState() {}

        void* MockMeshShadingPipelineState::GetHandle() const { return nullptr; }

        bool MockMeshShadingPipelineState::Create() { return true; }

        MockComputePipelineState::MockComputePipelineState(MockDevice* pDevice, const GfxComputePipelineDesc& desc, const char* name)
        {
            m_pDevice = pDevice;
            m_name    = name;
            m_desc    = desc;
            m_type    = GfxPipelineType::Compute;
        }

        MockComputePipelineState::~MockComputePipelineState() {}

        void* MockComputePipelineState::GetHandle() const { return nullptr; }

        bool MockComputePipelineState::Create() { return true; }

    }  // namespace ngfx
}  // namespace ncore
