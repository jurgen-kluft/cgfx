#ifndef __CGFX_MOCK_SHADER_H__
#define __CGFX_MOCK_SHADER_H__

#include "cgfx/gfx_shader.h"

namespace ncore
{
    namespace ngfx
    {
        namespace nmock
        {
            ngfx::shader_t* CreateShader(ngfx::device_t* device, ngfx::resource_t* resource, ngfx::shader_t* shader);
            void            Destroy(ngfx::device_t* device, ngfx::shader_t* shader);
            bool            Create(ngfx::device_t* device, ngfx::shader_t* shader, byte* data_ptr, u32 data_len);
            u64             GetHash(ngfx::device_t const* device, const ngfx::shader_t* shader);
        }  // namespace nmock
    }  // namespace ngfx
}  // namespace ncore

#endif
