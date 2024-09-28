#include "cgfx/d3d12/d3d12_shader.h"
#include "cgfx/d3d12/d3d12_device.h"
#include "xxHash/xxhash.h"

namespace ncore
{
    namespace ngfx
    {
        D3D12Shader::D3D12Shader(D3D12Device* pDevice, const GfxShaderDesc& desc, const eastl::string& name)
        {
            m_pDevice = pDevice;
            m_desc    = desc;
            m_name    = name;
        }

        bool D3D12Shader::Create(eastl::span<u8> data)
        {
            m_data.resize(data.size());
            memcpy(m_data.data(), data.data(), data.size());

            m_hash = XXH3_64bits(data.data(), data.size());

            return true;
        }
    }  // namespace ngfx
}  // namespace ncore
