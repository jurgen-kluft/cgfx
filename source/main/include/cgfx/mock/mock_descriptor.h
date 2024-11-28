#ifndef __CGFX_MOCK_DESCRIPTOR_H__
#define __CGFX_MOCK_DESCRIPTOR_H__

#include "cgfx/gfx_descriptor.h"

namespace ncore
{
    namespace ngfx
    {
        namespace nmock
        {
            struct srv_texture_t
            {
                D_GFX_CS_COMPONENT_SET(enums::ComponentMockDescriptorSrvTexture);
                texture_t* m_texture;
                srv_desc_t m_desc = {};
                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            struct srv_buffer_t
            {
                D_GFX_CS_COMPONENT_SET(enums::ComponentMockDescriptorSrvBuffer);
                buffer_t*  m_buffer;
                srv_desc_t m_desc = {};
                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            struct srv_tlas_t
            {
                D_GFX_CS_COMPONENT_SET(enums::ComponentMockDescriptorSrvRayTracingTLAS);
                tlas_t*    m_tlas;
                srv_desc_t m_desc = {};
                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            struct uav_texture_t
            {
                D_GFX_CS_COMPONENT_SET(enums::ComponentMockDescriptorUavTexture);
                ngfx::uav_desc_t m_desc = {};
                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            struct uav_buffer_t
            {
                D_GFX_CS_COMPONENT_SET(enums::ComponentMockDescriptorUavBuffer);
                ngfx::uav_desc_t m_desc = {};
                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            struct cbv_t
            {
                D_GFX_CS_COMPONENT_SET(enums::ComponentMockDescriptorCbv);
                ngfx::cbv_desc_t m_desc = {};
                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            struct sampler_t
            {
                D_GFX_CS_COMPONENT_SET(enums::ComponentMockDescriptorSampler);
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

            void  Destroy(ngfx::device_t* pDevice, ngfx::descriptor_t* d);
            bool  Create(ngfx::device_t* pDevice, ngfx::descriptor_t* d);
            void* GetHandle(ngfx::device_t* pDevice, ngfx::descriptor_t* d);
            u32   GetHeapIndex(ngfx::device_t* pDevice, ngfx::descriptor_t* d);

        }  // namespace nmock
    }  // namespace ngfx
}  // namespace ncore

#endif
