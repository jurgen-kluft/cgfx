#ifndef __CGFX_GFX_DESCRIPTOR_H__
#define __CGFX_GFX_DESCRIPTOR_H__

#include "cgfx/gfx_resource.h"

namespace ncore
{
    namespace ngfx
    {
        struct descriptor_t
        {
            D_GFX_OCS_COMPONENT;
            enums::descriptor_type m_type;
            DCORE_CLASS_PLACEMENT_NEW_DELETE
        };

        descriptor_t* CreateShaderResourceView(device_t* device, texture_t* resource, const srv_desc_t& desc, const char* name);
        descriptor_t* CreateShaderResourceView(device_t* device, buffer_t* resource, const srv_desc_t& desc, const char* name);
        descriptor_t* CreateUnorderedAccessView(device_t* device, texture_t* resource, const uav_desc_t& desc, const char* name);
        descriptor_t* CreateUnorderedAccessView(device_t* device, buffer_t* resource, const uav_desc_t& desc, const char* name);
        descriptor_t* CreateConstantBufferView(device_t* device, buffer_t* buffer, const cbv_desc_t& desc, const char* name);
        descriptor_t* CreateSampler(device_t* device, const sampler_desc_t& desc, const char* name);
        void          Destroy(device_t* device, descriptor_t* resource);
        u32           GetHeapIndex(descriptor_t*);

    }  // namespace ngfx
}  // namespace ncore

#endif
