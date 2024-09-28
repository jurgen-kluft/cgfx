#ifndef __CGFX_MOCK_SHADER_H__
#define __CGFX_MOCK_SHADER_H__

#include "cgfx/gfx_shader.h"

namespace ncore
{
    namespace ngfx
    {
        class MockDevice;

        class MockShader : public IGfxShader
        {
        public:
            MockShader(MockDevice* pDevice, const GfxShaderDesc& desc, const eastl::string& name);

            virtual void* GetHandle() const override;
            virtual bool  Create(eastl::span<u8> data) override;

        private:
            eastl::span<u8> m_data;
        };

    }  // namespace ngfx
}  // namespace ncore

#endif
