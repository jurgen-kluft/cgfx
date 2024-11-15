#ifndef __CGFX_MOCK_SHADER_H__
#define __CGFX_MOCK_SHADER_H__

#include "cgfx/gfx_shader.h"

namespace ncore
{
    namespace ngfx
    {
        namespace nmock
        {
            void  Destroy(ngfx::device_t* pDevice, ngfx::shader_t* pShader);
            bool  Create(ngfx::device_t* pDevice, ngfx::shader_t* pShader, byte* data_ptr, u32 data_len);
            void* GetHandle(ngfx::device_t* pDevice, const ngfx::shader_t* pShader);
        }  // namespace nmock
    }  // namespace ngfx
}  // namespace ncore

#endif
