#ifndef __CGFX_METAL_SHADER_H__
#define __CGFX_METAL_SHADER_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
    #pragma once
#endif

#include "cgfx/metal/metal_utils.h"
#include "cgfx/gfx_shader.h"

namespace ncore
{
    namespace ngfx
    {
        namespace nmetal
        {
            struct shader_reflection_t
            {
                D_GFX_OCS_COMPONENT;
                uint32_t threadsPerThreadgroup[3];
            };

            struct mshader_t
            {
                D_GFX_OCS_COMPONENT_SET(enums::ComponentMetalShader);
                MTL::Function*      m_pFunction = nullptr;
                shader_reflection_t m_reflection;
                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            ngfx::shader_t*     CreateShader(ngfx::device_t* device, ngfx::resource_t* resource, ngfx::shader_t* shader);
            void                Destroy(ngfx::device_t* device, ngfx::shader_t* shader);
            bool                Create(ngfx::device_t* device, ngfx::shader_t* shader, byte* data_ptr, u32 data_len);
            u64                 GetHash(ngfx::device_t const* device, const ngfx::shader_t* shader);
            MTL::Function*      GetHandle(ngfx::device_t const* device, const ngfx::shader_t* shader);
            shader_reflection_t GetReflection(ngfx::device_t const* device, const ngfx::shader_t* shader);
        }  // namespace nmetal
    }  // namespace ngfx
}  // namespace ncore

#endif
