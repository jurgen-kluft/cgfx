#include "cgfx/metal/metal_shader.h"
#include "cgfx/metal/metal_device.h"
#include "cbase/c_memory.h"
#include "cbase/c_hash.h"

#include "cmacos/Metal/Metal.hpp"
#include "cmacos/MetalShaderConverter/IRConverterRuntime/ir_converter_runtime.h"

namespace ncore
{
    namespace ngfx
    {
        MetalShader::MetalShader(MetalDevice* pDevice, const GfxShaderDesc& desc, const char* name)
        {
            m_pDevice = pDevice;
            m_desc    = desc;
            m_name    = name;
        }

        MetalShader::~MetalShader() { m_pFunction->release(); }

        bool MetalShader::Create(byte* data_ptr, u32 data_len)
        {
            m_pFunction->release();

            MTL::Device* device = (MTL::Device*)m_pDevice->GetHandle();

            nmem::memcpy(&m_reflection, data_ptr, sizeof(MetalShaderReflection));
            dispatch_data_t metalIR = dispatch_data_create(data_ptr + sizeof(MetalShaderReflection), data_len - sizeof(MetalShaderReflection), dispatch_get_main_queue(), NULL);

            NS::Error*    error;
            MTL::Library* library = device->newLibrary(metalIR, &error);
            CFRelease(metalIR);

            if (library == nullptr)
            {
                // RE_ERROR("[MetalShader] failed to create {} \r\n{}", m_name, error->localizedDescription()->utf8String());

                error->release();
                return false;
            }

            NS::String* functionName = NS::String::alloc()->init(m_desc.entry_point, NS::StringEncoding::UTF8StringEncoding);
            m_pFunction              = library->newFunction(functionName);
            library->release();
            functionName->release();

            SetDebugLabel(m_pFunction, m_name);

            m_hash = nhash::datahash(data_ptr, data_len);

            return true;
        }

    }  // namespace ngfx
}  // namespace ncore
