#include "cgfx/gfx_defines.h"

#include "cgfx/mock/mock_descriptor.h"
#include "cgfx/mock/mock_device.h"

namespace ncore
{
    namespace ngfx
    {
        namespace nmock
        {
            struct srv_texture_t
            {
                D_GFX_OCS_COMPONENT_SET(enums::ComponentMockDescriptorSrvTexture);
                texture_t* m_texture;
                srv_desc_t m_desc = {};
                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            struct srv_buffer_t
            {
                D_GFX_OCS_COMPONENT_SET(enums::ComponentMockDescriptorSrvBuffer);
                buffer_t*  m_buffer;
                srv_desc_t m_desc = {};
                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            struct srv_tlas_t
            {
                D_GFX_OCS_COMPONENT_SET(enums::ComponentMockDescriptorSrvRayTracingTLAS);
                tlas_t*    m_tlas;
                srv_desc_t m_desc = {};
                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            struct uav_texture_t
            {
                D_GFX_OCS_COMPONENT_SET(enums::ComponentMockDescriptorUavTexture);
                ngfx::uav_desc_t m_desc = {};
                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            struct uav_buffer_t
            {
                D_GFX_OCS_COMPONENT_SET(enums::ComponentMockDescriptorUavBuffer);
                ngfx::uav_desc_t m_desc = {};
                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            struct cbv_t
            {
                D_GFX_OCS_COMPONENT_SET(enums::ComponentMockDescriptorCbv);
                ngfx::cbv_desc_t m_desc = {};
                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            struct sampler_t
            {
                D_GFX_OCS_COMPONENT_SET(enums::ComponentMockDescriptorSampler);
                ngfx::sampler_desc_t m_desc;
                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            // -------------------------------------------------------------------------------------------------------------------------------
            // -------------------------------------------------------------------------------------------------------------------------------

            ngfx::descriptor_t* CreateSrv(ngfx::device_t* pDevice, descriptor_t* descriptor, texture_t* texture, const ngfx::srv_desc_t& desc)
            {
                srv_texture_t* srv_texture = CreateComponent<descriptor_t, srv_texture_t>(pDevice, descriptor);
                srv_texture->m_texture     = texture;
                srv_texture->m_desc        = desc;
                return descriptor;
            }

            ngfx::descriptor_t* CreateSrv(ngfx::device_t* pDevice, descriptor_t* descriptor, buffer_t* buffer, const ngfx::srv_desc_t& desc)
            {
                srv_buffer_t* srv_buffer = CreateComponent<descriptor_t, srv_buffer_t>(pDevice, descriptor);
                srv_buffer->m_buffer     = buffer;
                srv_buffer->m_desc       = desc;
                return descriptor;
            }

            ngfx::descriptor_t* CreateSrv(ngfx::device_t* pDevice, descriptor_t* descriptor, tlas_t* tlas, const ngfx::srv_desc_t& desc)
            {
                srv_tlas_t* srv_tlas = CreateComponent<descriptor_t, srv_tlas_t>(pDevice, descriptor);
                srv_tlas->m_tlas     = tlas;
                srv_tlas->m_desc     = desc;
                return descriptor;
            }

            ngfx::descriptor_t* CreateUav(ngfx::device_t* pDevice, descriptor_t* descriptor, texture_t* texture, const uav_desc_t& desc)
            {
                uav_texture_t* uav  = CreateComponent<descriptor_t, uav_texture_t>(pDevice, descriptor);
                uav->m_desc = desc;
                return descriptor;
            }

            ngfx::descriptor_t* CreateUav(ngfx::device_t* pDevice, descriptor_t* descriptor, buffer_t* buffer, const uav_desc_t& desc)
            {
                uav_buffer_t* uav  = CreateComponent<descriptor_t, uav_buffer_t>(pDevice, descriptor);
                uav->m_desc = desc;
                return descriptor;
            }

            ngfx::descriptor_t* CreateCbv(ngfx::device_t* pDevice, descriptor_t* descriptor, buffer_t* buffer, const cbv_desc_t& desc)
            {
                cbv_t* cbv  = CreateComponent<descriptor_t, cbv_t>(pDevice, descriptor);
                cbv->m_desc = desc;
                return descriptor;
            }

            ngfx::descriptor_t* CreateSampler(ngfx::device_t* pDevice, descriptor_t* descriptor, const sampler_desc_t& desc)
            {
                sampler_t* sampler = CreateComponent<descriptor_t, sampler_t>(pDevice, descriptor);
                sampler->m_desc    = desc;
                return descriptor;
            }

            void  Destroy(ngfx::device_t* pDevice, ngfx::descriptor_t* d) {}
            bool  Create(ngfx::device_t* pDevice, ngfx::descriptor_t* d) { return true; }
            void* GetHandle(ngfx::device_t* pDevice, ngfx::descriptor_t* d) { return nullptr; }
            u32   GetHeapIndex(ngfx::device_t* pDevice, ngfx::descriptor_t* d) { return 0; }

        }  // namespace nmock
    }  // namespace ngfx
}  // namespace ncore
