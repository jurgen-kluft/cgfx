#ifndef __CGFX_D3D12_SHADER_H__
#define __CGFX_D3D12_SHADER_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
    #pragma once
#endif

#include "cbase/c_carray.h"
#include "cgfx/gfx_shader.h"
#include "cgfx/d3d12/d3d12_header.h"

namespace ncore
{
    namespace ngfx
    {
        namespace nd3d12
        {
            struct shader_t
            {
                D_GFX_OCS_COMPONENT_SET(enums::ComponentD3D12Shader);
                carray_t<u8> m_data;
                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            ngfx::shader_t*       CreateShader(ngfx::device_t* device, ngfx::resource_t* resource, ngfx::shader_t* shader);
            void                  Destroy(ngfx::device_t* device, ngfx::shader_t* shader);
            bool                  Create(ngfx::device_t* device, ngfx::shader_t* shader, byte* data_ptr, u32 data_len);
            u64                   GetHash(ngfx::device_t const* device, const ngfx::shader_t* shader);
            D3D12_SHADER_BYTECODE GetByteCode(ngfx::device_t const* device, const ngfx::shader_t* shader);
        }  // namespace nd3d12
    }  // namespace ngfx
}  // namespace ncore
#endif
