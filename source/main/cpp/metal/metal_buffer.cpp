#include "cgfx/metal/metal_buffer.h"
#include "cgfx/metal/metal_device.h"
#include "cgfx/metal/metal_heap.h"

namespace ncore
{
    namespace ngfx
    {

        MetalBuffer::MetalBuffer(MetalDevice* pDevice, const GfxBufferDesc& desc, const char* name)
        {
            m_pDevice = pDevice;
            m_desc    = desc;
            m_name    = name;
        }

        MetalBuffer::~MetalBuffer()
        {
            ((MetalDevice*)m_pDevice)->Evict(m_pBuffer);
            m_pBuffer->release();
        }

        bool MetalBuffer::Create()
        {
            if (m_desc.heap != nullptr)
            {
                ASSERT(m_desc.alloc_type == GfxAllocation::Placed);
                ASSERT(m_desc.memory_type == m_desc.heap->GetDesc().memory_type);
                ASSERT(m_desc.size + m_desc.heap_offset <= m_desc.heap->GetDesc().size);

                MTL::Heap* heap = (MTL::Heap*)m_desc.heap->GetHandle();
                m_pBuffer       = heap->newBuffer(m_desc.size, ToResourceOptions(m_desc.memory_type), m_desc.heap_offset);
            }
            else
            {
                MTL::Device* device = (MTL::Device*)m_pDevice->GetHandle();
                m_pBuffer           = device->newBuffer(m_desc.size, ToResourceOptions(m_desc.memory_type));
            }

            if (m_pBuffer == nullptr)
            {
                // RE_ERROR("[MetalBuffer] failed to create {}", m_name);
                return false;
            }

            ((MetalDevice*)m_pDevice)->MakeResident(m_pBuffer);

            // TODO
            // SetDebugLabel(m_pBuffer, m_name.c_str());

            if (m_desc.memory_type != GfxMemory::GpuOnly)
            {
                m_pCpuAddress = m_pBuffer->contents();
            }

            return true;
        }

        u64 MetalBuffer::GetGpuAddress() { return m_pBuffer->gpuAddress(); }

        u32 MetalBuffer::GetRequiredStagingBufferSize() const { return m_desc.size; }
    }  // namespace ngfx
}  // namespace ncore
