#include "cgfx/metal/metal_descriptor.h"
#include "cgfx/metal/metal_device.h"
#include "cgfx/metal/metal_texture.h"
#include "cgfx/metal/metal_rt_tlas.h"
#include "cgfx/gfx.h"

#include "cmacos/Metal/Metal.hpp"
#include "cmacos/MetalShaderConverter/IRConverterRuntime/ir_converter_runtime.h"

namespace ncore
{
    namespace ngfx
    {
        // MetalShaderResourceView::MetalShaderResourceView(MetalDevice* pDevice, resource_t* pResource, const srv_desc_t& desc, const char* name)
        // {
        //     m_pDevice   = pDevice;
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
        //         case GfxShaderResourceView::Texture2D:
        //             {
        //                 m_pTextureView = texture->newTextureView(format, MTL::TextureType2DArray, levelRange, NS::Range(0, 1));
        //                 IRDescriptorTableSetTexture(descriptorTableEntry, m_pTextureView, 0.0f, 0);
        //                 break;
        //             }
        //         case GfxShaderResourceView::Texture2DArray:
        //             {
        //                 m_pTextureView = texture->newTextureView(format, MTL::TextureType2DArray, levelRange, NS::Range(m_desc.texture.array_slice, m_desc.texture.array_size));
        //                 IRDescriptorTableSetTexture(descriptorTableEntry, m_pTextureView, 0.0f, 0);
        //                 break;
        //             }
        //         case GfxShaderResourceView::Texture3D:
        //             {
        //                 m_pTextureView = texture->newTextureView(format, MTL::TextureType3D, levelRange, NS::Range(0, 1));
        //                 IRDescriptorTableSetTexture(descriptorTableEntry, m_pTextureView, 0.0f, 0);
        //                 break;
        //             }
        //         case GfxShaderResourceView::TextureCube:
        //             {
        //                 m_pTextureView = texture->newTextureView(format, MTL::TextureTypeCubeArray, levelRange, NS::Range(0, 6));
        //                 IRDescriptorTableSetTexture(descriptorTableEntry, m_pTextureView, 0.0f, 0);
        //                 break;
        //             }
        //         case GfxShaderResourceView::TextureCubeArray:
        //             {
        //                 m_pTextureView = texture->newTextureView(format, MTL::TextureTypeCubeArray, levelRange, NS::Range(m_desc.texture.array_slice, m_desc.texture.array_size));
        //                 IRDescriptorTableSetTexture(descriptorTableEntry, m_pTextureView, 0.0f, 0);
        //                 break;
        //             }
        //         case GfxShaderResourceView::StructuredBuffer:
        //             {
        //                 const buffer_desc_t& bufferDesc = ((buffer_t*)m_pResource)->GetDesc();
        //                 ASSERT(bufferDesc.usage & GfxBufferUsage::StructuredBuffer);
        //                 ASSERT(m_desc.format == Gfx::Unknown);
        //                 ASSERT(m_desc.buffer.offset % bufferDesc.stride == 0);
        //                 ASSERT(m_desc.buffer.size % bufferDesc.stride == 0);

        //                 IRDescriptorTableSetBuffer(descriptorTableEntry, buffer->gpuAddress(), 0);
        //                 break;
        //             }
        //         case GfxShaderResourceView::TypedBuffer:
        //             {
        //                 const buffer_desc_t& bufferDesc = ((buffer_t*)m_pResource)->GetDesc();
        //                 ASSERT(bufferDesc.usage & GfxBufferUsage::TypedBuffer);
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
        //         case GfxShaderResourceView::RawBuffer:
        //             {
        //                 const buffer_desc_t& bufferDesc = ((buffer_t*)m_pResource)->GetDesc();
        //                 ASSERT(bufferDesc.usage & GfxBufferUsage::RawBuffer);
        //                 ASSERT(bufferDesc.stride % 4 == 0);
        //                 ASSERT(m_desc.buffer.offset % 4 == 0);
        //                 ASSERT(m_desc.buffer.size % 4 == 0);

        //                 IRDescriptorTableSetBuffer(descriptorTableEntry, buffer->gpuAddress(), 0);
        //                 break;
        //             }
        //         case GfxShaderResourceView::RayTracingTLAS:
        //             {
        //                 MetalRayTracingTLAS* tlas = (MetalRayTracingTLAS*)m_pResource;
        //                 IRDescriptorTableSetAccelerationStructure(descriptorTableEntry, tlas->GetGPUHeaderBuffer()->gpuAddress());
        //                 break;
        //             }
        //         default: break;
        //     }

        //     return true;
        // }

        // MetalUnorderedAccessView::MetalUnorderedAccessView(MetalDevice* pDevice, resource_t* pResource, const uav_desc_t& desc, const char* name)
        // {
        //     m_pDevice   = pDevice;
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
        //         case GfxUnorderedAccessView::Texture2D:
        //             {
        //                 m_pTextureView = texture->newTextureView(format, MTL::TextureType2DArray, levelRange, NS::Range(0, 1));
        //                 IRDescriptorTableSetTexture(descriptorTableEntry, m_pTextureView, 0.0f, 0);
        //                 break;
        //             }
        //         case GfxUnorderedAccessView::Texture2DArray:
        //             {
        //                 m_pTextureView = texture->newTextureView(format, MTL::TextureType2DArray, levelRange, NS::Range(m_desc.texture.array_slice, m_desc.texture.array_size));
        //                 IRDescriptorTableSetTexture(descriptorTableEntry, m_pTextureView, 0.0f, 0);
        //                 break;
        //             }
        //         case GfxUnorderedAccessView::Texture3D:
        //             {
        //                 m_pTextureView = texture->newTextureView(format, MTL::TextureType3D, levelRange, NS::Range(0, 1));
        //                 IRDescriptorTableSetTexture(descriptorTableEntry, m_pTextureView, 0.0f, 0);
        //                 break;
        //             }
        //         case GfxUnorderedAccessView::StructuredBuffer:
        //             {
        //                 const buffer_desc_t& bufferDesc = ((buffer_t*)m_pResource)->GetDesc();
        //                 ASSERT(bufferDesc.usage & GfxBufferUsage::StructuredBuffer);
        //                 ASSERT(bufferDesc.usage & GfxBufferUsage::UnorderedAccess);
        //                 ASSERT(m_desc.format == Gfx::Unknown);
        //                 ASSERT(m_desc.buffer.offset % bufferDesc.stride == 0);
        //                 ASSERT(m_desc.buffer.size % bufferDesc.stride == 0);

        //                 IRDescriptorTableSetBuffer(descriptorTableEntry, buffer->gpuAddress(), 0);
        //                 break;
        //             }
        //         case GfxUnorderedAccessView::TypedBuffer:
        //             {
        //                 const buffer_desc_t& bufferDesc = ((buffer_t*)m_pResource)->GetDesc();
        //                 ASSERT(bufferDesc.usage & GfxBufferUsage::TypedBuffer);
        //                 ASSERT(bufferDesc.usage & GfxBufferUsage::UnorderedAccess);
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
        //         case GfxUnorderedAccessView::RawBuffer:
        //             {
        //                 const buffer_desc_t& bufferDesc = ((buffer_t*)m_pResource)->GetDesc();
        //                 ASSERT(bufferDesc.usage & GfxBufferUsage::RawBuffer);
        //                 ASSERT(bufferDesc.usage & GfxBufferUsage::UnorderedAccess);
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

        // MetalConstantBufferView::MetalConstantBufferView(MetalDevice* pDevice, buffer_t* buffer, const cbv_desc_t& desc, const char* name)
        // {
        //     m_pDevice = pDevice;
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

        // MetalSampler::MetalSampler(MetalDevice* pDevice, const sampler_desc_t& desc, const char* name)
        // {
        //     m_pDevice = pDevice;
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
        //     MTL::SamplerDescriptor* descritor = MTL::SamplerDescriptor::alloc()->init();
        //     descritor->setMinFilter(ToSamplerMinMagFilter(m_desc.min_filter));
        //     descritor->setMagFilter(ToSamplerMinMagFilter(m_desc.mag_filter));
        //     descritor->setMipFilter(ToSamplerMipFilter(m_desc.mip_filter));
        //     descritor->setSAddressMode(ToSamplerAddressMode(m_desc.address_u));
        //     descritor->setTAddressMode(ToSamplerAddressMode(m_desc.address_v));
        //     descritor->setRAddressMode(ToSamplerAddressMode(m_desc.address_w));
        //     descritor->setNormalizedCoordinates(true);
        //     descritor->setLodMinClamp(m_desc.min_lod);
        //     descritor->setLodMaxClamp(m_desc.max_lod);
        //     descritor->setSupportArgumentBuffers(true);

        //     if (m_desc.enable_anisotropy)
        //     {
        //         descritor->setMaxAnisotropy((NS::UInteger)m_desc.max_anisotropy);
        //     }

        //     // unfortunately, Metal does not support Min/Max reduction mode

        //     if (m_desc.reduction_mode == GfxSamplerReductionMode::Compare)
        //     {
        //         descritor->setCompareFunction(ToCompareFunction(m_desc.compare_func));
        //     }

        //     if (m_desc.border_color[0] == 0.0f && m_desc.border_color[1] == 0.0f && m_desc.border_color[2] == 0.0f && m_desc.border_color[2] == 0.0f)
        //     {
        //         descritor->setBorderColor(MTL::SamplerBorderColorTransparentBlack);
        //     }
        //     else if (m_desc.border_color[0] == 0.0f && m_desc.border_color[1] == 0.0f && m_desc.border_color[2] == 0.0f && m_desc.border_color[2] == 1.0f)
        //     {
        //         descritor->setBorderColor(MTL::SamplerBorderColorOpaqueBlack);
        //     }
        //     else if (m_desc.border_color[0] == 1.0f && m_desc.border_color[1] == 1.0f && m_desc.border_color[2] == 1.0f && m_desc.border_color[2] == 1.0f)
        //     {
        //         descritor->setBorderColor(MTL::SamplerBorderColorOpaqueWhite);
        //     }
        //     else
        //     {
        //         ASSERT(false);  // unsupported border color
        //     }

        //     SetDebugLabel(descritor, m_name);

        //     MTL::Device* device = (MTL::Device*)m_pDevice->GetHandle();
        //     m_pSampler          = device->newSamplerState(descritor);
        //     descritor->release();

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

    }  // namespace ngfx
}  // namespace ncore
