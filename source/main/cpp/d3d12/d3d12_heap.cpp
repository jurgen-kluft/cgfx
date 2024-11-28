#include "cgfx/d3d12/d3d12_heap.h"
#include "cgfx/d3d12/d3d12_device.h"

#ifdef TARGET_PC

    #include "cd3d12/ma/D3D12MemAlloc.h"

namespace ncore
{
    namespace ngfx
    {
        namespace nd3d12
        {
            void CreateHeap(ngfx::device_t* device, ngfx::heap_t* heap) { nd3d12::heap_t* pHeap = CreateComponent<ngfx::heap_t, nd3d12::heap_t>(device, heap); }
            void DestroyHeap(ngfx::device_t* device, ngfx::heap_t* heap) { DestroyComponent<ngfx::heap_t, nd3d12::heap_t>(device, heap); }

            bool Create(ngfx::device_t* device, ngfx::heap_t* heap)
            {
                ASSERT(heap->m_desc.size % (64 * 1024) == 0);

                nd3d12::device_t* pDevice = GetComponent<ngfx::device_t, nd3d12::device_t>(device, device);
                nd3d12::heap_t*   pHeap   = GetComponent<ngfx::heap_t, nd3d12::heap_t>(device, heap);

                D3D12MA::ALLOCATION_DESC allocationDesc = {};
                allocationDesc.HeapType                 = d3d12_heap_type(heap->m_desc.memory_type);
                allocationDesc.Flags                    = D3D12MA::ALLOCATION_FLAG_COMMITTED;

                D3D12_RESOURCE_ALLOCATION_INFO allocationInfo = {};
                allocationInfo.SizeInBytes                    = heap->m_desc.size;
                allocationInfo.Alignment                      = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;

                D3D12MA::Allocator* pAllocator = pDevice->m_pResourceAllocator;
                HRESULT             hr         = pAllocator->AllocateMemory(&allocationDesc, &allocationInfo, &pHeap->m_pAllocation);
                if (FAILED(hr))
                {
                    // RE_ERROR("[D3D12Heap] failed to create {}", m_name);
                    return false;
                }

                name_t const* name = GetComponent<ngfx::heap_t, ngfx::name_t>(device, heap);
                pHeap->m_pAllocation->SetName(name->m_wname);

                return true;
            }

            void Destroy(ngfx::device_t* device, ngfx::heap_t* heap)
            {
                nd3d12::device_t* pDevice = GetComponent<ngfx::device_t, nd3d12::device_t>(device, device);
                nd3d12::heap_t*   pHeap   = GetComponent<ngfx::heap_t, nd3d12::heap_t>(device, heap);

                nd3d12::Delete(pDevice, pHeap->m_pAllocation);
                pHeap->m_pAllocation = nullptr;
            }
        }  // namespace nd3d12
    }  // namespace ngfx
}  // namespace ncore

#endif