#include "cgfx/metal/metal_heap.h"
#include "cgfx/metal/metal_device.h"

namespace ncore
{
    namespace ngfx
    {
        namespace nmetal
        {
            heap_t* CreateHeap(ngfx::device_t* pDevice, resource_t* resource, heap_t* heap)
            {
                mheap_t* mheap = AddAnotherComponent<ngfx::heap_t, mheap_t>(pDevice, heap);
                mheap->m_pHeap = nullptr;
                return heap;
            }

            bool Create(ngfx::device_t* pDevice, heap_t* pHeap)
            {
                ASSERT(m_desc.size % (64 * 1024) == 0);

                nmetal::device_t* mdevice   = GetOtherComponent<ngfx::device_t, nmetal::device_t>(pDevice, pDevice);
                MTL::Device*      mtlDevice = mdevice->m_pDevice;
                ;

                MTL::HeapDescriptor* descriptor = MTL::HeapDescriptor::alloc()->init();
                descriptor->setSize(pHeap->m_desc.size);
                descriptor->setResourceOptions(ToResourceOptions(pHeap->m_desc.memory_type));
                descriptor->setType(MTL::HeapTypePlacement);  // TODO : MTL::HeapTypeSparse for sparse textures

                nmetal::mheap_t* mheap = GetOtherComponent<ngfx::heap_t, nmetal::mheap_t>(pDevice, pHeap);
                mheap->m_pHeap         = mtlDevice->newHeap(descriptor);
                descriptor->release();

                if (mheap->m_pHeap == nullptr)
                {
                    // RE_ERROR("[MetalHeap] failed to create {}", m_name);
                    return false;
                }

                MakeResident(pDevice, mheap->m_pHeap);

                name_t const* name = GetOtherComponent<ngfx::heap_t, name_t>(pDevice, pHeap);
                SetDebugLabel(mheap->m_pHeap, name->m_name);

                return true;
            }

            void Destroy(ngfx::device_t* pDevice, heap_t* pHeap)
            {
                nmetal::mheap_t* mheap = GetOtherComponent<ngfx::heap_t, nmetal::mheap_t>(pDevice, pHeap);
                if (mheap != nullptr)
                {
                    nmetal::Evict(pDevice, mheap->m_pHeap);
                    mheap->m_pHeap->release();
                }
            }

            void* GetHandle(ngfx::device_t* pDevice, const heap_t* pHeap)
            {
                nmetal::mheap_t* mheap = GetOtherComponent<ngfx::heap_t, nmetal::mheap_t>(pDevice, pHeap);
                return mheap->m_pHeap;
            }
            
        }  // namespace nmetal

    }  // namespace ngfx
}  // namespace ncore
