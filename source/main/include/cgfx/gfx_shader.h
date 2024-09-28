#ifndef __CGFX_GFX_SHADER_H__
#define __CGFX_GFX_SHADER_H__

#include "cgfx/gfx_resource.h"

namespace ncore
{
    namespace ngfx
    {
        class IGfxShader : public IGfxResource
        {
        public:
            const GfxShaderDesc& GetDesc() const { return m_desc; }
            u64                  GetHash() const { return m_hash; }
            virtual bool         Create(eastl::span<u8> data) = 0;

        protected:
            GfxShaderDesc m_desc = {};
            u64           m_hash = 0;
        };
    }  // namespace ngfx
}  // namespace ncore

#endif
