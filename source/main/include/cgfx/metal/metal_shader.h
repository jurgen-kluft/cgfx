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

        class MetalDevice;

        class MetalShader : public IGfxShader
        {
        public:
            MetalShader(MetalDevice* pDevice, const GfxShaderDesc& desc, const char* name);
            ~MetalShader();

            const MetalShaderReflection& GetReflection() const { return m_reflection; }

            virtual void* GetHandle() const override { return m_pFunction; }
            virtual bool  Create(byte* data_ptr, u32 data_len) override;

        private:
            MTL::Function*        m_pFunction = nullptr;
            MetalShaderReflection m_reflection;
        };

    }  // namespace ngfx
}  // namespace ncore

#endif
