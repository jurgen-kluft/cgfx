#ifndef __CGFX_GFX_DESCRIPTOR_H__
#define __CGFX_GFX_DESCRIPTOR_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
    #pragma once
#endif

#include "cgfx/gfx_name.h"

namespace ncore
{
    namespace ngfx
    {
        struct descriptor_t
        {
            D_GFX_CS_COMPONENT_SET(enums::ComponentDescriptor);
            enums::descriptor_type m_type;
            DCORE_CLASS_PLACEMENT_NEW_DELETE
        };

        descriptor_t* CreateShaderResourceView(device_t* device, texture_t* texture, const srv_desc_t& desc, const char* name);
        descriptor_t* CreateShaderResourceView(device_t* device, buffer_t* buffer, const srv_desc_t& desc, const char* name);
        descriptor_t* CreateShaderResourceView(device_t* device, tlas_t* tlas, const srv_desc_t& desc, const char* name);
        descriptor_t* CreateUnorderedAccessView(device_t* device, texture_t* texture, const uav_desc_t& desc, const char* name);
        descriptor_t* CreateUnorderedAccessView(device_t* device, buffer_t* buffer, const uav_desc_t& desc, const char* name);
        descriptor_t* CreateConstantBufferView(device_t* device, buffer_t* buffer, const cbv_desc_t& desc, const char* name);
        descriptor_t* CreateSampler(device_t* device, const sampler_desc_t& desc, const char* name);
        void          Destroy(device_t* device, descriptor_t* descriptor);
        u32           GetHeapIndex(device_t* device, descriptor_t* descriptor);

    }  // namespace ngfx
}  // namespace ncore

#endif
