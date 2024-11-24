#include "cgfx/metal/metal_descriptor.h"
#include "cgfx/metal/metal_device.h"
#include "cgfx/metal/metal_texture.h"
#include "cgfx/metal/metal_buffer.h"
#include "cgfx/metal/metal_rt_tlas.h"
#include "cgfx/gfx.h"

#include "cmacos/Metal/Metal.hpp"
#include "cmacos/MetalShaderConverter/IRConverterRuntime/ir_converter_runtime.h"

namespace ncore
{
    namespace ngfx
    {
        // MetalShaderResourceView::MetalShaderResourceView(MetalDevice* device, resource_t* pResource, const srv_desc_t& desc, const char* name)
        // {
        //     m_pDevice   = device;
        //     m_name      = name;
        //     m_pResource = pResource;
        //     m_desc      = desc;
        // }

        // MetalShaderResourceView::~MetalShaderResourceView()
        // {
        //     if (m_pTextureView)
        //     {
        //         m_pTextureView->release();
        //     }

        //     ((MetalDevice*)m_pDevice)->FreeResourceDescriptor(m_heapIndex);
        // }

        // bool MetalShaderResourceView::Create()
        // {
        //     IRDescriptorTableEntry* descriptorTableEntry = nullptr;
        //     m_heapIndex                                  = ((MetalDevice*)m_pDevice)->AllocateResourceDescriptor(&descriptorTableEntry);

        //     MTL::Texture*    texture = nullptr;
        //     MTL::Buffer*     buffer  = nullptr;
        //     MTL::PixelFormat format  = ToPixelFormat(m_desc.format);
        //     NS::Range        levelRange(0, 1);

        //     if (m_pResource->IsTexture())
        //     {
        //         const texture_desc_t& textureDesc = ((texture_t*)m_pResource)->GetDesc();
        //         levelRange                        = NS::Range(m_desc.texture.mip_slice, m_desc.texture.mip_levels == GFX_ALL_SUB_RESOURCE ? textureDesc.mip_levels - m_desc.texture.mip_slice : m_desc.texture.mip_levels);

        //         texture = (MTL::Texture*)m_pResource->GetHandle();
        //     }
        //     else
        //     {
        //         buffer = (MTL::Buffer*)m_pResource->GetHandle();
        //     }

        //     switch (m_desc.type)
        //     {
        //         case enums::SRV_Texture2D:
        //             {
        //                 m_pTextureView = texture->newTextureView(format, MTL::TextureType2DArray, levelRange, NS::Range(0, 1));
        //                 IRDescriptorTableSetTexture(descriptorTableEntry, m_pTextureView, 0.0f, 0);
        //                 break;
        //             }
        //         case enums::SRV_Texture2DArray:
        //             {
        //                 m_pTextureView = texture->newTextureView(format, MTL::TextureType2DArray, levelRange, NS::Range(m_desc.texture.array_slice, m_desc.texture.array_size));
        //                 IRDescriptorTableSetTexture(descriptorTableEntry, m_pTextureView, 0.0f, 0);
        //                 break;
        //             }
        //         case enums::SRV_Texture3D:
        //             {
        //                 m_pTextureView = texture->newTextureView(format, MTL::TextureType3D, levelRange, NS::Range(0, 1));
        //                 IRDescriptorTableSetTexture(descriptorTableEntry, m_pTextureView, 0.0f, 0);
        //                 break;
        //             }
        //         case enums::SRV_TextureCube:
        //             {
        //                 m_pTextureView = texture->newTextureView(format, MTL::TextureTypeCubeArray, levelRange, NS::Range(0, 6));
        //                 IRDescriptorTableSetTexture(descriptorTableEntry, m_pTextureView, 0.0f, 0);
        //                 break;
        //             }
        //         case enums::SRV_TextureCubeArray:
        //             {
        //                 m_pTextureView = texture->newTextureView(format, MTL::TextureTypeCubeArray, levelRange, NS::Range(m_desc.texture.array_slice, m_desc.texture.array_size));
        //                 IRDescriptorTableSetTexture(descriptorTableEntry, m_pTextureView, 0.0f, 0);
        //                 break;
        //             }
        //         case enums::SRV_StructuredBuffer:
        //             {
        //                 const buffer_desc_t& bufferDesc = ((buffer_t*)m_pResource)->GetDesc();
        //                 ASSERT(bufferDesc.usage & enums::BufferUsageStructuredBuffer);
        //                 ASSERT(m_desc.format == enums::FORMAT_UNKNOWN);
        //                 ASSERT(m_desc.buffer.offset % bufferDesc.stride == 0);
        //                 ASSERT(m_desc.buffer.size % bufferDesc.stride == 0);

        //                 IRDescriptorTableSetBuffer(descriptorTableEntry, buffer->gpuAddress(), 0);
        //                 break;
        //             }
        //         case enums::SRV_TypedBuffer:
        //             {
        //                 const buffer_desc_t& bufferDesc = ((buffer_t*)m_pResource)->GetDesc();
        //                 ASSERT(bufferDesc.usage & enums::BufferUsageTypedBuffer);
        //                 ASSERT(m_desc.buffer.offset % bufferDesc.stride == 0);
        //                 ASSERT(m_desc.buffer.size % bufferDesc.stride == 0);

        //                 u32 element_num = m_desc.buffer.size / bufferDesc.stride;

        //                 MTL::TextureDescriptor* descriptor = MTL::TextureDescriptor::alloc()->textureBufferDescriptor(format, element_num, buffer->resourceOptions(), MTL::TextureUsageShaderRead);
        //                 m_pTextureView                     = buffer->newTexture(descriptor, m_desc.buffer.offset, GetFormatRowPitch(m_desc.format, element_num));

        //                 IRBufferView bufferView      = {};
        //                 bufferView.buffer            = buffer;
        //                 bufferView.bufferOffset      = m_desc.buffer.offset;
        //                 bufferView.bufferSize        = m_desc.buffer.size;
        //                 bufferView.textureBufferView = m_pTextureView;
        //                 bufferView.typedBuffer       = true;
        //                 IRDescriptorTableSetBufferView(descriptorTableEntry, &bufferView);
        //                 break;
        //             }
        //         case enums::SRV_RawBuffer:
        //             {
        //                 const buffer_desc_t& bufferDesc = ((buffer_t*)m_pResource)->GetDesc();
        //                 ASSERT(bufferDesc.usage & enums::BufferUsageRawBuffer);
        //                 ASSERT(bufferDesc.stride % 4 == 0);
        //                 ASSERT(m_desc.buffer.offset % 4 == 0);
        //                 ASSERT(m_desc.buffer.size % 4 == 0);

        //                 IRDescriptorTableSetBuffer(descriptorTableEntry, buffer->gpuAddress(), 0);
        //                 break;
        //             }
        //         case enums::SRV_RayTracingTLAS:
        //             {
        //                 MetalRayTracingTLAS* tlas = (MetalRayTracingTLAS*)m_pResource;
        //                 IRDescriptorTableSetAccelerationStructure(descriptorTableEntry, tlas->GetGPUHeaderBuffer()->gpuAddress());
        //                 break;
        //             }
        //         default: break;
        //     }

        //     return true;
        // }

        // MetalUnorderedAccessView::MetalUnorderedAccessView(MetalDevice* device, resource_t* pResource, const uav_desc_t& desc, const char* name)
        // {
        //     m_pDevice   = device;
        //     m_name      = name;
        //     m_pResource = pResource;
        //     m_desc      = desc;
        // }

        // MetalUnorderedAccessView::~MetalUnorderedAccessView()
        // {
        //     if (m_pTextureView)
        //     {
        //         m_pTextureView->release();
        //     }

        //     ((MetalDevice*)m_pDevice)->FreeResourceDescriptor(m_heapIndex);
        // }

        // bool MetalUnorderedAccessView::Create()
        // {
        //     IRDescriptorTableEntry* descriptorTableEntry = nullptr;
        //     m_heapIndex                                  = ((MetalDevice*)m_pDevice)->AllocateResourceDescriptor(&descriptorTableEntry);

        //     MTL::Texture*    texture = nullptr;
        //     MTL::Buffer*     buffer  = nullptr;
        //     MTL::PixelFormat format  = ToPixelFormat(m_desc.format);
        //     NS::Range        levelRange(m_desc.texture.mip_slice, 1);

        //     if (m_pResource->IsTexture())
        //     {
        //         texture = (MTL::Texture*)m_pResource->GetHandle();
        //     }
        //     else
        //     {
        //         buffer = (MTL::Buffer*)m_pResource->GetHandle();
        //     }

        //     switch (m_desc.type)
        //     {
        //         case enums::UAV_Texture2D:
        //             {
        //                 m_pTextureView = texture->newTextureView(format, MTL::TextureType2DArray, levelRange, NS::Range(0, 1));
        //                 IRDescriptorTableSetTexture(descriptorTableEntry, m_pTextureView, 0.0f, 0);
        //                 break;
        //             }
        //         case enums::UAV_Texture2DArray:
        //             {
        //                 m_pTextureView = texture->newTextureView(format, MTL::TextureType2DArray, levelRange, NS::Range(m_desc.texture.array_slice, m_desc.texture.array_size));
        //                 IRDescriptorTableSetTexture(descriptorTableEntry, m_pTextureView, 0.0f, 0);
        //                 break;
        //             }
        //         case enums::UAV_Texture3D:
        //             {
        //                 m_pTextureView = texture->newTextureView(format, MTL::TextureType3D, levelRange, NS::Range(0, 1));
        //                 IRDescriptorTableSetTexture(descriptorTableEntry, m_pTextureView, 0.0f, 0);
        //                 break;
        //             }
        //         case enums::UAV_StructuredBuffer:
        //             {
        //                 const buffer_desc_t& bufferDesc = ((buffer_t*)m_pResource)->GetDesc();
        //                 ASSERT(bufferDesc.usage & enums::BufferUsageStructuredBuffer);
        //                 ASSERT(bufferDesc.usage & enums::BufferUsageUnorderedAccess);
        //                 ASSERT(m_desc.format == enums::FORMAT_UNKNOWN);
        //                 ASSERT(m_desc.buffer.offset % bufferDesc.stride == 0);
        //                 ASSERT(m_desc.buffer.size % bufferDesc.stride == 0);

        //                 IRDescriptorTableSetBuffer(descriptorTableEntry, buffer->gpuAddress(), 0);
        //                 break;
        //             }
        //         case enums::UAV_TypedBuffer:
        //             {
        //                 const buffer_desc_t& bufferDesc = ((buffer_t*)m_pResource)->GetDesc();
        //                 ASSERT(bufferDesc.usage & enums::BufferUsageTypedBuffer);
        //                 ASSERT(bufferDesc.usage & enums::BufferUsageUnorderedAccess);
        //                 ASSERT(m_desc.buffer.offset % bufferDesc.stride == 0);
        //                 ASSERT(m_desc.buffer.size % bufferDesc.stride == 0);

        //                 u32 element_num = m_desc.buffer.size / bufferDesc.stride;

        //                 MTL::TextureDescriptor* descriptor =
        //                   MTL::TextureDescriptor::alloc()->textureBufferDescriptor(format, element_num, buffer->resourceOptions(), MTL::TextureUsageShaderRead | MTL::TextureUsageShaderWrite);  // todo MTL::TextureUsageShaderAtomic
        //                 m_pTextureView = buffer->newTexture(descriptor, m_desc.buffer.offset, GetFormatRowPitch(m_desc.format, element_num));

        //                 IRBufferView bufferView      = {};
        //                 bufferView.buffer            = buffer;
        //                 bufferView.bufferOffset      = m_desc.buffer.offset;
        //                 bufferView.bufferSize        = m_desc.buffer.size;
        //                 bufferView.textureBufferView = m_pTextureView;
        //                 bufferView.typedBuffer       = true;
        //                 IRDescriptorTableSetBufferView(descriptorTableEntry, &bufferView);
        //                 break;
        //             }
        //         case enums::UAV_RawBuffer:
        //             {
        //                 const buffer_desc_t& bufferDesc = ((buffer_t*)m_pResource)->GetDesc();
        //                 ASSERT(bufferDesc.usage & enums::BufferUsageRawBuffer);
        //                 ASSERT(bufferDesc.usage & enums::BufferUsageUnorderedAccess);
        //                 ASSERT(bufferDesc.stride % 4 == 0);
        //                 ASSERT(m_desc.buffer.offset % 4 == 0);
        //                 ASSERT(m_desc.buffer.size % 4 == 0);

        //                 IRDescriptorTableSetBuffer(descriptorTableEntry, buffer->gpuAddress(), 0);
        //                 break;
        //             }
        //         default: break;
        //     }

        //     return true;
        // }

        // MetalConstantBufferView::MetalConstantBufferView(MetalDevice* device, buffer_t* buffer, const cbv_desc_t& desc, const char* name)
        // {
        //     m_pDevice = device;
        //     m_name    = name;
        //     m_pBuffer = buffer;
        //     m_desc    = desc;
        // }

        // MetalConstantBufferView::~MetalConstantBufferView() { ((MetalDevice*)m_pDevice)->FreeResourceDescriptor(m_heapIndex); }

        // bool MetalConstantBufferView::Create()
        // {
        //     IRDescriptorTableEntry* descriptorTableEntry = nullptr;
        //     m_heapIndex                                  = ((MetalDevice*)m_pDevice)->AllocateResourceDescriptor(&descriptorTableEntry);
        //     IRDescriptorTableSetBuffer(descriptorTableEntry, m_pBuffer->GetGpuAddress(), 0);

        //     return true;
        // }

        // MetalSampler::MetalSampler(MetalDevice* device, const sampler_desc_t& desc, const char* name)
        // {
        //     m_pDevice = device;
        //     m_name    = name;
        //     m_desc    = desc;
        // }

        // MetalSampler::~MetalSampler()
        // {
        //     m_pSampler->release();

        //     ((MetalDevice*)m_pDevice)->FreeSamplerDescriptor(m_heapIndex);
        // }

        // bool MetalSampler::Create()
        // {
        //     MTL::SamplerDescriptor* mtlDescriptor = MTL::SamplerDescriptor::alloc()->init();
        //     mtlDescriptor->setMinFilter(ToSamplerMinMagFilter(m_desc.min_filter));
        //     mtlDescriptor->setMagFilter(ToSamplerMinMagFilter(m_desc.mag_filter));
        //     mtlDescriptor->setMipFilter(ToSamplerMipFilter(m_desc.mip_filter));
        //     mtlDescriptor->setSAddressMode(ToSamplerAddressMode(m_desc.address_u));
        //     mtlDescriptor->setTAddressMode(ToSamplerAddressMode(m_desc.address_v));
        //     mtlDescriptor->setRAddressMode(ToSamplerAddressMode(m_desc.address_w));
        //     mtlDescriptor->setNormalizedCoordinates(true);
        //     mtlDescriptor->setLodMinClamp(m_desc.min_lod);
        //     mtlDescriptor->setLodMaxClamp(m_desc.max_lod);
        //     mtlDescriptor->setSupportArgumentBuffers(true);

        //     if (m_desc.enable_anisotropy)
        //     {
        //         mtlDescriptor->setMaxAnisotropy((NS::UInteger)m_desc.max_anisotropy);
        //     }

        //     // unfortunately, Metal does not support Min/Max reduction mode

        //     if (m_desc.reduction_mode == GfxSamplerReductionMode::Compare)
        //     {
        //         mtlDescriptor->setCompareFunction(ToCompareFunction(m_desc.compare_func));
        //     }

        //     if (m_desc.border_color[0] == 0.0f && m_desc.border_color[1] == 0.0f && m_desc.border_color[2] == 0.0f && m_desc.border_color[2] == 0.0f)
        //     {
        //         mtlDescriptor->setBorderColor(MTL::SamplerBorderColorTransparentBlack);
        //     }
        //     else if (m_desc.border_color[0] == 0.0f && m_desc.border_color[1] == 0.0f && m_desc.border_color[2] == 0.0f && m_desc.border_color[2] == 1.0f)
        //     {
        //         mtlDescriptor->setBorderColor(MTL::SamplerBorderColorOpaqueBlack);
        //     }
        //     else if (m_desc.border_color[0] == 1.0f && m_desc.border_color[1] == 1.0f && m_desc.border_color[2] == 1.0f && m_desc.border_color[2] == 1.0f)
        //     {
        //         mtlDescriptor->setBorderColor(MTL::SamplerBorderColorOpaqueWhite);
        //     }
        //     else
        //     {
        //         ASSERT(false);  // unsupported border color
        //     }

        //     SetDebugLabel(mtlDescriptor, m_name);

        //     MTL::Device* device = (MTL::Device*)m_pDevice->GetHandle();
        //     m_pSampler          = device->newSamplerState(mtlDescriptor);
        //     mtlDescriptor->release();

        //     if (m_pSampler == nullptr)
        //     {
        //         // RE_ERROR("[MetalSampler] failed to create {}", m_name);
        //         return false;
        //     }

        //     IRDescriptorTableEntry* descriptorTableEntry = nullptr;
        //     m_heapIndex                                  = ((MetalDevice*)m_pDevice)->AllocateSamplerDescriptor(&descriptorTableEntry);
        //     IRDescriptorTableSetSampler(descriptorTableEntry, m_pSampler, m_desc.mip_bias);

        //     return true;
        // }

        namespace nmetal
        {
            void CreateSrv(ngfx::device_t* device, descriptor_t* descriptor, texture_t* texture, const ngfx::srv_desc_t& desc)
            {
                nmetal::srv_texture_t* srv = CreateComponent<descriptor_t, srv_texture_t>(device, descriptor);
                srv->m_base.m_desc         = desc;
                srv->m_base.m_heapIndex    = GFX_INVALID_RESOURCE;
                srv->m_base.m_textureView  = nullptr;

                nmetal::mtexture_t* mtexture = GetComponent<ngfx::texture_t, nmetal::mtexture_t>(device, texture);
                srv->m_texture               = mtexture;
            }

            void CreateSrv(ngfx::device_t* device, descriptor_t* descriptor, buffer_t* buffer, const ngfx::srv_desc_t& desc)
            {
                nmetal::srv_buffer_t* srv = CreateComponent<descriptor_t, srv_buffer_t>(device, descriptor);
                srv->m_base.m_desc        = desc;
                srv->m_base.m_heapIndex   = GFX_INVALID_RESOURCE;
                srv->m_base.m_textureView = nullptr;

                nmetal::mbuffer_t* mbuffer = GetComponent<ngfx::buffer_t, nmetal::mbuffer_t>(device, buffer);
                srv->m_buffer              = mbuffer;
            }

            void CreateSrv(ngfx::device_t* device, descriptor_t* descriptor, tlas_t* tlas, const ngfx::srv_desc_t& desc)
            {
                nmetal::srv_tlas_t* srv   = CreateComponent<descriptor_t, srv_tlas_t>(device, descriptor);
                srv->m_base.m_desc        = desc;
                srv->m_base.m_heapIndex   = GFX_INVALID_RESOURCE;
                srv->m_base.m_textureView = nullptr;
                srv->m_tlas               = GetComponent<ngfx::tlas_t, nmetal::mtlas_t>(device, tlas);
            }

            void CreateUav(ngfx::device_t* device, descriptor_t* descriptor, texture_t* buffer, const uav_desc_t& desc)
            {
                nmetal::uav_texture_t* uav = CreateComponent<descriptor_t, uav_texture_t>(device, descriptor);
                uav->m_base.m_desc         = desc;
                uav->m_base.m_heapIndex    = GFX_INVALID_RESOURCE;
                uav->m_base.m_textureView  = nullptr;

                nmetal::mtexture_t* mtexture = GetComponent<ngfx::texture_t, nmetal::mtexture_t>(device, buffer);
                uav->m_texture               = mtexture;
            }

            void CreateUav(ngfx::device_t* device, descriptor_t* descriptor, buffer_t* buffer, const uav_desc_t& desc)
            {
                nmetal::uav_buffer_t* uav = CreateComponent<descriptor_t, uav_buffer_t>(device, descriptor);
                uav->m_base.m_desc        = desc;
                uav->m_base.m_heapIndex   = GFX_INVALID_RESOURCE;
                uav->m_base.m_textureView = nullptr;

                nmetal::mbuffer_t* mbuffer = GetComponent<ngfx::buffer_t, nmetal::mbuffer_t>(device, buffer);
                uav->m_buffer              = mbuffer;
            }

            void CreateCbv(ngfx::device_t* device, descriptor_t* descriptor, buffer_t* buffer, const cbv_desc_t& desc)
            {
                nmetal::cbv_t* cbv = CreateComponent<descriptor_t, cbv_t>(device, descriptor);
                cbv->m_desc        = desc;
                cbv->m_heapIndex   = GFX_INVALID_RESOURCE;
                cbv->m_buffer      = GetComponent<ngfx::buffer_t, nmetal::mbuffer_t>(device, buffer);
            }

            void CreateSampler(ngfx::device_t* device, descriptor_t* descriptor, const sampler_desc_t& desc)
            {
                nmetal::sampler_t* sampler = CreateComponent<descriptor_t, sampler_t>(device, descriptor);
                sampler->m_desc            = desc;
                sampler->m_heapIndex       = GFX_INVALID_RESOURCE;
                sampler->m_pSampler        = nullptr;
            }

            void Destroy(ngfx::device_t* device, ngfx::descriptor_t* d)
            {
                if (d->m_type == enums::DescriptorTypeSrvTexture)
                {
                    nmetal::srv_texture_t* srv = GetComponent<ngfx::descriptor_t, nmetal::srv_texture_t>(device, d);
                    if (srv)
                    {
                        if (srv->m_base.m_textureView)
                        {
                            srv->m_base.m_textureView->release();
                        }

                        nmetal::FreeResourceDescriptor(device, srv->m_base.m_heapIndex);
                    }
                }
                else if (d->m_type == enums::DescriptorTypeSrvBuffer)
                {
                    nmetal::srv_buffer_t* srv = GetComponent<ngfx::descriptor_t, nmetal::srv_buffer_t>(device, d);
                    if (srv)
                    {
                        if (srv->m_base.m_textureView)
                        {
                            srv->m_base.m_textureView->release();
                        }

                        nmetal::FreeResourceDescriptor(device, srv->m_base.m_heapIndex);
                    }
                }
                else if (d->m_type == enums::DescriptorTypeUavTexture)
                {
                    nmetal::uav_texture_t* uav = GetComponent<ngfx::descriptor_t, nmetal::uav_texture_t>(device, d);
                    if (uav)
                    {
                        if (uav->m_base.m_textureView)
                        {
                            uav->m_base.m_textureView->release();
                        }

                        nmetal::FreeResourceDescriptor(device, uav->m_base.m_heapIndex);
                    }
                }
                else if (d->m_type == enums::DescriptorTypeUavBuffer)
                {
                    nmetal::uav_buffer_t* uav = GetComponent<ngfx::descriptor_t, nmetal::uav_buffer_t>(device, d);
                    if (uav)
                    {
                        if (uav->m_base.m_textureView)
                        {
                            uav->m_base.m_textureView->release();
                        }

                        nmetal::FreeResourceDescriptor(device, uav->m_base.m_heapIndex);
                    }
                }
                else if (d->m_type == enums::DescriptorTypeCbv)
                {
                    nmetal::cbv_t* cbv = GetComponent<ngfx::descriptor_t, nmetal::cbv_t>(device, d);
                    if (cbv)
                    {
                        if (cbv->m_buffer != nullptr)
                        {
                            cbv->m_buffer->m_pBuffer->release();
                            cbv->m_buffer->m_pBuffer = nullptr;
                        }

                        nmetal::FreeResourceDescriptor(device, cbv->m_heapIndex);
                    }
                }
                else if (d->m_type == enums::DescriptorTypeSampler)
                {
                    nmetal::sampler_t* sampler = GetComponent<ngfx::descriptor_t, nmetal::sampler_t>(device, d);
                    if (sampler)
                    {
                        if (sampler->m_pSampler)
                        {
                            sampler->m_pSampler->release();
                        }

                        nmetal::FreeSamplerDescriptor(device, sampler->m_heapIndex);
                    }
                }
                else if (d->m_type == enums::DescriptorTypeSrvRayTracingTLAS)
                {
                    nmetal::srv_tlas_t* srv = GetComponent<ngfx::descriptor_t, nmetal::srv_tlas_t>(device, d);
                    if (srv)
                    {
                        nmetal::FreeResourceDescriptor(device, srv->m_base.m_heapIndex);
                    }
                }
            }

            bool Create(ngfx::device_t* device, ngfx::descriptor_t* d)
            {
                if (d->m_type == enums::DescriptorTypeSrvTexture)
                {
                    nmetal::srv_texture_t* srv = GetComponent<ngfx::descriptor_t, nmetal::srv_texture_t>(device, d);
                    if (srv)
                    {
                        srv_desc_t const&     desc        = srv->m_base.m_desc;
                        nmetal::mtexture_t*   mtexture    = srv->m_texture;
                        MTL::Texture*         mtlTexture  = mtexture->m_pTexture;
                        ngfx::texture_t*      texture     = GetComponent<nmetal::mtexture_t, ngfx::texture_t>(device, mtexture);
                        const texture_desc_t& textureDesc = texture->m_desc;

                        IRDescriptorTableEntry* descriptorTableEntry = nullptr;
                        srv->m_base.m_heapIndex                      = nmetal::AllocateResourceDescriptor(device, &descriptorTableEntry);

                        MTL::PixelFormat format = ToPixelFormat(desc.format);
                        NS::Range        levelRange(0, 1);

                        levelRange = NS::Range(desc.texture.mip_slice, desc.texture.mip_levels == GFX_ALL_SUB_RESOURCE ? textureDesc.mip_levels - desc.texture.mip_slice : desc.texture.mip_levels);

                        switch (desc.type)
                        {
                            case enums::SRV_Texture2D:
                                {
                                    srv->m_base.m_textureView = mtlTexture->newTextureView(format, MTL::TextureType2DArray, levelRange, NS::Range(0, 1));
                                    IRDescriptorTableSetTexture(descriptorTableEntry, srv->m_base.m_textureView, 0.0f, 0);
                                    break;
                                }
                            case enums::SRV_Texture2DArray:
                                {
                                    srv->m_base.m_textureView = mtlTexture->newTextureView(format, MTL::TextureType2DArray, levelRange, NS::Range(desc.texture.array_slice, desc.texture.array_size));
                                    IRDescriptorTableSetTexture(descriptorTableEntry, srv->m_base.m_textureView, 0.0f, 0);
                                    break;
                                }
                            case enums::SRV_Texture3D:
                                {
                                    srv->m_base.m_textureView = mtlTexture->newTextureView(format, MTL::TextureType3D, levelRange, NS::Range(0, 1));
                                    IRDescriptorTableSetTexture(descriptorTableEntry, srv->m_base.m_textureView, 0.0f, 0);
                                    break;
                                }
                            case enums::SRV_TextureCube:
                                {
                                    srv->m_base.m_textureView = mtlTexture->newTextureView(format, MTL::TextureTypeCubeArray, levelRange, NS::Range(0, 6));
                                    IRDescriptorTableSetTexture(descriptorTableEntry, srv->m_base.m_textureView, 0.0f, 0);
                                    break;
                                }
                            case enums::SRV_TextureCubeArray:
                                {
                                    srv->m_base.m_textureView = mtlTexture->newTextureView(format, MTL::TextureTypeCubeArray, levelRange, NS::Range(desc.texture.array_slice, desc.texture.array_size));
                                    IRDescriptorTableSetTexture(descriptorTableEntry, srv->m_base.m_textureView, 0.0f, 0);
                                    break;
                                }
                            default: break;
                        }

                        return true;
                    }
                }
                else if (d->m_type == enums::DescriptorTypeSrvBuffer)
                {
                    nmetal::srv_buffer_t* srv = GetComponent<ngfx::descriptor_t, nmetal::srv_buffer_t>(device, d);
                    if (srv)
                    {
                        srv_desc_t const&    desc       = srv->m_base.m_desc;
                        nmetal::mbuffer_t*   mbuffer    = srv->m_buffer;
                        MTL::Buffer*         mtlBuffer  = mbuffer->m_pBuffer;
                        buffer_t*            buffer     = GetComponent<nmetal::mbuffer_t, buffer_t>(device, mbuffer);
                        const buffer_desc_t& bufferDesc = buffer->m_desc;

                        IRDescriptorTableEntry* descriptorTableEntry = nullptr;
                        srv->m_base.m_heapIndex                      = nmetal::AllocateResourceDescriptor(device, &descriptorTableEntry);

                        MTL::PixelFormat format = ToPixelFormat(desc.format);
                        NS::Range        levelRange(0, 1);

                        switch (desc.type)
                        {
                            case enums::SRV_StructuredBuffer:
                                {
                                    ASSERT(bufferDesc.usage & enums::BufferUsageStructuredBuffer);
                                    ASSERT(desc.format == enums::FORMAT_UNKNOWN);
                                    ASSERT(desc.buffer.offset % bufferDesc.stride == 0);
                                    ASSERT(desc.buffer.size % bufferDesc.stride == 0);

                                    IRDescriptorTableSetBuffer(descriptorTableEntry, mtlBuffer->gpuAddress(), 0);
                                    break;
                                }
                            case enums::SRV_TypedBuffer:
                                {
                                    ASSERT(bufferDesc.usage & enums::BufferUsageTypedBuffer);
                                    ASSERT(desc.buffer.offset % bufferDesc.stride == 0);
                                    ASSERT(desc.buffer.size % bufferDesc.stride == 0);

                                    u32 const element_num = desc.buffer.size / bufferDesc.stride;

                                    MTL::TextureDescriptor* descriptor = MTL::TextureDescriptor::alloc()->textureBufferDescriptor(format, element_num, mtlBuffer->resourceOptions(), MTL::TextureUsageShaderRead);
                                    srv->m_base.m_textureView          = mtlBuffer->newTexture(descriptor, desc.buffer.offset, enums::GetFormatRowPitch(desc.format, element_num));

                                    IRBufferView bufferView      = {};
                                    bufferView.buffer            = mtlBuffer;
                                    bufferView.bufferOffset      = desc.buffer.offset;
                                    bufferView.bufferSize        = desc.buffer.size;
                                    bufferView.textureBufferView = srv->m_base.m_textureView;
                                    bufferView.typedBuffer       = true;
                                    IRDescriptorTableSetBufferView(descriptorTableEntry, &bufferView);
                                    break;
                                }
                            case enums::SRV_RawBuffer:
                                {
                                    ASSERT(bufferDesc.usage & enums::BufferUsageRawBuffer);
                                    ASSERT(bufferDesc.stride % 4 == 0);
                                    ASSERT(desc.buffer.offset % 4 == 0);
                                    ASSERT(desc.buffer.size % 4 == 0);

                                    IRDescriptorTableSetBuffer(descriptorTableEntry, mtlBuffer->gpuAddress(), 0);
                                    break;
                                }
                            case enums::SRV_RayTracingTLAS:
                                {
                                    // MetalRayTracingTLAS* tlas = (MetalRayTracingTLAS*)m_pResource;
                                    // IRDescriptorTableSetAccelerationStructure(descriptorTableEntry, tlas->GetGPUHeaderBuffer()->gpuAddress());
                                    break;
                                }
                            default: break;
                        }

                        return true;
                    }
                }
                else if (d->m_type == enums::DescriptorTypeSrvRayTracingTLAS)
                {
                    nmetal::srv_tlas_t* srv = GetComponent<ngfx::descriptor_t, nmetal::srv_tlas_t>(device, d);
                    if (srv)
                    {
                        nmetal::mtlas_t*        tlas                 = GetComponent<ngfx::descriptor_t, nmetal::mtlas_t>(device, d);
                        IRDescriptorTableEntry* descriptorTableEntry = nullptr;
                        IRDescriptorTableSetAccelerationStructure(descriptorTableEntry, tlas->m_pGPUHeaderBuffer->gpuAddress());
                        return true;
                    }
                }
                else if (d->m_type == enums::DescriptorTypeUavTexture)
                {
                    nmetal::uav_texture_t* uav = GetComponent<ngfx::descriptor_t, nmetal::uav_texture_t>(device, d);
                    if (uav)
                    {
                        IRDescriptorTableEntry* descriptorTableEntry = nullptr;
                        uav->m_base.m_heapIndex                      = nmetal::AllocateResourceDescriptor(device, &descriptorTableEntry);

                        ngfx::uav_desc_t const& desc       = uav->m_base.m_desc;
                        nmetal::mtexture_t*     mtexture   = uav->m_texture;
                        MTL::Texture*           mtlTexture = mtexture->m_pTexture;
                        MTL::PixelFormat        format     = ToPixelFormat(desc.format);
                        NS::Range               levelRange(desc.texture.mip_slice, 1);

                        switch (desc.type)
                        {
                            case enums::UAV_Texture2D:
                                {
                                    uav->m_base.m_textureView = mtlTexture->newTextureView(format, MTL::TextureType2DArray, levelRange, NS::Range(0, 1));
                                    IRDescriptorTableSetTexture(descriptorTableEntry, uav->m_base.m_textureView, 0.0f, 0);
                                    break;
                                }
                            case enums::UAV_Texture2DArray:
                                {
                                    uav->m_base.m_textureView = mtlTexture->newTextureView(format, MTL::TextureType2DArray, levelRange, NS::Range(desc.texture.array_slice, desc.texture.array_size));
                                    IRDescriptorTableSetTexture(descriptorTableEntry, uav->m_base.m_textureView, 0.0f, 0);
                                    break;
                                }
                            case enums::UAV_Texture3D:
                                {
                                    uav->m_base.m_textureView = mtlTexture->newTextureView(format, MTL::TextureType3D, levelRange, NS::Range(0, 1));
                                    IRDescriptorTableSetTexture(descriptorTableEntry, uav->m_base.m_textureView, 0.0f, 0);
                                    break;
                                }
                            default: break;
                        }

                        return true;
                    }
                }
                else if (d->m_type == enums::DescriptorTypeUavBuffer)
                {
                    nmetal::uav_buffer_t* uav = GetComponent<ngfx::descriptor_t, nmetal::uav_buffer_t>(device, d);
                    if (uav)
                    {
                        IRDescriptorTableEntry* descriptorTableEntry = nullptr;
                        uav->m_base.m_heapIndex                      = nmetal::AllocateResourceDescriptor(device, &descriptorTableEntry);

                        ngfx::uav_desc_t const& desc       = uav->m_base.m_desc;
                        nmetal::mbuffer_t*      mbuffer    = uav->m_buffer;
                        ngfx::buffer_t*         buffer     = GetComponent<nmetal::mbuffer_t, ngfx::buffer_t>(device, mbuffer);
                        const buffer_desc_t&    bufferDesc = buffer->m_desc;
                        MTL::Buffer*            mtlBuffer  = mbuffer->m_pBuffer;
                        MTL::PixelFormat        format     = ToPixelFormat(desc.format);
                        NS::Range               levelRange(desc.texture.mip_slice, 1);

                        switch (desc.type)
                        {
                            case enums::UAV_StructuredBuffer:
                                {
                                    ASSERT(bufferDesc.usage & enums::BufferUsageStructuredBuffer);
                                    ASSERT(bufferDesc.usage & enums::BufferUsageUnorderedAccess);
                                    ASSERT(desc.format == enums::FORMAT_UNKNOWN);
                                    ASSERT(desc.buffer.offset % bufferDesc.stride == 0);
                                    ASSERT(desc.buffer.size % bufferDesc.stride == 0);

                                    IRDescriptorTableSetBuffer(descriptorTableEntry, mtlBuffer->gpuAddress(), 0);
                                    break;
                                }
                            case enums::UAV_TypedBuffer:
                                {
                                    ASSERT(bufferDesc.usage & enums::BufferUsageTypedBuffer);
                                    ASSERT(bufferDesc.usage & enums::BufferUsageUnorderedAccess);
                                    ASSERT(desc.buffer.offset % bufferDesc.stride == 0);
                                    ASSERT(desc.buffer.size % bufferDesc.stride == 0);

                                    u32 const element_num = desc.buffer.size / bufferDesc.stride;

                                    // todo MTL::TextureUsageShaderAtomic
                                    MTL::TextureDescriptor* mtlDescriptor = MTL::TextureDescriptor::alloc()->textureBufferDescriptor(format, element_num, mtlBuffer->resourceOptions(), MTL::TextureUsageShaderRead | MTL::TextureUsageShaderWrite);
                                    uav->m_base.m_textureView             = mtlBuffer->newTexture(mtlDescriptor, desc.buffer.offset, GetFormatRowPitch(desc.format, element_num));

                                    IRBufferView bufferView      = {};
                                    bufferView.buffer            = mtlBuffer;
                                    bufferView.bufferOffset      = desc.buffer.offset;
                                    bufferView.bufferSize        = desc.buffer.size;
                                    bufferView.textureBufferView = uav->m_base.m_textureView;
                                    bufferView.typedBuffer       = true;
                                    IRDescriptorTableSetBufferView(descriptorTableEntry, &bufferView);
                                    break;
                                }
                            case enums::UAV_RawBuffer:
                                {
                                    ASSERT(bufferDesc.usage & enums::BufferUsageRawBuffer);
                                    ASSERT(bufferDesc.usage & enums::BufferUsageUnorderedAccess);
                                    ASSERT(bufferDesc.stride % 4 == 0);
                                    ASSERT(desc.buffer.offset % 4 == 0);
                                    ASSERT(desc.buffer.size % 4 == 0);

                                    IRDescriptorTableSetBuffer(descriptorTableEntry, mtlBuffer->gpuAddress(), 0);
                                    break;
                                }
                            default: break;
                        }

                        return true;
                    }
                }
                else if (d->m_type == enums::DescriptorTypeCbv)
                {
                    nmetal::cbv_t* cbv = GetComponent<ngfx::descriptor_t, nmetal::cbv_t>(device, d);
                    if (cbv)
                    {
                        MTL::Buffer*            mtlBuffer            = cbv->m_buffer->m_pBuffer;
                        IRDescriptorTableEntry* descriptorTableEntry = nullptr;
                        cbv->m_heapIndex                             = AllocateResourceDescriptor(device, &descriptorTableEntry);
                        IRDescriptorTableSetBuffer(descriptorTableEntry, mtlBuffer->gpuAddress(), 0);
                        return true;
                    }
                }
                else if (d->m_type == enums::DescriptorTypeSampler)
                {
                    nmetal::sampler_t* sampler = GetComponent<ngfx::descriptor_t, nmetal::sampler_t>(device, d);
                    if (sampler)
                    {
                        sampler_desc_t const& desc = sampler->m_desc;

                        MTL::SamplerDescriptor* mtlDescriptor = MTL::SamplerDescriptor::alloc()->init();
                        mtlDescriptor->setMinFilter(ToSamplerMinMagFilter(desc.min_filter));
                        mtlDescriptor->setMagFilter(ToSamplerMinMagFilter(desc.mag_filter));
                        mtlDescriptor->setMipFilter(ToSamplerMipFilter(desc.mip_filter));
                        mtlDescriptor->setSAddressMode(ToSamplerAddressMode(desc.address_u));
                        mtlDescriptor->setTAddressMode(ToSamplerAddressMode(desc.address_v));
                        mtlDescriptor->setRAddressMode(ToSamplerAddressMode(desc.address_w));
                        mtlDescriptor->setNormalizedCoordinates(true);
                        mtlDescriptor->setLodMinClamp(desc.min_lod);
                        mtlDescriptor->setLodMaxClamp(desc.max_lod);
                        mtlDescriptor->setSupportArgumentBuffers(true);

                        if (desc.enable_anisotropy)
                        {
                            mtlDescriptor->setMaxAnisotropy((NS::UInteger)desc.max_anisotropy);
                        }

                        // unfortunately, Metal does not support Min/Max reduction mode

                        if (desc.reduction_mode == enums::SamplerReductionModeCompare)
                        {
                            mtlDescriptor->setCompareFunction(ToCompareFunction(desc.compare_func));
                        }
                        if (desc.border_color[0] == 0.0f && desc.border_color[1] == 0.0f && desc.border_color[2] == 0.0f && desc.border_color[2] == 0.0f)
                        {
                            mtlDescriptor->setBorderColor(MTL::SamplerBorderColorTransparentBlack);
                        }
                        else if (desc.border_color[0] == 0.0f && desc.border_color[1] == 0.0f && desc.border_color[2] == 0.0f && desc.border_color[2] == 1.0f)
                        {
                            mtlDescriptor->setBorderColor(MTL::SamplerBorderColorOpaqueBlack);
                        }
                        else if (desc.border_color[0] == 1.0f && desc.border_color[1] == 1.0f && desc.border_color[2] == 1.0f && desc.border_color[2] == 1.0f)
                        {
                            mtlDescriptor->setBorderColor(MTL::SamplerBorderColorOpaqueWhite);
                        }
                        else
                        {
                            ASSERT(false);  // unsupported border color
                        }

                        const name_t* name = GetComponent<ngfx::descriptor_t, name_t>(device, d);
                        SetDebugLabel(mtlDescriptor, name->m_name);

                        nmetal::device_t* mdevice   = GetComponent<ngfx::device_t, nmetal::device_t>(device, device);
                        MTL::Device*      mtlDevice = mdevice->m_pDevice;
                        sampler->m_pSampler         = mtlDevice->newSamplerState(mtlDescriptor);
                        mtlDescriptor->release();

                        if (sampler->m_pSampler == nullptr)
                        {
                            // RE_ERROR("[MetalSampler] failed to create {}", m_name);
                            return false;
                        }

                        IRDescriptorTableEntry* descriptorTableEntry = nullptr;
                        sampler->m_heapIndex                         = nmetal::AllocateSamplerDescriptor(device, &descriptorTableEntry);
                        IRDescriptorTableSetSampler(descriptorTableEntry, sampler->m_pSampler, desc.mip_bias);

                        return true;
                    }
                }

                return false;
            }

            u32 GetHeapIndex(ngfx::device_t* device, ngfx::descriptor_t* d)
            {
                u32 heapIndex = GFX_INVALID_RESOURCE;
                switch (d->m_type)
                {
                    case enums::DescriptorTypeSrvTexture:
                        {
                            nmetal::srv_texture_t* srv = GetComponent<ngfx::descriptor_t, nmetal::srv_texture_t>(device, d);
                            heapIndex                  = (srv != nullptr) ? srv->m_base.m_heapIndex : GFX_INVALID_RESOURCE;
                        }
                    case enums::DescriptorTypeSrvBuffer:
                        {
                            nmetal::srv_buffer_t* srv = GetComponent<ngfx::descriptor_t, nmetal::srv_buffer_t>(device, d);
                            heapIndex                 = (srv != nullptr) ? srv->m_base.m_heapIndex : GFX_INVALID_RESOURCE;
                        }
                    case enums::DescriptorTypeUavTexture:
                        {
                            nmetal::uav_texture_t* uav = GetComponent<ngfx::descriptor_t, nmetal::uav_texture_t>(device, d);
                            heapIndex                  = (uav != nullptr) ? uav->m_base.m_heapIndex : GFX_INVALID_RESOURCE;
                        }
                    case enums::DescriptorTypeUavBuffer:
                        {
                            nmetal::uav_buffer_t* uav = GetComponent<ngfx::descriptor_t, nmetal::uav_buffer_t>(device, d);
                            heapIndex                 = (uav != nullptr) ? uav->m_base.m_heapIndex : GFX_INVALID_RESOURCE;
                        }
                    case enums::DescriptorTypeCbv:
                        {
                            nmetal::cbv_t* cbv = GetComponent<ngfx::descriptor_t, nmetal::cbv_t>(device, d);
                            heapIndex          = (cbv != nullptr) ? cbv->m_heapIndex : GFX_INVALID_RESOURCE;
                        }
                    case enums::DescriptorTypeSampler:
                        {
                            nmetal::sampler_t* sampler = GetComponent<ngfx::descriptor_t, nmetal::sampler_t>(device, d);
                            heapIndex                  = (sampler != nullptr) ? sampler->m_heapIndex : GFX_INVALID_RESOURCE;
                        }
                    case enums::DescriptorTypeSrvRayTracingTLAS:
                        {
                            nmetal::srv_tlas_t* srv = GetComponent<ngfx::descriptor_t, nmetal::srv_tlas_t>(device, d);
                            heapIndex               = (srv != nullptr) ? srv->m_base.m_heapIndex : GFX_INVALID_RESOURCE;
                        }
                }
                return heapIndex;
            }
        }  // namespace nmetal

    }  // namespace ngfx
}  // namespace ncore
