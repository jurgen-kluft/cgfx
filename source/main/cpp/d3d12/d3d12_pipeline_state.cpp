#include "cgfx/d3d12/d3d12_pipeline_state.h"
#include "cgfx/d3d12/d3d12_device.h"
#include "cgfx/d3d12/d3d12_shader.h"

namespace ncore
{
    namespace ngfx
    {
        template <class T>
        inline bool has_rt_binding(const T& desc)
        {
            for (int i = 0; i < 8; ++i)
            {
                if (desc.rt_format[i] != Gfx::Unknown)
                {
                    return true;
                }
            }
            return false;
        }

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

            void CreateGraphicsPipelineState(ngfx::device_t* device, ngfx::pipeline_state_t* ps, const graphics_pipeline_desc_t& desc)
            {
                nd3d12::graphics_pipeline_state_t* pPipelineState = CreateComponent<ngfx::pipeline_state_t, nd3d12::graphics_pipeline_state_t>(device, ps);
                pPipelineState->m_desc                            = desc;
                pPipelineState->m_primitiveTopology               = d3d12_primitive_topology(desc.primitive_type);
            }

            void CreateMeshShadingPipelineState(ngfx::device_t* device, ngfx::pipeline_state_t* ps, const mesh_shading_pipeline_desc_t& desc)
            {
                nd3d12::meshshading_pipeline_state_t* pPipelineState = CreateComponent<ngfx::pipeline_state_t, nd3d12::meshshading_pipeline_state_t>(device, ps);
                pPipelineState->m_desc                               = desc;
            }

            void CreateComputePipelineState(ngfx::device_t* device, ngfx::pipeline_state_t* ps, const compute_pipeline_desc_t& desc)
            {
                nd3d12::compute_pipeline_state_t* pPipelineState = CreateComponent<ngfx::pipeline_state_t, nd3d12::compute_pipeline_state_t>(device, ps);
                pPipelineState->m_desc                           = desc;
            }

            void DestroyPipelineState(ngfx::device_t* device, ngfx::pipeline_state_t* ps)
            {
                Destroy(device, ps);

                if (ps->m_type == enums::PipelineGraphics)
                {
                    DestroyComponent<ngfx::pipeline_state_t, nd3d12::graphics_pipeline_state_t>(device, ps);
                }
                else if (ps->m_type == enums::PipelineMeshShading)
                {
                    DestroyComponent<ngfx::pipeline_state_t, nd3d12::meshshading_pipeline_state_t>(device, ps);
                }
                else if (ps->m_type == enums::PipelineCompute)
                {
                    DestroyComponent<ngfx::pipeline_state_t, nd3d12::compute_pipeline_state_t>(device, ps);
                }
            }

            bool Create(ngfx::device_t* device, ngfx::pipeline_state_t* ps)
            {
                nd3d12::device_t* pDevice = GetComponent<ngfx::device_t, nd3d12::device_t>(device, device);
                if (ps->m_type == enums::PipelineGraphics)
                {
                    nd3d12::graphics_pipeline_state_t* pPipelineState = GetComponent<ngfx::pipeline_state_t, nd3d12::graphics_pipeline_state_t>(device, ps);

                    D3D12_GRAPHICS_PIPELINE_STATE_DESC desc = {};
                    desc.pRootSignature                     = pDevice->m_pDevice->GetRootSignature();

                    desc.VS = GetByteCode(device, pPipelineState->m_desc.vs);
                    if (pPipelineState->m_desc.ps)
                    {
                        desc.PS = GetByteCode(device, pPipelineState->m_desc.ps);
                    }

                    desc.RasterizerState   = d3d12_rasterizer_desc(pPipelineState->m_desc.rasterizer_state);
                    desc.DepthStencilState = d3d12_depth_stencil_desc(pPipelineState->m_desc.depthstencil_state);
                    desc.BlendState        = d3d12_blend_desc(pPipelineState->m_desc.blend_state);

                    desc.NumRenderTargets = has_rt_binding(pPipelineState->m_desc) ? 8 : 0;
                    for (int i = 0; i < 8; ++i)
                    {
                        desc.RTVFormats[i] = dxgi_format(pPipelineState->m_desc.rt_format[i]);
                    }
                    desc.DSVFormat             = dxgi_format(pPipelineState->m_desc.depthstencil_format);
                    desc.PrimitiveTopologyType = d3d12_topology_type(pPipelineState->m_desc.primitive_type);
                    desc.SampleMask            = 0xFFFFFFFF;
                    desc.SampleDesc.Count      = 1;

                    ID3D12PipelineState* pipelineState = nullptr;
                    if (FAILED(pDevice->m_pDevice->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&pipelineState))))
                    {
                        // RE_ERROR("[D3D12GraphicsPipelineState] failed to create {}", m_name);
                        return false;
                    }
                }
                else if (ps->m_type == enums::PipelineMeshShading)
                {
                    nd3d12::meshshading_pipeline_state_t* pPipelineState = GetComponent<ngfx::pipeline_state_t, nd3d12::meshshading_pipeline_state_t>(device, ps);

                    D3DX12_MESH_SHADER_PIPELINE_STATE_DESC psoDesc = {};
                    psoDesc.pRootSignature                         = pDevice->m_pDevice->GetRootSignature();

                    if (pPipelineState->m_desc.as)
                    {
                        psoDesc.AS = GetByteCode(device, pPipelineState->m_desc.as);
                    }

                    psoDesc.MS = GetByteCode(device, pPipelineState->m_desc.ms);

                    if (pPipelineState->m_desc.ps)
                    {
                        psoDesc.PS = GetByteCode(device, pPipelineState->m_desc.ps);
                    }

                    psoDesc.RasterizerState   = d3d12_rasterizer_desc(pPipelineState->m_desc.rasterizer_state);
                    psoDesc.DepthStencilState = d3d12_depth_stencil_desc(pPipelineState->m_desc.depthstencil_state);
                    psoDesc.BlendState        = d3d12_blend_desc(pPipelineState->m_desc.blend_state);

                    psoDesc.NumRenderTargets = has_rt_binding(pPipelineState->m_desc) ? 8 : 0;
                    for (int i = 0; i < 8; ++i)
                    {
                        psoDesc.RTVFormats[i] = dxgi_format(pPipelineState->m_desc.rt_format[i]);
                    }
                    psoDesc.DSVFormat        = dxgi_format(pPipelineState->m_desc.depthstencil_format);
                    psoDesc.SampleMask       = 0xFFFFFFFF;
                    psoDesc.SampleDesc.Count = 1;

                    auto psoStream = CD3DX12_PIPELINE_MESH_STATE_STREAM(psoDesc);

                    D3D12_PIPELINE_STATE_STREAM_DESC streamDesc;
                    streamDesc.pPipelineStateSubobjectStream = &psoStream;
                    streamDesc.SizeInBytes                   = sizeof(psoStream);

                    ID3D12PipelineState* pipelineState = nullptr;
                    HRESULT              hr            = pDevice->m_pDevice->CreatePipelineState(&streamDesc, IID_PPV_ARGS(&pipelineState));
                    if (FAILED(hr))
                    {
                        // RE_ERROR("[D3D12MeshShadingPipelineState] failed to create {}", m_name);
                        return false;
                    }

                    // TODO set name
                    // pipelineState->SetName(string_to_wstring(m_name).c_str());

                    if (pPipelineState->m_pPipelineState)
                    {
                        pDevice->m_pDevice->Delete(pPipelineState->m_pPipelineState);
                    }

                    pPipelineState->m_pPipelineState = pipelineState;
                    return true;
                }
                else if (ps->m_type == enums::PipelineCompute)
                {
                    nd3d12::compute_pipeline_state_t* pPipelineState = GetComponent<ngfx::pipeline_state_t, nd3d12::compute_pipeline_state_t>(device, ps);

                    D3D12_COMPUTE_PIPELINE_STATE_DESC desc = {};
                    desc.pRootSignature                    = pDevice->m_pDevice->GetRootSignature();
                    desc.CS                                = GetByteCode(device, m_desc.cs);

                    ID3D12PipelineState* pipelineState = nullptr;
                    if (FAILED(pDevice->m_pDevice->CreateComputePipelineState(&desc, IID_PPV_ARGS(&pipelineState))))
                    {
                        // RE_ERROR("[D3D12ComputePipelineState] failed to create {}", m_name);
                        return false;
                    }

                    // TODO set name
                    // pipelineState->SetName(string_to_wstring(m_name).c_str());

                    if (pPipelineState->m_pPipelineState)
                    {
                        pDevice->m_pDevice->Delete(pPipelineState->m_pPipelineState);
                    }

                    pPipelineState->m_pPipelineState = pipelineState;
                    return true;
                }
            }

            void Destroy(ngfx::device_t* device, ngfx::pipeline_state_t* ps)
            {
                nd3d12::device_t* pDevice = GetComponent<ngfx::device_t, nd3d12::device_t>(device, device);
                if (ps->m_type == enums::PipelineGraphics)
                {
                    nd3d12::graphics_pipeline_state_t* pPipelineState = GetComponent<ngfx::pipeline_state_t, nd3d12::graphics_pipeline_state_t>(device, ps);
                    if (pPipelineState->m_pPipelineState)
                    {
                        pDevice->Delete(pPipelineState->m_pPipelineState);
                        pPipelineState->m_pPipelineState = nullptr;
                    }
                }
                else if (ps->m_type == enums::PipelineMeshShading)
                {
                    nd3d12::meshshading_pipeline_state_t* pPipelineState = GetComponent<ngfx::pipeline_state_t, nd3d12::meshshading_pipeline_state_t>(device, ps);
                    if (pPipelineState->m_pPipelineState)
                    {
                        pDevice->Delete(pPipelineState->m_pPipelineState);
                        pPipelineState->m_pPipelineState = nullptr;
                    }
                }
                else if (ps->m_type == enums::PipelineCompute)
                {
                    nd3d12::compute_pipeline_state_t* pPipelineState = GetComponent<ngfx::pipeline_state_t, nd3d12::compute_pipeline_state_t>(device, ps);
                    if (pPipelineState->m_pPipelineState)
                    {
                        pDevice->Delete(pPipelineState->m_pPipelineState);
                        pPipelineState->m_pPipelineState = nullptr;
                    }
                }
            }

            void* GetHandle(ngfx::device_t* device, ngfx::pipeline_state_t* ps);
        }  // namespace nd3d12

    }  // namespace ngfx
}  // namespace ncore
