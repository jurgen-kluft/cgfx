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
                D_GFX_OCS_COMPONENT;
                texture_t* m_texture;
                srv_desc_t m_desc = {};
                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            struct srv_buffer_t
            {
                D_GFX_OCS_COMPONENT;
                buffer_t*  m_buffer;
                srv_desc_t m_desc = {};
                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            struct uav_t
            {
                D_GFX_OCS_COMPONENT;
                ngfx::uav_desc_t m_desc = {};
                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            struct cbv_t
            {
                D_GFX_OCS_COMPONENT;
                ngfx::cbv_desc_t m_desc = {};
                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            struct sampler_t
            {
                D_GFX_OCS_COMPONENT;
                ngfx::sampler_desc_t m_desc;
                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            // -------------------------------------------------------------------------------------------------------------------------------
            // -------------------------------------------------------------------------------------------------------------------------------

            ngfx::descriptor_t* CreateSrv(ngfx::device_t* pDevice, resource_t* resource, descriptor_t* descriptor, texture_t* texture, const ngfx::srv_desc_t& desc)
            {
                srv_texture_t* srv_texture = CreateComponent<resource_t, srv_texture_t>(pDevice, resource);
                srv_texture->m_texture     = texture;
                srv_texture->m_desc        = desc;
                return descriptor;
            }

            ngfx::descriptor_t* CreateSrv(ngfx::device_t* pDevice, resource_t* resource, descriptor_t* descriptor, buffer_t* buffer, const ngfx::srv_desc_t& desc)
            {
                srv_buffer_t* srv_buffer = CreateComponent<resource_t, srv_buffer_t>(pDevice, resource);
                srv_buffer->m_buffer     = buffer;
                srv_buffer->m_desc       = desc;
                return descriptor;
            }

            ngfx::descriptor_t* CreateUav(ngfx::device_t* pDevice, resource_t* resource, descriptor_t* descriptor, texture_t* texture, const uav_desc_t& desc)
            {
                uav_t* uav  = CreateComponent<resource_t, uav_t>(pDevice, resource);
                uav->m_desc = desc;
                return descriptor;
            }

            ngfx::descriptor_t* CreateUav(ngfx::device_t* pDevice, resource_t* resource, descriptor_t* descriptor, buffer_t* buffer, const uav_desc_t& desc)
            {
                uav_t* uav  = CreateComponent<resource_t, uav_t>(pDevice, resource);
                uav->m_desc = desc;
                return descriptor;
            }

            ngfx::descriptor_t* CreateCbv(ngfx::device_t* pDevice, resource_t* resource, descriptor_t* descriptor, buffer_t* buffer, const cbv_desc_t& desc)
            {
                cbv_t* cbv  = CreateComponent<resource_t, cbv_t>(pDevice, resource);
                cbv->m_desc = desc;
                return descriptor;
            }

            ngfx::descriptor_t* CreateSampler(ngfx::device_t* pDevice, resource_t* resource, descriptor_t* descriptor, const sampler_desc_t& desc)
            {
                sampler_t* sampler = CreateComponent<resource_t, sampler_t>(pDevice, resource);
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
