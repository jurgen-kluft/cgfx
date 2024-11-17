#include "cgfx/metal/metal_pipeline_state.h"
#include "cgfx/metal/metal_device.h"
#include "cgfx/metal/metal_shader.h"
#include "cgfx/gfx.h"

namespace ncore
{
    namespace ngfx
    {
        namespace nmetal
        {
            pipeline_state_t* CreateGraphicsPipelineState(ngfx::device_t* device, resource_t* resource, pipeline_state_t* ps, const graphics_pipeline_desc_t& desc)
            {
                nmetal::graphics_pipeline_state_t* mps = AddComponent<ngfx::resource_t, nmetal::graphics_pipeline_state_t>(device, resource);
                mps->m_desc                            = desc;
                return ps;
            }

            pipeline_state_t* CreateMeshShadingPipelineState(ngfx::device_t* device, resource_t* resource, pipeline_state_t* ps, const mesh_shading_pipeline_desc_t& desc)
            {
                nmetal::meshshading_pipeline_state_t* mps = AddComponent<ngfx::resource_t, nmetal::meshshading_pipeline_state_t>(device, resource);
                mps->m_desc                               = desc;
                return ps;
            }

            pipeline_state_t* CreateComputePipelineState(ngfx::device_t* device, resource_t* resource, pipeline_state_t* ps, const compute_pipeline_desc_t& desc)
            {
                nmetal::compute_pipeline_state_t* mps = AddComponent<ngfx::resource_t, nmetal::compute_pipeline_state_t>(device, resource);
                mps->m_desc                           = desc;
                return ps;
            }

            bool Create(ngfx::device_t* device, ngfx::pipeline_state_t* ps)
            {
                if (ps->m_type == enums::PipelineGraphics)
                {
                    nmetal::graphics_pipeline_state_t* mps = GetOtherComponent<ngfx::pipeline_state_t, nmetal::graphics_pipeline_state_t>(device, ps);

                    mps->m_pPSO->release();
                    mps->m_pDepthStencilState->release();

                    MTL::RenderPipelineDescriptor* descriptor = MTL::RenderPipelineDescriptor::alloc()->init();
                    descriptor->setVertexFunction((MTL::Function*)GetHandle(device, mps->m_desc.vs));
                    if (mps->m_desc.ps)
                    {
                        descriptor->setFragmentFunction((MTL::Function*)GetHandle(device, mps->m_desc.ps));
                    }

                    for (u32 i = 0; i < 8; ++i)
                    {
                        if (mps->m_desc.rt_format[i] != enums::FORMAT_UNKNOWN)
                        {
                            MTL::RenderPipelineColorAttachmentDescriptor* colorAttachment = descriptor->colorAttachments()->object(i);
                            colorAttachment->setPixelFormat(ToPixelFormat(mps->m_desc.rt_format[i]));
                            colorAttachment->setBlendingEnabled(mps->m_desc.blend_state[i].blend_enable);
                            colorAttachment->setSourceRGBBlendFactor(ToBlendFactor(mps->m_desc.blend_state[i].color_src));
                            colorAttachment->setDestinationRGBBlendFactor(ToBlendFactor(mps->m_desc.blend_state[i].color_dst));
                            colorAttachment->setRgbBlendOperation(ToBlendOperation(mps->m_desc.blend_state[i].color_op));
                            colorAttachment->setSourceAlphaBlendFactor(ToBlendFactor(mps->m_desc.blend_state[i].alpha_src));
                            colorAttachment->setDestinationAlphaBlendFactor(ToBlendFactor(mps->m_desc.blend_state[i].alpha_dst));
                            colorAttachment->setAlphaBlendOperation(ToBlendOperation(mps->m_desc.blend_state[i].alpha_op));
                            colorAttachment->setWriteMask(ToColorWriteMask(mps->m_desc.blend_state[i].write_mask));
                        }
                    }

                    descriptor->setDepthAttachmentPixelFormat(ToPixelFormat(mps->m_desc.depthstencil_format));
                    if (IsStencilFormat(mps->m_desc.depthstencil_format))
                    {
                        descriptor->setStencilAttachmentPixelFormat(ToPixelFormat(mps->m_desc.depthstencil_format));
                    }

                    descriptor->setInputPrimitiveTopology(ToTopologyClass(mps->m_desc.primitive_type));
                    descriptor->setRasterSampleCount(1);

                    name_t const* name = GetOtherComponent<ngfx::pipeline_state_t, name_t>(device, ps);
                    SetDebugLabel(descriptor, name->m_name);

                    nmetal::device_t* mdevice   = GetOtherComponent<ngfx::device_t, nmetal::device_t>(device, device);
                    MTL::Device*      mtlDevice = mdevice->m_pDevice;
                    NS::Error*        pError    = nullptr;
                    mps->m_pPSO                 = mtlDevice->newRenderPipelineState(descriptor, &pError);
                    descriptor->release();

                    if (!mps->m_pPSO)
                    {
                        // RE_ERROR("[MetalGraphicsPipelineState] failed to create {} \r\n{}", m_name, pError->localizedDescription()->utf8String());
                        pError->release();
                        return false;
                    }

                    MTL::DepthStencilDescriptor* depthStencilDescriptor = ToDepthStencilDescriptor(mps->m_desc.depthstencil_state);
                    mps->m_pDepthStencilState                           = mtlDevice->newDepthStencilState(depthStencilDescriptor);
                    depthStencilDescriptor->release();

                    return true;
                }
                else if (ps->m_type == enums::PipelineMeshShading)
                {
                    nmetal::meshshading_pipeline_state_t* mps = GetOtherComponent<ngfx::pipeline_state_t, nmetal::meshshading_pipeline_state_t>(device, ps);

                    mps->m_pPSO->release();
                    mps->m_pDepthStencilState->release();

                    MTL::MeshRenderPipelineDescriptor* descriptor = MTL::MeshRenderPipelineDescriptor::alloc()->init();
                    descriptor->setMeshFunction((MTL::Function*)GetHandle(device, mps->m_desc.ms));
                    if (mps->m_desc.as)
                    {
                        descriptor->setObjectFunction((MTL::Function*)GetHandle(device, mps->m_desc.as));
                    }

                    if (mps->m_desc.ps)
                    {
                        descriptor->setFragmentFunction((MTL::Function*)GetHandle(device, mps->m_desc.ps));
                    }

                    for (u32 i = 0; i < 8; ++i)
                    {
                        if (mps->m_desc.rt_format[i] != enums::FORMAT_UNKNOWN)
                        {
                            MTL::RenderPipelineColorAttachmentDescriptor* colorAttachment = descriptor->colorAttachments()->object(i);
                            colorAttachment->setPixelFormat(ToPixelFormat(mps->m_desc.rt_format[i]));
                            colorAttachment->setBlendingEnabled(mps->m_desc.blend_state[i].blend_enable);
                            colorAttachment->setSourceRGBBlendFactor(ToBlendFactor(mps->m_desc.blend_state[i].color_src));
                            colorAttachment->setDestinationRGBBlendFactor(ToBlendFactor(mps->m_desc.blend_state[i].color_dst));
                            colorAttachment->setRgbBlendOperation(ToBlendOperation(mps->m_desc.blend_state[i].color_op));
                            colorAttachment->setSourceAlphaBlendFactor(ToBlendFactor(mps->m_desc.blend_state[i].alpha_src));
                            colorAttachment->setDestinationAlphaBlendFactor(ToBlendFactor(mps->m_desc.blend_state[i].alpha_dst));
                            colorAttachment->setAlphaBlendOperation(ToBlendOperation(mps->m_desc.blend_state[i].alpha_op));
                            colorAttachment->setWriteMask(ToColorWriteMask(mps->m_desc.blend_state[i].write_mask));
                        }
                    }

                    descriptor->setDepthAttachmentPixelFormat(ToPixelFormat(mps->m_desc.depthstencil_format));
                    if (IsStencilFormat(mps->m_desc.depthstencil_format))
                    {
                        descriptor->setStencilAttachmentPixelFormat(ToPixelFormat(mps->m_desc.depthstencil_format));
                    }

                    descriptor->setRasterSampleCount(1);

                    name_t const* name = GetOtherComponent<ngfx::pipeline_state_t, name_t>(device, ps);
                    SetDebugLabel(descriptor, name->m_name);

                    nmetal::device_t* mdevice   = GetOtherComponent<ngfx::device_t, nmetal::device_t>(device, device);
                    MTL::Device*      mtlDevice = mdevice->m_pDevice;
                    NS::Error*        pError    = nullptr;
                    mps->m_pPSO                 = mtlDevice->newRenderPipelineState(descriptor, MTL::PipelineOptionNone, nullptr, &pError);
                    descriptor->release();

                    if (!mps->m_pPSO)
                    {
                        // RE_ERROR("[MetalMeshShadingPipelineState] failed to create {} \r\n{}", m_name, pError->localizedDescription()->utf8String());
                        pError->release();
                        return false;
                    }

                    MTL::DepthStencilDescriptor* depthStencilDescriptor = ToDepthStencilDescriptor(mps->m_desc.depthstencil_state);
                    mps->m_pDepthStencilState                           = mtlDevice->newDepthStencilState(depthStencilDescriptor);
                    depthStencilDescriptor->release();

                    if (mps->m_desc.as)
                    {
                        const nmetal::shader_reflection_t& reflection = GetReflection(device, mps->m_desc.as);
                        mps->m_threadsPerObjectThreadgroup            = MTL::Size::Make(reflection.threadsPerThreadgroup[0], reflection.threadsPerThreadgroup[1], reflection.threadsPerThreadgroup[2]);
                    }

                    {
                        const nmetal::shader_reflection_t& reflection = GetReflection(device, mps->m_desc.ms);
                        mps->m_threadsPerMeshThreadgroup              = MTL::Size::Make(reflection.threadsPerThreadgroup[0], reflection.threadsPerThreadgroup[1], reflection.threadsPerThreadgroup[2]);
                    }

                    return true;
                }
                else if (ps->m_type == enums::PipelineCompute)
                {
                    nmetal::compute_pipeline_state_t* mps = GetOtherComponent<ngfx::pipeline_state_t, nmetal::compute_pipeline_state_t>(device, ps);
                }
                return false;
            }

            void Destroy(ngfx::device_t* device, ngfx::pipeline_state_t* ps)
            {
                if (ps->m_type == enums::PipelineGraphics)
                {
                    nmetal::graphics_pipeline_state_t* mps = GetOtherComponent<ngfx::pipeline_state_t, nmetal::graphics_pipeline_state_t>(device, ps);
                    if (mps != nullptr)
                    {
                        mps->m_pPSO->release();
                        mps->m_pDepthStencilState->release();
                    }
                }
                else if (ps->m_type == enums::PipelineMeshShading)
                {
                    nmetal::meshshading_pipeline_state_t* mps = GetOtherComponent<ngfx::pipeline_state_t, nmetal::meshshading_pipeline_state_t>(device, ps);
                    if (mps != nullptr)
                    {
                        mps->m_pPSO->release();
                        mps->m_pDepthStencilState->release();
                    }
                }
                else if (ps->m_type == enums::PipelineCompute)
                {
                    nmetal::compute_pipeline_state_t* mps = GetOtherComponent<ngfx::pipeline_state_t, nmetal::compute_pipeline_state_t>(device, ps);
                    if (mps != nullptr)
                    {
                        mps->m_pPSO->release();
                    }
                }
            }

            void* GetHandle(ngfx::device_t* device, ngfx::pipeline_state_t* ps)
            {
                if (ps->m_type == enums::PipelineGraphics)
                {
                    nmetal::graphics_pipeline_state_t* mps = GetOtherComponent<ngfx::pipeline_state_t, nmetal::graphics_pipeline_state_t>(device, ps);
                    return mps->m_pPSO;
                }
                else if (ps->m_type == enums::PipelineMeshShading)
                {
                    nmetal::meshshading_pipeline_state_t* mps = GetOtherComponent<ngfx::pipeline_state_t, nmetal::meshshading_pipeline_state_t>(device, ps);
                    return mps->m_pPSO;
                }
                else if (ps->m_type == enums::PipelineCompute)
                {
                    nmetal::compute_pipeline_state_t* mps = GetOtherComponent<ngfx::pipeline_state_t, nmetal::compute_pipeline_state_t>(device, ps);
                    return mps->m_pPSO;
                }
                return nullptr;
            }

        }  // namespace nmetal

    }  // namespace ngfx
}  // namespace ncore
