#ifndef __CGFX_GFX_SHADER_H__
#define __CGFX_GFX_SHADER_H__

#include "cgfx/gfx_resource.h"

namespace ncore
{
    namespace ngfx
    {
        struct shader_t
        {
            D_GFX_CS_COMPONENT_SET(enums::ComponentShader);
            u64           m_hash;
            shader_desc_t m_desc = {};
            DCORE_CLASS_PLACEMENT_NEW_DELETE
        };

        shader_t* CreateShader(device_t* device, const shader_desc_t& desc, const char* name);
        bool      Create(device_t* device, shader_t* shader, byte* data_ptr, u32 data_len);
        void      Destroy(device_t* device, shader_t* resource);
        u64       GetHash(device_t const* device, const shader_t* shader);
    }  // namespace ngfx
}  // namespace ncore

#endif
