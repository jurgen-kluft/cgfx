#include "cgfx/d3d12/d3d12_shader.h"
#include "cgfx/d3d12/d3d12_device.h"
#include "xxHash/xxhash.h"

namespace ncore
{
    namespace ngfx
    {
        D3D12Shader::D3D12Shader(D3D12Device* pDevice, const GfxShaderDesc& desc, const char* name)
        {
            m_pDevice = pDevice;
            m_desc    = desc;
            m_name    = name;
        }

        bool D3D12Shader::Create(byte* data_ptr, u32 data_len)
        {
            m_data.resize(data.size());
            memcpy(m_data.data(), data.data(), data.size());

            m_hash = XXH3_64bits(data.data(), data.size());

            return true;
        }
    }  // namespace ngfx
}  // namespace ncore
