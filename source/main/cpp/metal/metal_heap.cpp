#include "cgfx/metal/metal_heap.h"
#include "cgfx/metal/metal_device.h"

namespace ncore
{
    namespace ngfx
    {
        MetalHeap::MetalHeap(MetalDevice* pDevice, const heap_desc_t& desc, const char* name)
        {
            m_pDevice = pDevice;
            m_desc    = desc;
            m_name    = name;
        }

        MetalHeap::~MetalHeap()
        {
            ((MetalDevice*)m_pDevice)->Evict(m_pHeap);

            m_pHeap->release();
        }

        bool MetalHeap::Create()
        {
            ASSERT(m_desc.size % (64 * 1024) == 0);

            MTL::Device* device = (MTL::Device*)m_pDevice->GetHandle();

            MTL::HeapDescriptor* descriptor = MTL::HeapDescriptor::alloc()->init();
            descriptor->setSize(m_desc.size);
            descriptor->setResourceOptions(ToResourceOptions(m_desc.memory_type));
            descriptor->setType(MTL::HeapTypePlacement);  // TODO : MTL::HeapTypeSparse for sparse textures

            m_pHeap = device->newHeap(descriptor);
            descriptor->release();

            if (m_pHeap == nullptr)
            {
                // RE_ERROR("[MetalHeap] failed to create {}", m_name);
                return false;
            }

            ((MetalDevice*)m_pDevice)->MakeResident(m_pHeap);

            SetDebugLabel(m_pHeap, m_name);

            return true;
        }

    }  // namespace ngfx
}  // namespace ncore
