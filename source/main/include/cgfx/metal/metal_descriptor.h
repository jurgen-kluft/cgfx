#ifndef __CGFX_METAL_DESCRIPTOR_H__
#define __CGFX_METAL_DESCRIPTOR_H__

#include "cgfx/metal/metal_utils.h"
#include "cgfx/gfx_descriptor.h"
#include "cgfx/gfx_buffer.h"
#include "cgfx/metal/metal_rt_tlas.h"

namespace ncore
{
    namespace ngfx
    {
#ifdef TARGET_MAC
        namespace nmetal
        {
            struct mbuffer_t;
            struct mtexture_t;

            struct srv_base_t
            {
                MTL::Texture* m_textureView;
                u32           m_heapIndex = GFX_INVALID_RESOURCE;
                srv_desc_t    m_desc      = {};
            };

            struct srv_texture_t
            {
                D_GFX_OCS_COMPONENT_SET(enums::ComponentMetalDescriptorSrvTexture);
                nmetal::mtexture_t* m_texture;
                srv_base_t          m_base;
                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            struct srv_buffer_t
            {
                D_GFX_OCS_COMPONENT_SET(enums::ComponentMetalDescriptorSrvBuffer);
                nmetal::mbuffer_t* m_buffer;
                srv_base_t         m_base;
                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            struct srv_tlas_t
            {
                D_GFX_OCS_COMPONENT_SET(enums::ComponentMetalDescriptorSrvRayTracingTLAS);
                nmetal::mtlas_t* m_tlas;
                srv_base_t       m_base;
                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            struct uav_base_t
            {
                MTL::Texture*    m_textureView;
                u32              m_heapIndex = GFX_INVALID_RESOURCE;
                ngfx::uav_desc_t m_desc      = {};
            };

            struct uav_texture_t
            {
                D_GFX_OCS_COMPONENT_SET(enums::ComponentMetalDescriptorUavTexture);
                nmetal::mtexture_t* m_texture;
                uav_base_t          m_base;
                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            struct uav_buffer_t
            {
                D_GFX_OCS_COMPONENT_SET(enums::ComponentMetalDescriptorUavBuffer);
                nmetal::mbuffer_t* m_buffer;
                uav_base_t         m_base;
                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            struct cbv_t
            {
                D_GFX_OCS_COMPONENT_SET(enums::ComponentMetalDescriptorCbv);
                nmetal::mbuffer_t* m_buffer;
                u32                m_heapIndex = GFX_INVALID_RESOURCE;
                ngfx::cbv_desc_t   m_desc      = {};
                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            struct sampler_t
            {
                D_GFX_OCS_COMPONENT_SET(enums::ComponentMetalDescriptorSampler);
                MTL::SamplerState*   m_pSampler  = nullptr;
                u32                  m_heapIndex = GFX_INVALID_RESOURCE;
                ngfx::sampler_desc_t m_desc;
                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            void CreateSrv(ngfx::device_t* pDevice, descriptor_t* descriptor, texture_t* texture, const ngfx::srv_desc_t& desc);
            void CreateSrv(ngfx::device_t* pDevice, descriptor_t* descriptor, buffer_t* buffer, const ngfx::srv_desc_t& desc);
            void CreateSrv(ngfx::device_t* pDevice, descriptor_t* descriptor, tlas_t* tlas, const ngfx::srv_desc_t& desc);
            void CreateUav(ngfx::device_t* pDevice, descriptor_t* descriptor, texture_t* buffer, const uav_desc_t& desc);
            void CreateUav(ngfx::device_t* pDevice, descriptor_t* descriptor, buffer_t* buffer, const uav_desc_t& desc);
            void CreateCbv(ngfx::device_t* pDevice, descriptor_t* descriptor, buffer_t* buffer, const cbv_desc_t& desc);
            void CreateSampler(ngfx::device_t* pDevice, descriptor_t* descriptor, const sampler_desc_t& desc);

            void Destroy(ngfx::device_t* pDevice, ngfx::descriptor_t* d);
            bool Create(ngfx::device_t* pDevice, ngfx::descriptor_t* d);
            u32  GetHeapIndex(ngfx::device_t* pDevice, ngfx::descriptor_t* d);
        }  // namespace nmetal
#else
        namespace nmetal
        {
            void CreateSrv(ngfx::device_t* pDevice, ngfx::descriptor_t* descriptor, texture_t* texture, const ngfx::srv_desc_t& desc) {}
            void CreateSrv(ngfx::device_t* pDevice, ngfx::descriptor_t* descriptor, buffer_t* buffer, const ngfx::srv_desc_t& desc) {}
            void CreateSrv(ngfx::device_t* pDevice, ngfx::descriptor_t* descriptor, tlas_t* tlas, const ngfx::srv_desc_t& desc) {}
            void CreateUav(ngfx::device_t* pDevice, ngfx::descriptor_t* descriptor, texture_t* buffer, const uav_desc_t& desc) {}
            void CreateUav(ngfx::device_t* pDevice, ngfx::descriptor_t* descriptor, buffer_t* buffer, const uav_desc_t& desc) {}
            void CreateCbv(ngfx::device_t* pDevice, ngfx::descriptor_t* descriptor, buffer_t* buffer, const cbv_desc_t& desc) {}
            void CreateSampler(ngfx::device_t* pDevice, ngfx::descriptor_t* descriptor, const sampler_desc_t& desc) {}

            void Destroy(ngfx::device_t* pDevice, ngfx::descriptor_t* d) {}
            bool Create(ngfx::device_t* pDevice, ngfx::descriptor_t* d) { return true; }
            u32  GetHeapIndex(ngfx::device_t* pDevice, ngfx::descriptor_t* d) { return GFX_INVALID_RESOURCE; }
        }  // namespace nmetal
#endif
    }  // namespace ngfx
}  // namespace ncore
#endif
