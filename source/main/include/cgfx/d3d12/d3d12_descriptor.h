#ifndef __CGFX_D3D12_DESCRIPTOR_H__
#define __CGFX_D3D12_DESCRIPTOR_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
    #pragma once
#endif

#include "cgfx/gfx_descriptor.h"
#include "cgfx/gfx_buffer.h"
#include "cgfx/gfx_texture.h"
#include "cgfx/gfx_rt_tlas.h"
#include "cgfx/d3d12/d3d12_header.h"

namespace ncore
{
    namespace ngfx
    {
#ifdef TARGET_PC
        namespace nd3d12
        {
            struct texture_t;
            struct buffer_t;
            struct tlas_t;

            struct srv_texture_t
            {
                D_GFX_CS_COMPONENT_SET(enums::ComponentD3D12DescriptorSrvTexture);
                nd3d12::texture_t* m_texture = nullptr;
                srv_desc_t         m_desc    = {};
                D3D12Descriptor    m_descriptor;
                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            struct srv_buffer_t
            {
                D_GFX_CS_COMPONENT_SET(enums::ComponentD3D12DescriptorSrvBuffer);
                nd3d12::buffer_t* m_buffer = nullptr;
                srv_desc_t        m_desc   = {};
                D3D12Descriptor   m_descriptor;
                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            struct srv_tlas_t
            {
                D_GFX_CS_COMPONENT_SET(enums::ComponentD3D12DescriptorSrvRayTracingTLAS);
                nd3d12::tlas_t* m_tlas = nullptr;
                srv_desc_t      m_desc = {};
                D3D12Descriptor m_descriptor;
                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            struct uav_texture_t
            {
                D_GFX_CS_COMPONENT_SET(enums::ComponentD3D12DescriptorUavTexture);
                nd3d12::texture_t* m_texture = nullptr;
                uav_desc_t         m_desc    = {};
                D3D12Descriptor    m_descriptor;
                D3D12Descriptor    m_nonShaderVisibleDescriptor;  // for uav clear
                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            struct uav_buffer_t
            {
                D_GFX_CS_COMPONENT_SET(enums::ComponentD3D12DescriptorUavBuffer);
                nd3d12::buffer_t* m_buffer = nullptr;
                uav_desc_t        m_desc   = {};
                D3D12Descriptor   m_descriptor;
                D3D12Descriptor   m_nonShaderVisibleDescriptor;  // for uav clear
                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            struct cbv_t
            {
                D_GFX_CS_COMPONENT_SET(enums::ComponentD3D12DescriptorCbv);
                nd3d12::buffer_t* m_buffer = nullptr;
                cbv_desc_t        m_desc   = {};
                D3D12Descriptor   m_descriptor;
                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            struct sampler_t
            {
                D_GFX_CS_COMPONENT_SET(enums::ComponentD3D12DescriptorSampler);
                sampler_desc_t  m_desc;
                D3D12Descriptor m_descriptor;
                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            void CreateSrv(ngfx::device_t* device, ngfx::descriptor_t* descriptor, ngfx::texture_t* texture, const srv_desc_t& desc);
            void CreateSrv(ngfx::device_t* device, ngfx::descriptor_t* descriptor, ngfx::buffer_t* buffer, const srv_desc_t& desc);
            void CreateSrv(ngfx::device_t* device, ngfx::descriptor_t* descriptor, ngfx::tlas_t* tlas, const srv_desc_t& desc);
            void CreateUav(ngfx::device_t* device, ngfx::descriptor_t* descriptor, ngfx::texture_t* buffer, const uav_desc_t& desc);
            void CreateUav(ngfx::device_t* device, ngfx::descriptor_t* descriptor, ngfx::buffer_t* buffer, const uav_desc_t& desc);
            void CreateCbv(ngfx::device_t* device, ngfx::descriptor_t* descriptor, ngfx::buffer_t* buffer, const cbv_desc_t& desc);
            void CreateSampler(ngfx::device_t* device, ngfx::descriptor_t* descriptor, const sampler_desc_t& desc);
            void DestroyDescriptor(ngfx::device_t* device, ngfx::descriptor_t* descriptor);

            void Destroy(ngfx::device_t* device, ngfx::descriptor_t* descriptor);
            bool Create(ngfx::device_t* device, ngfx::descriptor_t* descriptor);

            u32  GetHeapIndex(ngfx::device_t* pDevice, ngfx::descriptor_t* d);
            bool GetShaderVisibleDescriptor(ngfx::device_t* pDevice, ngfx::descriptor_t* d, D3D12Descriptor& outDescriptor);
            bool GetNonShaderVisibleDescriptor(ngfx::device_t* pDevice, ngfx::descriptor_t* d, D3D12Descriptor& outDescriptor);
        }  // namespace nd3d12
#else
        namespace nd3d12
        {
            inline void CreateSrv(ngfx::device_t* device, ngfx::descriptor_t* descriptor, ngfx::texture_t* texture, const srv_desc_t& desc) {}
            inline void CreateSrv(ngfx::device_t* device, ngfx::descriptor_t* descriptor, ngfx::buffer_t* buffer, const srv_desc_t& desc) {}
            inline void CreateSrv(ngfx::device_t* device, ngfx::descriptor_t* descriptor, ngfx::tlas_t* tlas, const srv_desc_t& desc) {}
            inline void CreateUav(ngfx::device_t* device, ngfx::descriptor_t* descriptor, ngfx::texture_t* buffer, const uav_desc_t& desc) {}
            inline void CreateUav(ngfx::device_t* device, ngfx::descriptor_t* descriptor, ngfx::buffer_t* buffer, const uav_desc_t& desc) {}
            inline void CreateCbv(ngfx::device_t* device, ngfx::descriptor_t* descriptor, ngfx::buffer_t* buffer, const cbv_desc_t& desc) {}
            inline void CreateSampler(ngfx::device_t* device, ngfx::descriptor_t* descriptor, const sampler_desc_t& desc) {}
            inline void DestroyDescriptor(ngfx::device_t* device, ngfx::descriptor_t* descriptor) {}

            inline void Destroy(ngfx::device_t* device, ngfx::descriptor_t* descriptor) {}
            inline bool Create(ngfx::device_t* device, ngfx::descriptor_t* descriptor) { return false; }
            inline u32  GetHeapIndex(ngfx::device_t* pDevice, ngfx::descriptor_t* d) { return 0; }
        }  // namespace nd3d12
#endif
    }  // namespace ngfx
}  // namespace ncore

#endif
