#include "cgfx/mock/mock_shader.h"
#include "cgfx/mock/mock_device.h"
#include "cbase/c_memory.h"
#include "cbase/c_hash.h"

namespace ncore
{
    namespace ngfx
    {
        MockShader::MockShader(MockDevice* pDevice, const GfxShaderDesc& desc, const char* name)
        {
            m_pDevice = pDevice;
            m_desc    = desc;
            m_name    = name;
        }

        void* MockShader::GetHandle() const { return nullptr; }

        bool MockShader::Create(byte* data_ptr, u32 data_len)
        {
            // m_data.resize(data.size());
            // nmem::memcpy(m_data.data(), data.data(), data.size());
            m_data = data_ptr;
            m_len  = data_len;
            m_hash = nhash::datahash(data_ptr, data_len);
            return true;
        }

    }  // namespace ngfx
}  // namespace ncore
