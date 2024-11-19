#include "cgfx/metal/metal_buffer.h"
#include "cgfx/metal/metal_device.h"
#include "cgfx/metal/metal_heap.h"
#include "cgfx/metal/metal_utils.h"

namespace ncore
{
    namespace ngfx
    {
        namespace nmetal
        {
            ngfx::buffer_t* CreateBuffer(ngfx::device_t* device, ngfx::buffer_t* buffer)
            {
                nmetal::mbuffer_t* mbuffer = CreateComponent<ngfx::buffer_t, nmetal::mbuffer_t>(device, buffer);
                // ..
                return buffer;
            }

            void Destroy(ngfx::device_t* device, ngfx::buffer_t* buffer)
            {
                nmetal::mbuffer_t* mbuffer = GetComponent<ngfx::buffer_t, nmetal::mbuffer_t>(device, buffer);
                if (mbuffer != nullptr && mbuffer->m_pCpuAddress)
                {
                    nmetal::Evict(device, mbuffer->m_pBuffer);
                    mbuffer->m_pBuffer->release();
                }
            }

            bool Create(ngfx::device_t* device, ngfx::buffer_t* buffer)
            {
                nmetal::mbuffer_t* mbuffer = GetComponent<ngfx::buffer_t, nmetal::mbuffer_t>(device, buffer);

                // TODO We should introduce a component that holds heap and heap allocation information, this should not be part of the buffer
                //      descriptor. This will allow us to remove the heap_offset and heap members from the buffer descriptor.

                if (buffer->m_desc.heap != nullptr)
                {
                    ASSERT(buffer->m_desc.alloc_type == enums::AllocationPlaced);
                    ASSERT(buffer->m_desc.memory_type == buffer->m_desc.heap->GetDesc().memory_type);
                    ASSERT(buffer->m_desc.size + buffer->m_desc.heap_offset <= buffer->m_desc.heap->GetDesc().size);

                    MTL::Heap* heap    = ngfx::nmetal::GetHandle(device, buffer->m_desc.heap);
                    mbuffer->m_pBuffer = heap->newBuffer(buffer->m_desc.size, ToResourceOptions(buffer->m_desc.memory_type), buffer->m_desc.heap_offset);
                }
                else
                {
                    nmetal::device_t* mdevice = GetComponent<ngfx::device_t, nmetal::device_t>(device, device);
                    mbuffer->m_pBuffer        = mdevice->m_pDevice->newBuffer(buffer->m_desc.size, ToResourceOptions(buffer->m_desc.memory_type));
                }

                if (mbuffer->m_pBuffer == nullptr)
                {
                    // RE_ERROR("[MetalBuffer] failed to create {}", m_name);
                    return false;
                }

                MakeResident(device, mbuffer->m_pBuffer);

                name_t const* name = GetComponent<ngfx::buffer_t, name_t>(device, buffer);
                SetDebugLabel(mbuffer->m_pBuffer, name->m_name);

                if (buffer->m_desc.memory_type != enums::MemoryGpuOnly)
                {
                    mbuffer->m_pCpuAddress = mbuffer->m_pBuffer->contents();
                }

                return true;
            }

            MTL::Buffer* GetHandle(ngfx::device_t* device, ngfx::buffer_t* buffer)
            {
                nmetal::mbuffer_t* mbuffer = GetComponent<ngfx::buffer_t, nmetal::mbuffer_t>(device, buffer);
                return mbuffer->m_pBuffer;
            }

            void* GetCpuAddress(ngfx::device_t* device, ngfx::buffer_t* buffer)
            {
                nmetal::mbuffer_t* mbuffer = GetComponent<ngfx::buffer_t, nmetal::mbuffer_t>(device, buffer);
                return mbuffer->m_pCpuAddress;
            }

            u64 GetGpuAddress(ngfx::device_t* device, ngfx::buffer_t* buffer)
            {
                nmetal::mbuffer_t* mbuffer = GetComponent<ngfx::buffer_t, nmetal::mbuffer_t>(device, buffer);
                return mbuffer->m_pBuffer->gpuAddress();
            }

            u32 GetRequiredStagingBufferSize(ngfx::device_t* device, ngfx::buffer_t* buffer) { return buffer->m_desc.size; }
        }  // namespace nmetal

    }  // namespace ngfx
}  // namespace ncore
