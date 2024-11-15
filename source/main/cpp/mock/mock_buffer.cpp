#include "cgfx/mock/mock_buffer.h"
#include "cgfx/mock/mock_device.h"

namespace ncore
{
    namespace ngfx
    {
        namespace nmock
        {
            struct buffer_t
            {
                void* m_pCpuAddress = nullptr;
            };

            void Destroy(device_t* pDevice, ngfx::buffer_t* buffer)
            {
                nmock::buffer_t* mbuffer = GetOtherComponent<ngfx::buffer_t, nmock::buffer_t>(pDevice, buffer);
                if (mbuffer != nullptr && mbuffer->m_pCpuAddress)
                {
                    // TODO
                    // RE_FREE(m_pCpuAddress);
                }
            }

            bool Create(ngfx::device_t* pDevice, ngfx::buffer_t* buffer)
            {
                if (buffer->m_desc.memory_type != enums::MemoryGpuOnly)
                {
                    nmock::buffer_t* mbuffer = AddComponent<ngfx::buffer_t, nmock::buffer_t>(pDevice, buffer);
                    mbuffer->m_pCpuAddress  = nullptr;

                    // TODO
                    //  m_pCpuAddress = RE_ALLOC(m_desc.size);
                    //  memset(m_pCpuAddress, 0, m_desc.size);
                }

                return true;
            }

            void* GetHandle(device_t* pDevice, ngfx::buffer_t* buffer) { return nullptr; }
            void* GetCpuAddress(device_t* pDevice, ngfx::buffer_t* buffer)
            {
                nmock::buffer_t* mbuffer = GetOtherComponent<ngfx::buffer_t, nmock::buffer_t>(pDevice, buffer);
                return mbuffer->m_pCpuAddress;
            }
            u64 GetGpuAddress(device_t* pDevice, ngfx::buffer_t* buffer) { return 0; }
            u32 GetRequiredStagingBufferSize(device_t* pDevice, ngfx::buffer_t* buffer) { return buffer->m_desc.size; }

        }  // namespace nmock
    }  // namespace ngfx
}  // namespace ncore
