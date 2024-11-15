#ifndef __CGFX_GFX_SHADER_H__
#define __CGFX_GFX_SHADER_H__

#include "cgfx/gfx_resource.h"

namespace ncore
{
    namespace ngfx
    {
        struct shader_t
        {
            D_GFX_OCS_COMPONENT;
            u64           m_hash = 0;
            shader_desc_t m_desc = {};
        };

        shader_t* CreateShader(device_t* device, const shader_desc_t& desc, byte* data_ptr, u32 data_len, const char* name);
        void      Destroy(device_t* device, shader_t* resource);
        bool      Create(device_t* device, shader_t* shader, byte* data_ptr, u32 data_len);

    }  // namespace ngfx
}  // namespace ncore

#endif
