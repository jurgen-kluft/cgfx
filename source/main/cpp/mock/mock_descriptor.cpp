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
            };

            struct srv_buffer_t
            {
                D_GFX_OCS_COMPONENT;
                buffer_t*  m_buffer;
                srv_desc_t m_desc = {};
            };

            struct uav_t
            {
                D_GFX_OCS_COMPONENT;
                ngfx::uav_desc_t m_desc = {};
            };

            struct cbv_t
            {
                D_GFX_OCS_COMPONENT;
                ngfx::cbv_desc_t m_desc = {};
            };

            struct sampler_t
            {
                D_GFX_OCS_COMPONENT;
                ngfx::sampler_desc_t m_desc;
            };

            // -------------------------------------------------------------------------------------------------------------------------------
            // -------------------------------------------------------------------------------------------------------------------------------
            
            ngfx::descriptor_t* Alloc(device_t* pDevice, resource_t* resource, descriptor_t* descriptor, texture_t* texture, const ngfx::srv_desc_t& desc)
            {
                srv_texture_t* srv_texture = AddComponent<resource_t, srv_texture_t>(pDevice, resource);
                srv_texture->m_texture     = texture;
                srv_texture->m_desc        = desc;
                return descriptor;
            }

            ngfx::descriptor_t* Alloc(device_t* pDevice, resource_t* resource, descriptor_t* descriptor, buffer_t* buffer, const ngfx::srv_desc_t& desc)
            {
                srv_buffer_t* srv_buffer = AddComponent<resource_t, srv_buffer_t>(pDevice, resource);
                srv_buffer->m_buffer     = buffer;
                srv_buffer->m_desc       = desc;
                return descriptor;
            }

            ngfx::descriptor_t* Alloc(device_t* pDevice, resource_t* resource, descriptor_t* descriptor, buffer_t* buffer, const uav_desc_t& desc)
            {
                uav_t* uav  = AddComponent<resource_t, uav_t>(pDevice, resource);
                uav->m_desc = desc;
                return descriptor;
            }

            ngfx::descriptor_t* Alloc(device_t* pDevice, resource_t* resource, descriptor_t* descriptor, buffer_t* buffer, const cbv_desc_t& desc)
            {
                cbv_t* cbv  = AddComponent<resource_t, cbv_t>(pDevice, resource);
                cbv->m_desc = desc;
                return descriptor;
            }

            ngfx::descriptor_t* Alloc(device_t* pDevice, resource_t* resource, descriptor_t* descriptor, const sampler_desc_t& desc)
            {
                sampler_t* sampler = AddComponent<resource_t, sampler_t>(pDevice, resource);
                sampler->m_desc    = desc;
                return descriptor;
            }

            void  Destroy(ngfx::descriptor_t* d) {}
            bool  Create(ngfx::descriptor_t* d) { return true; }
            void* GetHandle(ngfx::descriptor_t* d) { return nullptr; }
            u32   GetHeapIndex(ngfx::descriptor_t* d) { return 0; }

        }  // namespace nmock
    }  // namespace ngfx
}  // namespace ncore
