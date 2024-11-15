#include "cgfx/mock/mock_shader.h"
#include "cgfx/mock/mock_device.h"
#include "cbase/c_memory.h"
#include "cbase/c_hash.h"

namespace ncore
{
    namespace ngfx
    {
        namespace nmock
        {
            struct shader_t
            {
                byte* m_data;
                u32   m_len;
                u32   m_hash;
            };

            void Destroy(ngfx::device_t* pDevice, ngfx::shader_t* pShader)
            {
                // TODO, release the CPU memory of our shader
                nmock::shader_t* mshader = GetOtherComponent<ngfx::shader_t, nmock::shader_t>(pDevice, pShader);
            }

            bool Create(ngfx::device_t* pDevice, ngfx::shader_t* pShader, byte* data_ptr, u32 data_len)
            {
                nmock::shader_t* mshader = GetOtherComponent<ngfx::shader_t, nmock::shader_t>(pDevice, pShader);

                // TODO, allocate CPU memory for our shader and copy the data
                // m_data.resize(data.size());
                // nmem::memcpy(m_data.data(), data.data(), data.size());

                mshader->m_data = data_ptr;
                mshader->m_len  = data_len;
                mshader->m_hash = nhash::datahash(data_ptr, data_len);
                return true;
            }

            void* GetHandle(ngfx::device_t* pDevice, const ngfx::shader_t* pShader) { return nullptr; }

        }  // namespace nmock
    }  // namespace ngfx
}  // namespace ncore
