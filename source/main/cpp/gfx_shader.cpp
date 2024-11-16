#include "cgfx/gfx.h"
#include "cgfx/gfx_resource.h"

#include "cgfx/d3d12/d3d12_shader.h"
#include "cgfx/metal/metal_shader.h"
#include "cgfx/mock/mock_shader.h"

#include "ccore/c_math.h"
#include "cbase/c_hash.h"

namespace ncore
{
    namespace ngfx
    {
        shader_t* CreateShader(device_t* device, const shader_desc_t& desc, byte* data_ptr, u32 data_len, const char* name)
        {
            resource_t* resource = CreateObject<resource_t>(device, name);
            shader_t*   shader   = AddComponent<resource_t, shader_t>(device, resource);
            shader->m_desc       = desc;
            shader->m_hash       = nhash::datahash(data_ptr, data_len);

            switch (device->m_desc.backend)
            {
                case enums::Backend_D3D12: nd3d12::Create(device, shader, data_ptr, data_len); break;
                case enums::Backend_Metal: nmetal::Create(device, shader, data_ptr, data_len); break;
                case enums::Backend_Mock: nmock::Create(device, shader, data_ptr, data_len); break;
            }
            return shader;
        }

        void Destroy(device_t* device, shader_t* resource);
        bool Create(device_t* device, shader_t* shader, byte* data_ptr, u32 data_len);

    }  // namespace ngfx
}  // namespace ncore
