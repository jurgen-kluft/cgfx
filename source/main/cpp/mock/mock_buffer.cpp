#include "cgfx/mock/mock_buffer.h"
#include "cgfx/mock/mock_device.h"

namespace ncore
{
    namespace ngfx
    {
        MockBuffer::MockBuffer(MockDevice* pDevice, const GfxBufferDesc& desc, const eastl::string& name)
        {
            m_pDevice = pDevice;
            m_desc    = desc;
            m_name    = name;
        }

        MockBuffer::~MockBuffer()
        {
            if (m_pCpuAddress)
            {
                // TODO
                // RE_FREE(m_pCpuAddress);
            }
        }

        bool MockBuffer::Create()
        {
            if (m_desc.memory_type != GfxMemoryType::GpuOnly)
            {
                // TODO
                //  m_pCpuAddress = RE_ALLOC(m_desc.size);
                //  memset(m_pCpuAddress, 0, m_desc.size);
            }

            return true;
        }

        void* MockBuffer::GetHandle() const { return nullptr; }
        void* MockBuffer::GetCpuAddress() { return m_pCpuAddress; }
        u64   MockBuffer::GetGpuAddress() { return 0; }
        u32   MockBuffer::GetRequiredStagingBufferSize() const { return m_desc.size; }

    }  // namespace ngfx
}  // namespace ncore
