#include "cgfx/d3d12/d3d12_shader.h"
#include "cgfx/d3d12/d3d12_device.h"
#include "cbase/c_hash.h"

namespace ncore
{
    namespace ngfx
    {
#ifdef TARGET_PC
        namespace nd3d12
        {
            void CreateShader(ngfx::device_t* device, ngfx::shader_t* shader) { nd3d12::shader_t* d3d12_shader = CreateComponent<ngfx::shader_t, nd3d12::shader_t>(device, shader); }
            void DestroyShader(ngfx::device_t* device, ngfx::shader_t* shader)
            {
                nd3d12::Destroy(device, shader);
                DestroyComponent<ngfx::shader_t, nd3d12::shader_t>(device, shader);
            }

            void Destroy(ngfx::device_t* device, ngfx::shader_t* shader)
            {
                // ...
            }

            bool Create(ngfx::device_t* device, ngfx::shader_t* shader, byte* data_ptr, u32 data_len)
            {
                nd3d12::shader_t* dxshader = GetComponent<ngfx::shader_t, nd3d12::shader_t>(device, shader);
                dxshader->m_data.mArray           = data_ptr;
                dxshader->m_data.mSize     = data_len;
                dxshader->m_data.mCapacity= data_len;
                shader->m_hash             = nhash::datahash(data_ptr, data_len);
                return true;
            }

            u64                   GetHash(ngfx::device_t const* device, const ngfx::shader_t* shader) { return shader->m_hash; }
            D3D12_SHADER_BYTECODE GetByteCode(ngfx::device_t const* device, const ngfx::shader_t* shader)
            {
                nd3d12::shader_t const*     dxshader = GetComponent<ngfx::shader_t, nd3d12::shader_t>(device, shader);
                D3D12_SHADER_BYTECODE byteCode = {};
                byteCode.pShaderBytecode       = dxshader->m_data.data();
                byteCode.BytecodeLength        = dxshader->m_data.size();
                return byteCode;
            }
        }  // namespace nd3d12
#endif
    }  // namespace ngfx
}  // namespace ncore
