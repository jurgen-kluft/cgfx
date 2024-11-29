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
#ifdef TARGET_MAC
        namespace nmetal
        {
    #pragma pack(push, 1)
            struct shader_reflection_t
            {
                uint32_t threadsPerThreadgroup[3];
            };
    #pragma pack(pop)

            struct mshader_t
            {
                D_GFX_CS_COMPONENT_SET(enums::ComponentMetalShader);
                MTL::Function*      m_pFunction = nullptr;
                shader_reflection_t m_reflection;
                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            void                CreateShader(ngfx::device_t* device, ngfx::shader_t* shader);
            void                Destroy(ngfx::device_t* device, ngfx::shader_t* shader);
            bool                Create(ngfx::device_t* device, ngfx::shader_t* shader, byte* data_ptr, u32 data_len);
            u64                 GetHash(ngfx::device_t const* device, const ngfx::shader_t* shader);
            MTL::Function*      GetHandle(ngfx::device_t const* device, const ngfx::shader_t* shader);
            shader_reflection_t GetReflection(ngfx::device_t const* device, const ngfx::shader_t* shader);
        }  // namespace nmetal
#else
        namespace nmetal
        {
            inline void CreateShader(ngfx::device_t* device, ngfx::shader_t* shader) {}
            inline void Destroy(ngfx::device_t* device, ngfx::shader_t* shader) {}
            inline bool Create(ngfx::device_t* device, ngfx::shader_t* shader, byte* data_ptr, u32 data_len) { return false; }
            inline u64  GetHash(ngfx::device_t const* device, const ngfx::shader_t* shader) { return 0; }
        }  // namespace nmetal
#endif
    }  // namespace ngfx
}  // namespace ncore

#endif
