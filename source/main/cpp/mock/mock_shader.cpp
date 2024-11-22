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
            ngfx::shader_t* CreateShader(ngfx::device_t* device, ngfx::shader_t* shader)
            {
                nmock::shader_t* mshader = CreateComponent<ngfx::shader_t, nmock::shader_t>(device, shader);
                mshader->m_data          = nullptr;
                mshader->m_len           = 0;
                mshader->m_hash          = 0;
                return shader;
            }

            void Destroy(ngfx::device_t* device, ngfx::shader_t* shader)
            {
                // TODO, release the CPU memory of our shader
                nmock::shader_t* mshader = GetComponent<ngfx::shader_t, nmock::shader_t>(device, shader);
            }

            bool Create(ngfx::device_t* device, ngfx::shader_t* shader, byte* data_ptr, u32 data_len)
            {
                nmock::shader_t* mshader = GetComponent<ngfx::shader_t, nmock::shader_t>(device, shader);

                // TODO, allocate CPU memory for our shader and copy the data
                // m_data.resize(data.size());
                // nmem::memcpy(m_data.data(), data.data(), data.size());

                mshader->m_data = data_ptr;
                mshader->m_len  = data_len;
                mshader->m_hash = nhash::datahash(data_ptr, data_len);
                return true;
            }

            u64 GetHash(ngfx::device_t const* device, const ngfx::shader_t* shader)
            {
                const nmock::shader_t* mshader = GetComponent<ngfx::shader_t, nmock::shader_t>(device, shader);
                return mshader->m_hash;
            }

        }  // namespace nmock
    }  // namespace ngfx
}  // namespace ncore
