#include "cgfx/metal/metal_buffer.h"
#include "cgfx/metal/metal_device.h"
#include "cgfx/metal/metal_heap.h"

namespace ncore
{
    namespace ngfx
    {
        // MetalBuffer::MetalBuffer(MetalDevice* device, const buffer_desc_t& desc, const char* name)
        // {
        //     m_pDevice = device;
        //     m_desc    = desc;
        //     m_name    = name;
        // }

        // MetalBuffer::~MetalBuffer()
        // {
        //     ((MetalDevice*)m_pDevice)->Evict(m_pBuffer);
        //     m_pBuffer->release();
        // }

        // bool MetalBuffer::Create()
        // {
        //     if (m_desc.heap != nullptr)
        //     {
        //         ASSERT(m_desc.alloc_type == GfxAllocation::Placed);
        //         ASSERT(m_desc.memory_type == m_desc.heap->GetDesc().memory_type);
        //         ASSERT(m_desc.size + m_desc.heap_offset <= m_desc.heap->GetDesc().size);

        //         MTL::Heap* heap = (MTL::Heap*)m_desc.heap->GetHandle();
        //         m_pBuffer       = heap->newBuffer(m_desc.size, ToResourceOptions(m_desc.memory_type), m_desc.heap_offset);
        //     }
        //     else
        //     {
        //         MTL::Device* device = (MTL::Device*)m_pDevice->GetHandle();
        //         m_pBuffer           = device->newBuffer(m_desc.size, ToResourceOptions(m_desc.memory_type));
        //     }

        //     if (m_pBuffer == nullptr)
        //     {
        //         // RE_ERROR("[MetalBuffer] failed to create {}", m_name);
        //         return false;
        //     }

        //     ((MetalDevice*)m_pDevice)->MakeResident(m_pBuffer);

        //     // TODO
        //     // SetDebugLabel(m_pBuffer, m_name.c_str());

        //     if (m_desc.memory_type != GfxMemory::GpuOnly)
        //     {
        //         m_pCpuAddress = m_pBuffer->contents();
        //     }

        //     return true;
        // }

        namespace nmetal
        {
            struct mbuffer_t
            {
                D_GFX_OCS_COMPONENT;
                MTL::Buffer* m_pBuffer     = nullptr;
                void*        m_pCpuAddress = nullptr;
            };

            ngfx::buffer_t* CreateBuffer(ngfx::device_t* device, ngfx::buffer_t* buffer)
            {
                nmetal::mbuffer_t* mbuffer = AddAnotherComponent<ngfx::buffer_t, nmetal::mbuffer_t>(device, buffer);
                // ..
                return buffer;
            }

            bool Create(ngfx::device_t* device, ngfx::buffer_t* buffer)
            {
                nmetal::mbuffer_t* mbuffer = GetOtherComponent<ngfx::buffer_t, nmetal::mbuffer_t>(device, buffer);

                // TODO We should introduce a component that holds heap and heap allocation information, this should not be part of the buffer
                //      descriptor. This will allow us to remove the heap_offset and heap members from the buffer descriptor.

                if (buffer->m_desc.heap != nullptr)
                {
                    ASSERT(buffer->m_desc.alloc_type == enums::AllocationPlaced);
                    ASSERT(buffer->m_desc.memory_type == buffer->m_desc.heap->GetDesc().memory_type);
                    ASSERT(buffer->m_desc.size + buffer->m_desc.heap_offset <= buffer->m_desc.heap->GetDesc().size);

                    MTL::Heap* heap    = (MTL::Heap*)GetHandle(device, buffer->m_desc.heap);
                    mbuffer->m_pBuffer = heap->newBuffer(buffer->m_desc.size, ToResourceOptions(buffer->m_desc.memory_type), buffer->m_desc.heap_offset);
                }
                else
                {
                    nmetal::device_t* mdevice = GetOtherComponent<ngfx::device_t, nmetal::device_t>(device, device);
                    mbuffer->m_pBuffer        = mdevice->m_pDevice->newBuffer(buffer->m_desc.size, ToResourceOptions(buffer->m_desc.memory_type));
                }

                if (mbuffer->m_pBuffer == nullptr)
                {
                    // RE_ERROR("[MetalBuffer] failed to create {}", m_name);
                    return false;
                }

                MakeResident(device, mbuffer->m_pBuffer);

                name_t const* name = GetOtherComponent<ngfx::buffer_t, name_t>(device, buffer);
                SetDebugLabel(mbuffer->m_pBuffer, name->m_name);

                if (buffer->m_desc.memory_type != enums::MemoryGpuOnly)
                {
                    mbuffer->m_pCpuAddress = mbuffer->m_pBuffer->contents();
                }

                return true;
            }

            void Destroy(ngfx::device_t* device, ngfx::buffer_t* buffer)
            {
                nmetal::mbuffer_t* mbuffer = GetOtherComponent<ngfx::buffer_t, nmetal::mbuffer_t>(device, buffer);
                if (mbuffer != nullptr && mbuffer->m_pCpuAddress)
                {
                    nmetal::Evict(device, mbuffer->m_pBuffer);
                    mbuffer->m_pBuffer->release();
                }
            }

            void* GetHandle(ngfx::device_t* device, ngfx::buffer_t* buffer)
            {
                nmetal::mbuffer_t* mbuffer = GetOtherComponent<ngfx::buffer_t, nmetal::mbuffer_t>(device, buffer);
                return mbuffer->m_pBuffer;
            }

            void* GetCpuAddress(ngfx::device_t* device, ngfx::buffer_t* buffer)
            {
                nmetal::mbuffer_t* mbuffer = GetOtherComponent<ngfx::buffer_t, nmetal::mbuffer_t>(device, buffer);
                return mbuffer->m_pCpuAddress;
            }

            u64 GetGpuAddress(ngfx::device_t* device, ngfx::buffer_t* buffer)
            {
                nmetal::mbuffer_t* mbuffer = GetOtherComponent<ngfx::buffer_t, nmetal::mbuffer_t>(device, buffer);
                return mbuffer->m_pBuffer->gpuAddress();
            }

            u32 GetRequiredStagingBufferSize(ngfx::device_t* device, ngfx::buffer_t* buffer) { return buffer->m_desc.size; }
        }  // namespace nmetal

    }  // namespace ngfx
}  // namespace ncore
