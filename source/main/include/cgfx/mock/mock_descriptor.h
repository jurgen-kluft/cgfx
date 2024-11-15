#ifndef __CGFX_MOCK_DESCRIPTOR_H__
#define __CGFX_MOCK_DESCRIPTOR_H__

#include "cgfx/gfx_descriptor.h"

namespace ncore
{
    namespace ngfx
    {
        namespace nmock
        {
            ngfx::descriptor_t* Alloc(device_t* pDevice, resource_t* resource, descriptor_t* descriptor, texture_t* texture, const ngfx::srv_desc_t& desc);
            ngfx::descriptor_t* Alloc(device_t* pDevice, resource_t* resource, descriptor_t* descriptor, buffer_t* buffer, const ngfx::srv_desc_t& desc);
            ngfx::descriptor_t* Alloc(device_t* pDevice, resource_t* resource, descriptor_t* descriptor, buffer_t* buffer, const uav_desc_t& desc);
            ngfx::descriptor_t* Alloc(device_t* pDevice, resource_t* resource, descriptor_t* descriptor, buffer_t* buffer, const cbv_desc_t& desc);
            ngfx::descriptor_t* Alloc(device_t* pDevice, resource_t* resource, descriptor_t* descriptor, const sampler_desc_t& desc);

            void  Destroy(ngfx::descriptor_t* d);
            bool  Create(ngfx::descriptor_t* d);
            void* GetHandle(ngfx::descriptor_t* d);
            u32   GetHeapIndex(ngfx::descriptor_t* d);

        }  // namespace ngfx
    }  // namespace ncore
}

#endif
