#include "cgfx/mock/mock_shader.h"
#include "cgfx/mock/mock_device.h"
#include "cbase/c_memory.h"
#include "cbase/c_hash.h"

namespace ncore
{
    namespace ngfx
    {

        MockShader::MockShader(MockDevice* pDevice, const GfxShaderDesc& desc, const eastl::string& name)
        {
            m_pDevice = pDevice;
            m_desc    = desc;
            m_name    = name;
        }

        void* MockShader::GetHandle() const { return nullptr; }

        bool MockShader::Create(eastl::span<u8> data)
        {
            m_data.resize(data.size());
            nmem::memcpy(m_data.data(), data.data(), data.size());
            m_hash = nhash::datahash(data.data(), data.size());
            return true;
        }

    }  // namespace ngfx
}  // namespace ncore
