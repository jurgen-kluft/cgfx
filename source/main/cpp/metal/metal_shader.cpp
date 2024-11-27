#include "cgfx/metal/metal_shader.h"
#include "cgfx/metal/metal_device.h"
#include "cgfx/metal/metal_utils.h"
#include "cgfx/metal/metal_shader_reflection.h"

#include "cbase/c_memory.h"
#include "cbase/c_hash.h"

#include "cmacos/Metal/Metal.hpp"
#include "cmacos/MetalShaderConverter/IRConverterRuntime/ir_converter_runtime.h"

namespace ncore
{
    namespace ngfx
    {
        namespace nmetal
        {
            void CreateShader(ngfx::device_t* device, ngfx::shader_t* shader)
            {
                nmetal::mshader_t* msc                     = CreateComponent<ngfx::shader_t, nmetal::mshader_t>(device, shader);
                msc->m_reflection.threadsPerThreadgroup[0] = 1;
                msc->m_reflection.threadsPerThreadgroup[1] = 1;
                msc->m_reflection.threadsPerThreadgroup[2] = 1;
            }

            void Destroy(ngfx::device_t* device, ngfx::shader_t* shader)
            {
                nmetal::mshader_t* ms = GetComponent<ngfx::shader_t, nmetal::mshader_t>(device, shader);
                ms->m_pFunction->release();
            }

            bool Create(ngfx::device_t* device, ngfx::shader_t* shader, byte* data_ptr, u32 data_len)
            {
                nmetal::mshader_t* ms = GetComponent<ngfx::shader_t, nmetal::mshader_t>(device, shader);
                nmetal::device_t*  md = GetComponent<ngfx::device_t, nmetal::device_t>(device, device);

                ms->m_pFunction->release();

                nmem::memcpy(&ms->m_reflection, data_ptr, sizeof(shader_reflection_t));
                dispatch_data_t metalIR = dispatch_data_create(data_ptr + sizeof(shader_reflection_t), data_len - sizeof(shader_reflection_t), dispatch_get_main_queue(), NULL);

                NS::Error*    error;
                MTL::Library* library = md->m_pDevice->newLibrary(metalIR, &error);
                CFRelease(metalIR);

                if (library == nullptr)
                {
                    // RE_ERROR("[MetalShader] failed to create {} \r\n{}", m_name, error->localizedDescription()->utf8String());

                    error->release();
                    return false;
                }

                NS::String* functionName = NS::String::alloc()->init(shader->m_desc.entry_point, NS::StringEncoding::UTF8StringEncoding);
                ms->m_pFunction          = library->newFunction(functionName);
                library->release();
                functionName->release();

                name_t const* name = GetComponent<ngfx::shader_t, name_t>(device, shader);
                SetDebugLabel(ms->m_pFunction, name->m_name);

                shader->m_hash = nhash::datahash(data_ptr, data_len);

                return true;
            }

            u64 GetHash(ngfx::device_t const* device, const ngfx::shader_t* shader) { return shader->m_hash; }

            MTL::Function* GetHandle(ngfx::device_t const* device, const ngfx::shader_t* shader)
            {
                nmetal::mshader_t const* ms = GetComponent<ngfx::shader_t, nmetal::mshader_t>(device, shader);
                return ms->m_pFunction;
            }

            shader_reflection_t GetReflection(ngfx::device_t const* device, const ngfx::shader_t* shader)
            {
                nmetal::mshader_t const* ms = GetComponent<ngfx::shader_t, nmetal::mshader_t>(device, shader);
                return ms->m_reflection;
            }

        }  // namespace nmetal
    }  // namespace ngfx
}  // namespace ncore
