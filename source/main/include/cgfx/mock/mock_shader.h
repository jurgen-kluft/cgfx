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
            MockShader(MockDevice* pDevice, const GfxShaderDesc& desc, const char* name);

            virtual void* GetHandle() const override;
            virtual bool  Create(byte* data_ptr, u32 data_len) override;

        private:
            byte* m_data;
            u32   m_len;
        };

    }  // namespace ngfx
}  // namespace ncore

#endif
