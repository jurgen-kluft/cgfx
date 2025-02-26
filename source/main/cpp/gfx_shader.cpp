#include "cgfx/gfx.h"
#include "cgfx/gfx_name.h"

#include "cgfx/d3d12/d3d12_shader.h"
#include "cgfx/metal/metal_shader.h"
#include "cgfx/mock/mock_shader.h"

#include "ccore/c_math.h"
#include "cbase/c_hash.h"

namespace ncore
{
    namespace ngfx
    {
        shader_t* CreateShader(device_t* device, const shader_desc_t& desc, const char* name)
        {
            shader_t*   shader   = CreateInstance<shader_t>(device, name);
            shader->m_desc       = desc;

            switch (device->m_desc.backend)
            {
                case enums::Backend_D3D12: nd3d12::CreateShader(device,  shader); break;
                case enums::Backend_Metal: nmetal::CreateShader(device,  shader); break;
                case enums::Backend_Mock: nmock::CreateShader(device,  shader); break;
            }
            return shader;
        }

        bool Create(device_t* device, shader_t* shader, byte* data_ptr, u32 data_len)
        {
            switch (device->m_desc.backend)
            {
                case enums::Backend_D3D12: return nd3d12::Create(device, shader, data_ptr, data_len);
                case enums::Backend_Metal: return nmetal::Create(device, shader, data_ptr, data_len);
                case enums::Backend_Mock: return nmock::Create(device, shader, data_ptr, data_len);
            }
            return false;
        }

        void Destroy(device_t* device, shader_t* resource)
        {
            switch (device->m_desc.backend)
            {
                case enums::Backend_D3D12: nd3d12::Destroy(device, resource); break;
                case enums::Backend_Metal: nmetal::Destroy(device, resource); break;
                case enums::Backend_Mock: nmock::Destroy(device, resource); break;
            }
        }

    }  // namespace ngfx
}  // namespace ncore
