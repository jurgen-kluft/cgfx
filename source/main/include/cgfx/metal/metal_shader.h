#ifndef __CGFX_METAL_SHADER_H__
#define __CGFX_METAL_SHADER_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
    #pragma once
#endif

#include "cgfx/metal/metal_utils.h"
#include "cgfx/metal/metal_shader_reflection.h"
#include "cgfx/gfx_shader.h"

namespace ncore
{
    namespace ngfx
    {
        namespace nmetal
        {
            ngfx::shader_t* CreateShader(ngfx::device_t* device, ngfx::resource_t* resource, ngfx::shader_t* shader);
            void            Destroy(ngfx::device_t* device, ngfx::shader_t* shader);
            bool            Create(ngfx::device_t* device, ngfx::shader_t* shader, byte* data_ptr, u32 data_len);
            u64             GetHash(ngfx::device_t const* device, const ngfx::shader_t* shader);
        }  // namespace nmetal

        // class MetalDevice;
        // class MetalShader : public shader_t
        // {
        // public:
        //     MetalShader(MetalDevice* pDevice, const shader_desc_t& desc, const char* name);
        //     ~MetalShader();

        //     const MetalShaderReflection& GetReflection() const { return m_reflection; }

        //     virtual void* GetHandle() const override { return m_pFunction; }
        //     virtual bool  Create(byte* data_ptr, u32 data_len) override;

        // private:
        //     MTL::Function*        m_pFunction = nullptr;
        //     MetalShaderReflection m_reflection;
        // };
    }  // namespace ngfx
}  // namespace ncore

#endif
