#include "cbase/c_debug.h"
#include "cgfx/gfx_defines.h"
#include "cgfx/d3d12/d3d12_device.h"
#include "cgfx/d3d12/d3d12_buffer.h"
#include "cgfx/d3d12/d3d12_texture.h"
#include "cgfx/d3d12/d3d12_rt_tlas.h"
#include "cgfx/d3d12/d3d12_descriptor.h"

namespace ncore
{
    namespace ngfx
    {
        // D3D12ShaderResourceView::D3D12ShaderResourceView(D3D12Device* pDevice, resource_t* pResource, const srv_desc_t& desc, const char* name)
        // {
        //     m_pDevice   = pDevice;
        //     m_name      = name;
        //     m_pResource = pResource;
        //     m_desc      = desc;
        // }

        // D3D12ShaderResourceView::~D3D12ShaderResourceView() { ((D3D12Device*)m_pDevice)->DeleteResourceDescriptor(m_descriptor); }

        // bool D3D12ShaderResourceView::Create()
        // {
        //     D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        //     srvDesc.Shader4ComponentMapping         = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

        //     switch (m_desc.type)
        //     {
        //         case GfxShaderResourceView::Texture2D:
        //             {
        //                 const texture_desc_t& textureDesc = ((texture_t*)m_pResource)->GetDesc();
        //                 bool                  depth       = textureDesc.usage & GfxTextureUsage::DepthStencil;

        //                 srvDesc.Format                    = dxgi_format(m_desc.format, depth);
        //                 srvDesc.ViewDimension             = D3D12_SRV_DIMENSION_TEXTURE2D;
        //                 srvDesc.Texture2D.MostDetailedMip = m_desc.texture.mip_slice;
        //                 srvDesc.Texture2D.MipLevels       = m_desc.texture.mip_levels;
        //                 srvDesc.Texture2D.PlaneSlice      = m_desc.texture.plane_slice;
        //                 break;
        //             }
        //         case GfxShaderResourceView::Texture2DArray:
        //             {
        //                 srvDesc.Format                         = dxgi_format(m_desc.format);
        //                 srvDesc.ViewDimension                  = D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
        //                 srvDesc.Texture2DArray.MostDetailedMip = m_desc.texture.mip_slice;
        //                 srvDesc.Texture2DArray.MipLevels       = m_desc.texture.mip_levels;
        //                 srvDesc.Texture2DArray.FirstArraySlice = m_desc.texture.array_slice;
        //                 srvDesc.Texture2DArray.ArraySize       = m_desc.texture.array_size;
        //                 srvDesc.Texture2DArray.PlaneSlice      = m_desc.texture.plane_slice;
        //                 break;
        //             }
        //         case GfxShaderResourceView::Texture3D:
        //             {
        //                 srvDesc.Format                    = dxgi_format(m_desc.format);
        //                 srvDesc.ViewDimension             = D3D12_SRV_DIMENSION_TEXTURE3D;
        //                 srvDesc.Texture3D.MostDetailedMip = m_desc.texture.mip_slice;
        //                 srvDesc.Texture3D.MipLevels       = m_desc.texture.mip_levels;
        //                 break;
        //             }
        //         case GfxShaderResourceView::TextureCube:
        //             {
        //                 srvDesc.Format                      = dxgi_format(m_desc.format);
        //                 srvDesc.ViewDimension               = D3D12_SRV_DIMENSION_TEXTURECUBE;
        //                 srvDesc.TextureCube.MostDetailedMip = m_desc.texture.mip_slice;
        //                 srvDesc.TextureCube.MipLevels       = m_desc.texture.mip_levels;
        //                 break;
        //             }
        //         case GfxShaderResourceView::TextureCubeArray:
        //             {
        //                 srvDesc.Format                            = dxgi_format(m_desc.format);
        //                 srvDesc.ViewDimension                     = D3D12_SRV_DIMENSION_TEXTURECUBEARRAY;
        //                 srvDesc.TextureCubeArray.MostDetailedMip  = m_desc.texture.mip_slice;
        //                 srvDesc.TextureCubeArray.MipLevels        = m_desc.texture.mip_levels;
        //                 srvDesc.TextureCubeArray.First2DArrayFace = m_desc.texture.array_slice;
        //                 srvDesc.TextureCubeArray.NumCubes         = m_desc.texture.array_size / 6;
        //                 break;
        //             }
        //         case GfxShaderResourceView::StructuredBuffer:
        //             {
        //                 const buffer_desc_t& bufferDesc = ((buffer_t*)m_pResource)->GetDesc();
        //                 ASSERT(bufferDesc.usage & GfxBufferUsage::StructuredBuffer);
        //                 ASSERT(m_desc.format == Gfx::Unknown);
        //                 ASSERT(m_desc.buffer.offset % bufferDesc.stride == 0);
        //                 ASSERT(m_desc.buffer.size % bufferDesc.stride == 0);

        //                 srvDesc.Format                     = DXGI_FORMAT_UNKNOWN;
        //                 srvDesc.ViewDimension              = D3D12_SRV_DIMENSION_BUFFER;
        //                 srvDesc.Buffer.FirstElement        = m_desc.buffer.offset / bufferDesc.stride;
        //                 srvDesc.Buffer.NumElements         = m_desc.buffer.size / bufferDesc.stride;
        //                 srvDesc.Buffer.StructureByteStride = bufferDesc.stride;
        //                 break;
        //             }
        //         case GfxShaderResourceView::TypedBuffer:
        //             {
        //                 const buffer_desc_t& bufferDesc = ((buffer_t*)m_pResource)->GetDesc();
        //                 ASSERT(bufferDesc.usage & GfxBufferUsage::TypedBuffer);
        //                 ASSERT(m_desc.buffer.offset % bufferDesc.stride == 0);
        //                 ASSERT(m_desc.buffer.size % bufferDesc.stride == 0);

        //                 srvDesc.Format              = dxgi_format(m_desc.format);
        //                 srvDesc.ViewDimension       = D3D12_SRV_DIMENSION_BUFFER;
        //                 srvDesc.Buffer.FirstElement = m_desc.buffer.offset / bufferDesc.stride;
        //                 srvDesc.Buffer.NumElements  = m_desc.buffer.size / bufferDesc.stride;
        //                 break;
        //             }
        //         case GfxShaderResourceView::RawBuffer:
        //             {
        //                 const buffer_desc_t& bufferDesc = ((buffer_t*)m_pResource)->GetDesc();
        //                 ASSERT(bufferDesc.usage & GfxBufferUsage::RawBuffer);
        //                 ASSERT(bufferDesc.stride % 4 == 0);
        //                 ASSERT(m_desc.buffer.offset % 4 == 0);
        //                 ASSERT(m_desc.buffer.size % 4 == 0);

        //                 srvDesc.Format              = DXGI_FORMAT_R32_TYPELESS;
        //                 srvDesc.ViewDimension       = D3D12_SRV_DIMENSION_BUFFER;
        //                 srvDesc.Buffer.FirstElement = m_desc.buffer.offset / 4;
        //                 srvDesc.Buffer.NumElements  = m_desc.buffer.size / 4;
        //                 srvDesc.Buffer.Flags        = D3D12_BUFFER_SRV_FLAG_RAW;
        //                 break;
        //             }
        //         case GfxShaderResourceView::RayTracingTLAS:
        //             {
        //                 srvDesc.ViewDimension                            = D3D12_SRV_DIMENSION_RAYTRACING_ACCELERATION_STRUCTURE;
        //                 srvDesc.RaytracingAccelerationStructure.Location = ((D3D12RayTracingTLAS*)m_pResource)->GetGpuAddress();
        //                 break;
        //             }
        //         default: break;
        //     }

        //     m_descriptor = ((D3D12Device*)m_pDevice)->AllocateResourceDescriptor();

        //     ID3D12Device*   pDevice  = (ID3D12Device*)m_pDevice->GetHandle();
        //     ID3D12Resource* resource = m_desc.type == GfxShaderResourceView::RayTracingTLAS ? nullptr : (ID3D12Resource*)m_pResource->GetHandle();
        //     pDevice->CreateShaderResourceView(resource, &srvDesc, m_descriptor.cpu_handle);

        //     return true;
        // }

        // D3D12UnorderedAccessView::D3D12UnorderedAccessView(D3D12Device* pDevice, resource_t* pResource, const uav_desc_t& desc, const char* name)
        // {
        //     m_pDevice   = pDevice;
        //     m_name      = name;
        //     m_pResource = pResource;
        //     m_desc      = desc;
        // }

        // D3D12UnorderedAccessView::~D3D12UnorderedAccessView()
        // {
        //     ((D3D12Device*)m_pDevice)->DeleteResourceDescriptor(m_descriptor);
        //     ((D3D12Device*)m_pDevice)->DeleteNonShaderVisibleUAV(m_nonShaderVisibleDescriptor);
        // }

        // bool D3D12UnorderedAccessView::Create()
        // {
        //     D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};

        //     switch (m_desc.type)
        //     {
        //         case GfxUnorderedAccessView::Texture2D:
        //             {
        //                 const texture_desc_t& textureDesc = ((texture_t*)m_pResource)->GetDesc();
        //                 ASSERT(textureDesc.usage & GfxTextureUsage::UnorderedAccess);

        //                 uavDesc.Format               = dxgi_format(m_desc.format, false, true);
        //                 uavDesc.ViewDimension        = D3D12_UAV_DIMENSION_TEXTURE2D;
        //                 uavDesc.Texture2D.MipSlice   = m_desc.texture.mip_slice;
        //                 uavDesc.Texture2D.PlaneSlice = m_desc.texture.plane_slice;
        //                 break;
        //             }
        //         case GfxUnorderedAccessView::Texture2DArray:
        //             {
        //                 const texture_desc_t& textureDesc = ((texture_t*)m_pResource)->GetDesc();
        //                 ASSERT(textureDesc.usage & GfxTextureUsage::UnorderedAccess);

        //                 uavDesc.Format                         = dxgi_format(m_desc.format, false, true);
        //                 uavDesc.ViewDimension                  = D3D12_UAV_DIMENSION_TEXTURE2DARRAY;
        //                 uavDesc.Texture2DArray.MipSlice        = m_desc.texture.mip_slice;
        //                 uavDesc.Texture2DArray.FirstArraySlice = m_desc.texture.array_slice;
        //                 uavDesc.Texture2DArray.ArraySize       = m_desc.texture.array_size;
        //                 uavDesc.Texture2DArray.PlaneSlice      = m_desc.texture.plane_slice;
        //                 break;
        //             }
        //         case GfxUnorderedAccessView::Texture3D:
        //             {
        //                 const texture_desc_t& textureDesc = ((texture_t*)m_pResource)->GetDesc();
        //                 ASSERT(textureDesc.usage & GfxTextureUsage::UnorderedAccess);

        //                 uavDesc.Format                = dxgi_format(m_desc.format, false, true);
        //                 uavDesc.ViewDimension         = D3D12_UAV_DIMENSION_TEXTURE3D;
        //                 uavDesc.Texture3D.MipSlice    = m_desc.texture.mip_slice;
        //                 uavDesc.Texture3D.FirstWSlice = 0;
        //                 uavDesc.Texture3D.WSize       = textureDesc.depth;
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

        //                 uavDesc.Format                     = DXGI_FORMAT_UNKNOWN;
        //                 uavDesc.ViewDimension              = D3D12_UAV_DIMENSION_BUFFER;
        //                 uavDesc.Buffer.FirstElement        = m_desc.buffer.offset / bufferDesc.stride;
        //                 uavDesc.Buffer.NumElements         = m_desc.buffer.size / bufferDesc.stride;
        //                 uavDesc.Buffer.StructureByteStride = bufferDesc.stride;
        //                 break;
        //             }
        //         case GfxUnorderedAccessView::TypedBuffer:
        //             {
        //                 const buffer_desc_t& bufferDesc = ((buffer_t*)m_pResource)->GetDesc();
        //                 ASSERT(bufferDesc.usage & GfxBufferUsage::TypedBuffer);
        //                 ASSERT(bufferDesc.usage & GfxBufferUsage::UnorderedAccess);
        //                 ASSERT(m_desc.buffer.offset % bufferDesc.stride == 0);
        //                 ASSERT(m_desc.buffer.size % bufferDesc.stride == 0);

        //                 uavDesc.Format              = dxgi_format(m_desc.format);
        //                 uavDesc.ViewDimension       = D3D12_UAV_DIMENSION_BUFFER;
        //                 uavDesc.Buffer.FirstElement = m_desc.buffer.offset / bufferDesc.stride;
        //                 uavDesc.Buffer.NumElements  = m_desc.buffer.size / bufferDesc.stride;
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

        //                 uavDesc.Format              = DXGI_FORMAT_R32_TYPELESS;
        //                 uavDesc.ViewDimension       = D3D12_UAV_DIMENSION_BUFFER;
        //                 uavDesc.Buffer.FirstElement = m_desc.buffer.offset / 4;
        //                 uavDesc.Buffer.NumElements  = m_desc.buffer.size / 4;
        //                 uavDesc.Buffer.Flags        = D3D12_BUFFER_UAV_FLAG_RAW;
        //                 break;
        //             }
        //         default: break;
        //     }

        //     m_descriptor                 = ((D3D12Device*)m_pDevice)->AllocateResourceDescriptor();
        //     m_nonShaderVisibleDescriptor = ((D3D12Device*)m_pDevice)->AllocateNonShaderVisibleUAV();

        //     ID3D12Device* pDevice = (ID3D12Device*)m_pDevice->GetHandle();
        //     pDevice->CreateUnorderedAccessView((ID3D12Resource*)m_pResource->GetHandle(), nullptr, &uavDesc, m_descriptor.cpu_handle);
        //     pDevice->CreateUnorderedAccessView((ID3D12Resource*)m_pResource->GetHandle(), nullptr, &uavDesc, m_nonShaderVisibleDescriptor.cpu_handle);

        //     return true;
        // }

        // D3D12ConstantBufferView::D3D12ConstantBufferView(D3D12Device* pDevice, buffer_t* buffer, const cbv_desc_t& desc, const char* name)
        // {
        //     m_pDevice = pDevice;
        //     m_name    = name;
        //     m_pBuffer = buffer;
        //     m_desc    = desc;
        // }

        // D3D12ConstantBufferView::~D3D12ConstantBufferView() { ((D3D12Device*)m_pDevice)->DeleteResourceDescriptor(m_descriptor); }

        // bool D3D12ConstantBufferView::Create()
        // {
        //     ASSERT(m_pBuffer->GetDesc().usage & GfxBufferUsage::ConstantBuffer);

        //     D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
        //     cbvDesc.BufferLocation                  = m_pBuffer->GetGpuAddress() + m_desc.offset;
        //     cbvDesc.SizeInBytes                     = m_desc.size;

        //     m_descriptor = ((D3D12Device*)m_pDevice)->AllocateResourceDescriptor();

        //     ID3D12Device* pDevice = (ID3D12Device*)m_pDevice->GetHandle();
        //     pDevice->CreateConstantBufferView(&cbvDesc, m_descriptor.cpu_handle);

        //     return true;
        // }

        // D3D12Sampler::D3D12Sampler(D3D12Device* pDevice, const sampler_desc_t& desc, const char* name)
        // {
        //     m_pDevice = pDevice;
        //     m_name    = name;
        //     m_desc    = desc;
        // }

        // D3D12Sampler::~D3D12Sampler() { ((D3D12Device*)m_pDevice)->DeleteSampler(m_descriptor); }

        // bool D3D12Sampler::Create()
        // {
        //     D3D12_SAMPLER_DESC samplerDesc = {};
        //     samplerDesc.Filter             = d3d12_filter(m_desc);
        //     samplerDesc.AddressU           = d3d12_address_mode(m_desc.address_u);
        //     samplerDesc.AddressV           = d3d12_address_mode(m_desc.address_v);
        //     samplerDesc.AddressW           = d3d12_address_mode(m_desc.address_w);
        //     samplerDesc.MipLODBias         = m_desc.mip_bias;
        //     samplerDesc.MaxAnisotropy      = (UINT)m_desc.max_anisotropy;
        //     samplerDesc.ComparisonFunc     = m_desc.reduction_mode == GfxSamplerReductionMode::Compare ? d3d12_compare_func(m_desc.compare_func) : D3D12_COMPARISON_FUNC_NONE;
        //     samplerDesc.MinLOD             = m_desc.min_lod;
        //     samplerDesc.MaxLOD             = m_desc.max_lod;
        //     memcpy(samplerDesc.BorderColor, m_desc.border_color, sizeof(float) * 4);

        //     m_descriptor = ((D3D12Device*)m_pDevice)->AllocateSampler();

        //     ID3D12Device* pDevice = (ID3D12Device*)m_pDevice->GetHandle();
        //     pDevice->CreateSampler(&samplerDesc, m_descriptor.cpu_handle);

        //     return true;
        // }

#ifdef TARGET_PC

        namespace nd3d12
        {
            void CreateSrv(ngfx::device_t* device, ngfx::descriptor_t* descriptor, ngfx::texture_t* texture, const srv_desc_t& desc)
            {
                nd3d12::srv_texture_t* srv       = CreateComponent<ngfx::descriptor_t, nd3d12::srv_texture_t>(device, descriptor);
                nd3d12::texture_t*     dxtexture = GetComponent<ngfx::texture_t, nd3d12::texture_t>(device, texture);
                srv->m_texture                   = dxtexture;
                srv->m_desc                      = desc;
            }

            void CreateSrv(ngfx::device_t* device, ngfx::descriptor_t* descriptor, ngfx::buffer_t* buffer, const srv_desc_t& desc)
            {
                nd3d12::srv_buffer_t* srv      = CreateComponent<ngfx::descriptor_t, nd3d12::srv_buffer_t>(device, descriptor);
                nd3d12::buffer_t*     dxbuffer = GetComponent<ngfx::buffer_t, nd3d12::buffer_t>(device, buffer);
                srv->m_buffer                  = dxbuffer;
                srv->m_desc                    = desc;
            }

            void CreateSrv(ngfx::device_t* device, ngfx::descriptor_t* descriptor, ngfx::tlas_t* tlas, const srv_desc_t& desc)
            {
                nd3d12::srv_tlas_t* srv    = CreateComponent<ngfx::descriptor_t, nd3d12::srv_tlas_t>(device, descriptor);
                nd3d12::tlas_t*     dxtlas = GetComponent<ngfx::tlas_t, nd3d12::tlas_t>(device, tlas);
                srv->m_tlas                = dxtlas;
                srv->m_desc                = desc;
            }

            void CreateUav(ngfx::device_t* device, ngfx::descriptor_t* descriptor, ngfx::texture_t* buffer, const uav_desc_t& desc)
            {
                nd3d12::uav_texture_t* uav      = CreateComponent<ngfx::descriptor_t, nd3d12::uav_texture_t>(device, descriptor);
                nd3d12::texture_t*     dxbuffer = GetComponent<ngfx::texture_t, nd3d12::texture_t>(device, buffer);
                uav->m_texture                  = dxbuffer;
                uav->m_desc                     = desc;
            }

            void CreateUav(ngfx::device_t* device, ngfx::descriptor_t* descriptor, ngfx::buffer_t* buffer, const uav_desc_t& desc)
            {
                nd3d12::uav_buffer_t* uav      = CreateComponent<ngfx::descriptor_t, nd3d12::uav_buffer_t>(device, descriptor);
                nd3d12::buffer_t*     dxbuffer = GetComponent<ngfx::buffer_t, nd3d12::buffer_t>(device, buffer);
                uav->m_buffer                  = dxbuffer;
                uav->m_desc                    = desc;
            }

            void CreateCbv(ngfx::device_t* device, ngfx::descriptor_t* descriptor, ngfx::buffer_t* buffer, const cbv_desc_t& desc)
            {
                nd3d12::cbv_t*    cbv      = CreateComponent<ngfx::descriptor_t, nd3d12::cbv_t>(device, descriptor);
                nd3d12::buffer_t* dxbuffer = GetComponent<ngfx::buffer_t, nd3d12::buffer_t>(device, buffer);
                cbv->m_buffer              = dxbuffer;
                cbv->m_desc                = desc;
            }

            void CreateSampler(ngfx::device_t* device, ngfx::descriptor_t* descriptor, const sampler_desc_t& desc)
            {
                nd3d12::sampler_t* sampler = CreateComponent<ngfx::descriptor_t, nd3d12::sampler_t>(device, descriptor);
                sampler->m_desc            = desc;
            }

            void DestroyDescriptor(ngfx::device_t* device, ngfx::descriptor_t* descriptor)
            {
                nd3d12::Destroy(device, descriptor);

                switch (descriptor->m_type)
                {
                    case enums::DescriptorTypeSrvTexture: DestroyComponent<ngfx::descriptor_t, nd3d12::srv_texture_t>(device, descriptor); break;
                    case enums::DescriptorTypeSrvBuffer: DestroyComponent<ngfx::descriptor_t, nd3d12::srv_buffer_t>(device, descriptor); break;
                    case enums::DescriptorTypeSrvRayTracingTLAS: DestroyComponent<ngfx::descriptor_t, nd3d12::srv_tlas_t>(device, descriptor); break;
                    case enums::DescriptorTypeUavTexture: DestroyComponent<ngfx::descriptor_t, nd3d12::uav_texture_t>(device, descriptor); break;
                    case enums::DescriptorTypeUavBuffer: DestroyComponent<ngfx::descriptor_t, nd3d12::uav_buffer_t>(device, descriptor); break;
                    case enums::DescriptorTypeCbv: DestroyComponent<ngfx::descriptor_t, nd3d12::cbv_t>(device, descriptor); break;
                    case enums::DescriptorTypeSampler: DestroyComponent<ngfx::descriptor_t, nd3d12::sampler_t>(device, descriptor); break;
                    default: break;
                }
            }

            void Destroy(ngfx::device_t* device, ngfx::descriptor_t* descriptor)
            {
                nd3d12::device_t* dxdevice = GetComponent<ngfx::device_t, nd3d12::device_t>(device, device);
                switch (descriptor->m_type)
                {
                    case enums::DescriptorTypeSrvTexture:
                        {
                            nd3d12::srv_texture_t* srv = GetComponent<ngfx::descriptor_t, nd3d12::srv_texture_t>(device, descriptor);
                            if (IsNullDescriptor(srv->m_descriptor))
                            {
                                DeleteResourceDescriptor(dxdevice, srv->m_descriptor);
                            }
                            break;
                        }
                    case enums::DescriptorTypeSrvBuffer:
                        {
                            nd3d12::srv_buffer_t* srv = GetComponent<ngfx::descriptor_t, nd3d12::srv_buffer_t>(device, descriptor);
                            if (IsNullDescriptor(srv->m_descriptor))
                            {
                                DeleteResourceDescriptor(dxdevice, srv->m_descriptor);
                            }
                            break;
                        }
                    case enums::DescriptorTypeSrvRayTracingTLAS:
                        {
                            nd3d12::srv_tlas_t* srv = GetComponent<ngfx::descriptor_t, nd3d12::srv_tlas_t>(device, descriptor);
                            if (IsNullDescriptor(srv->m_descriptor))
                            {
                                DeleteResourceDescriptor(dxdevice, srv->m_descriptor);
                            }
                            break;
                        }
                    case enums::DescriptorTypeUavTexture:
                        {
                            nd3d12::uav_texture_t* uav = GetComponent<ngfx::descriptor_t, nd3d12::uav_texture_t>(device, descriptor);
                            if (IsNullDescriptor(uav->m_descriptor))
                            {
                                DeleteResourceDescriptor(dxdevice, uav->m_descriptor);
                            }
                            if (IsNullDescriptor(uav->m_nonShaderVisibleDescriptor))
                            {
                                DeleteNonShaderVisibleUAV(dxdevice, uav->m_nonShaderVisibleDescriptor);
                            }
                            break;
                        }
                    case enums::DescriptorTypeUavBuffer:
                        {
                            nd3d12::uav_buffer_t* uav = GetComponent<ngfx::descriptor_t, nd3d12::uav_buffer_t>(device, descriptor);
                            if (IsNullDescriptor(uav->m_descriptor))
                            {
                                DeleteResourceDescriptor(dxdevice, uav->m_descriptor);
                            }
                            if (IsNullDescriptor(uav->m_nonShaderVisibleDescriptor))
                            {
                                DeleteNonShaderVisibleUAV(dxdevice, uav->m_nonShaderVisibleDescriptor);
                            }
                            break;
                        }
                    case enums::DescriptorTypeCbv:
                        {
                            nd3d12::cbv_t* cbv = GetComponent<ngfx::descriptor_t, nd3d12::cbv_t>(device, descriptor);
                            if (IsNullDescriptor(cbv->m_descriptor))
                            {
                                DeleteResourceDescriptor(dxdevice, cbv->m_descriptor);
                            }
                            break;
                        }
                    case enums::DescriptorTypeSampler:
                        {
                            nd3d12::sampler_t* sampler = GetComponent<ngfx::descriptor_t, nd3d12::sampler_t>(device, descriptor);
                            if (IsNullDescriptor(sampler->m_descriptor))
                            {
                                DeleteSampler(dxdevice, sampler->m_descriptor);
                            }
                            break;
                        }
                    default: break;
                }
            }

            bool Create(ngfx::device_t* device, ngfx::descriptor_t* descriptor)
            {
                nd3d12::device_t* dxdevice = GetComponent<ngfx::device_t, nd3d12::device_t>(device, device);
                switch (descriptor->m_type)
                {
                    case enums::DescriptorTypeSrvTexture:
                        {
                            nd3d12::srv_texture_t* srv = GetComponent<ngfx::descriptor_t, nd3d12::srv_texture_t>(device, descriptor);
                            // ... create d3d shader resource view
                            break;
                        }
                    case enums::DescriptorTypeSrvBuffer:
                        {
                            nd3d12::srv_buffer_t* srv = GetComponent<ngfx::descriptor_t, nd3d12::srv_buffer_t>(device, descriptor);
                            // ... create d3d shader resource view
                            break;
                        }
                    case enums::DescriptorTypeSrvRayTracingTLAS:
                        {
                            nd3d12::srv_tlas_t* srv = GetComponent<ngfx::descriptor_t, nd3d12::srv_tlas_t>(device, descriptor);
                            // ... create d3d shader resource view
                            break;
                        }
                    case enums::DescriptorTypeUavTexture:
                        {
                            nd3d12::uav_texture_t* uav = GetComponent<ngfx::descriptor_t, nd3d12::uav_texture_t>(device, descriptor);
                            // ... create d3d unordered access view
                            break;
                        }
                    case enums::DescriptorTypeUavBuffer:
                        {
                            nd3d12::uav_buffer_t* uav = GetComponent<ngfx::descriptor_t, nd3d12::uav_buffer_t>(device, descriptor);
                            // ... create d3d unordered access view
                            break;
                        }
                    case enums::DescriptorTypeCbv:
                        {
                            nd3d12::cbv_t* cbv = GetComponent<ngfx::descriptor_t, nd3d12::cbv_t>(device, descriptor);
                            // ... create d3d constant buffer view
                            break;
                        }
                    case enums::DescriptorTypeSampler:
                        {
                            nd3d12::sampler_t* sampler = GetComponent<ngfx::descriptor_t, nd3d12::sampler_t>(device, descriptor);
                            // ... create d3d sampler
                            break;
                        }
                    default: break;
                }

                return false;
            }

            u32 GetHeapIndex(ngfx::device_t* pDevice, ngfx::descriptor_t* d) { return 0; }

            bool GetShaderVisibleDescriptor(ngfx::device_t* pDevice, ngfx::descriptor_t* d, D3D12Descriptor& outDescriptor)
            {
                outDescriptor = {};
                switch (d->m_type)
                {
                    case enums::DescriptorTypeSrvTexture:
                        {
                            nd3d12::srv_texture_t* srv = GetComponent<ngfx::descriptor_t, nd3d12::srv_texture_t>(pDevice, d);
                            outDescriptor              = srv->m_descriptor;
                            return true;
                        }
                    case enums::DescriptorTypeSrvBuffer:
                        {
                            nd3d12::srv_buffer_t* srv = GetComponent<ngfx::descriptor_t, nd3d12::srv_buffer_t>(pDevice, d);
                            outDescriptor             = srv->m_descriptor;
                            return true;
                        }
                    case enums::DescriptorTypeSrvRayTracingTLAS:
                        {
                            nd3d12::srv_tlas_t* srv = GetComponent<ngfx::descriptor_t, nd3d12::srv_tlas_t>(pDevice, d);
                            outDescriptor           = srv->m_descriptor;
                            return true;
                        }
                    case enums::DescriptorTypeUavTexture:
                        {
                            nd3d12::uav_texture_t* uav = GetComponent<ngfx::descriptor_t, nd3d12::uav_texture_t>(pDevice, d);
                            outDescriptor              = uav->m_descriptor;
                            return true;
                        }
                    case enums::DescriptorTypeUavBuffer:
                        {
                            nd3d12::uav_buffer_t* uav = GetComponent<ngfx::descriptor_t, nd3d12::uav_buffer_t>(pDevice, d);
                            outDescriptor             = uav->m_descriptor;
                            return true;
                        }
                    case enums::DescriptorTypeCbv:
                        {
                            nd3d12::cbv_t* cbv = GetComponent<ngfx::descriptor_t, nd3d12::cbv_t>(pDevice, d);
                            outDescriptor      = cbv->m_descriptor;
                            return true;
                        }
                    case enums::DescriptorTypeSampler:
                        {
                            nd3d12::sampler_t* sampler = GetComponent<ngfx::descriptor_t, nd3d12::sampler_t>(pDevice, d);
                            outDescriptor              = sampler->m_descriptor;
                            return true;
                        }
                    default: break;
                }

                return false;
            }

            bool GetNonShaderVisibleDescriptor(ngfx::device_t* pDevice, ngfx::descriptor_t* d, D3D12Descriptor& outDescriptor)
            {
                outDescriptor = {};
                switch (d->m_type)
                {
                    case enums::DescriptorTypeUavTexture:
                        {
                            nd3d12::uav_texture_t* uav = GetComponent<ngfx::descriptor_t, nd3d12::uav_texture_t>(pDevice, d);
                            outDescriptor              = uav->m_nonShaderVisibleDescriptor;
                            return true;
                        }
                    case enums::DescriptorTypeUavBuffer:
                        {
                            nd3d12::uav_buffer_t* uav = GetComponent<ngfx::descriptor_t, nd3d12::uav_buffer_t>(pDevice, d);
                            outDescriptor             = uav->m_nonShaderVisibleDescriptor;
                            return true;
                        }
                    default: break;
                }

                return false;
            }

        }  // namespace nd3d12
#endif
    }  // namespace ngfx
}  // namespace ncore
