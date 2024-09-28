#ifndef __CGFX_D3D12_SHADER_H__
#define __CGFX_D3D12_SHADER_H__

#include "cgfx/d3d12/d3d12_header.h"
#include "cgfx/gfx_shader.h"

namespace ncore
{
    namespace ngfx
    {
        class D3D12Device;

        class D3D12Shader : public IGfxShader
        {
        public:
            D3D12Shader(D3D12Device* pDevice, const GfxShaderDesc& desc, const eastl::string& name);

            virtual void* GetHandle() const override { return nullptr; }
            virtual bool  Create(eastl::span<u8> data) override;

            D3D12_SHADER_BYTECODE GetByteCode() const { return {m_data.data(), m_data.size()}; }

        private:
            eastl::vector<u8> m_data;
        };
    }  // namespace ngfx
}  // namespace ncore
#endif
