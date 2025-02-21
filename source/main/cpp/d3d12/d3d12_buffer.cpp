#include "cgfx/gfx_defines.h"
#include "cgfx/gfx_device.h"
#include "cgfx/d3d12/d3d12_buffer.h"
#include "cgfx/d3d12/d3d12_device.h"
#include "cgfx/d3d12/d3d12_heap.h"

#ifdef TARGET_PC

    #include "cd3d12/ma/D3D12MemAlloc.h"

namespace ncore
{
    namespace ngfx
    {
        namespace nd3d12
        {
            void CreateBuffer(ngfx::device_t* device, ngfx::buffer_t* buffer) { nd3d12::buffer_t* dxbuffer = CreateComponent<ngfx::buffer_t, nd3d12::buffer_t>(device, buffer); }

            void DestroyBuffer(ngfx::device_t* device, ngfx::buffer_t* buffer)
            {
                nd3d12::buffer_t* dxbuffer = CreateComponent<ngfx::buffer_t, nd3d12::buffer_t>(device, buffer);
                if (dxbuffer)
                {
                    nd3d12::Destroy(device, buffer);
                    DestroyComponent<ngfx::buffer_t, nd3d12::buffer_t>(device, buffer);
                }
            }

            bool Create(ngfx::device_t* device, ngfx::buffer_t* buffer)
            {
                nd3d12::buffer_t* dxbuffer = GetComponent<ngfx::buffer_t, nd3d12::buffer_t>(device, buffer);
                nd3d12::device_t* dxdevice = GetComponent<ngfx::device_t, nd3d12::device_t>(device, device);

                D3D12MA::Allocator*  pAllocator     = dxdevice->m_pResourceAllocator;
                D3D12_RESOURCE_FLAGS flags          = (buffer->m_desc.usage & enums::BufferUsageUnorderedAccess) ? D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS : D3D12_RESOURCE_FLAG_NONE;
                D3D12_RESOURCE_DESC1 resourceDesc1  = CD3DX12_RESOURCE_DESC1::Buffer(buffer->m_desc.size, flags);
                D3D12_BARRIER_LAYOUT initial_layout = D3D12_BARRIER_LAYOUT_UNDEFINED;

                HRESULT hr;
                if (buffer->m_desc.heap != nullptr)
                {
                    ASSERT(buffer->m_desc.alloc_type == enums::AllocationPlaced);
                    ASSERT(buffer->m_desc.memory_type == buffer->m_desc.heap->m_desc.memory_type);
                    ASSERT(buffer->m_desc.size + buffer->m_desc.heap_offset <= buffer->m_desc.heap->m_desc.size);

                    nd3d12::heap_t* dxHeap = GetComponent<ngfx::heap_t, nd3d12::heap_t>(device, buffer->m_desc.heap);
                    hr                     = pAllocator->CreateAliasingResource2(dxHeap->m_pAllocation, buffer->m_desc.heap_offset, &resourceDesc1, initial_layout, nullptr, 0, nullptr, IID_PPV_ARGS(&dxbuffer->m_pBuffer));
                }
                else
                {
                    D3D12MA::ALLOCATION_DESC allocationDesc = {};
                    allocationDesc.HeapType                 = d3d12_heap_type(buffer->m_desc.memory_type);
                    allocationDesc.Flags                    = buffer->m_desc.alloc_type == enums::AllocationCommitted ? D3D12MA::ALLOCATION_FLAG_COMMITTED : D3D12MA::ALLOCATION_FLAG_NONE;
                    hr                                      = pAllocator->CreateResource3(&allocationDesc, &resourceDesc1, initial_layout, nullptr, 0, nullptr, &dxbuffer->m_pAllocation, IID_PPV_ARGS(&dxbuffer->m_pBuffer));
                }

                if (FAILED(hr))
                {
                    // RE_ERROR("[D3D12Buffer] failed to create {}", m_name);
                    return false;
                }

                const name_t* name = GetComponent<ngfx::buffer_t, name_t>(device, buffer);
                dxbuffer->m_pBuffer->SetName(name->m_wname);

                if (dxbuffer->m_pAllocation)
                {
                    dxbuffer->m_pAllocation->SetName(name->m_wname);
                }

                if (buffer->m_desc.memory_type != enums::MemoryGpuOnly)
                {
                    CD3DX12_RANGE readRange(0, 0);
                    dxbuffer->m_pBuffer->Map(0, &readRange, reinterpret_cast<void**>(&dxbuffer->m_pCpuAddress));
                }
                return true;
            }

            void Destroy(ngfx::device_t* device, ngfx::buffer_t* buffer)
            {
                nd3d12::device_t* dxdevice = GetComponent<ngfx::device_t, nd3d12::device_t>(device, device);
                nd3d12::buffer_t* dxbuffer = GetComponent<ngfx::buffer_t, nd3d12::buffer_t>(device, buffer);
                nd3d12::Delete(dxdevice, dxbuffer->m_pBuffer);
                nd3d12::Delete(dxdevice, dxbuffer->m_pAllocation);
            }

            void* GetCpuAddress(ngfx::device_t* device, ngfx::buffer_t* buffer)
            {
                nd3d12::buffer_t* dxbuffer = GetComponent<ngfx::buffer_t, nd3d12::buffer_t>(device, buffer);
                return dxbuffer->m_pCpuAddress;
            }

            u64 GetGpuAddress(ngfx::device_t* device, ngfx::buffer_t* buffer)
            {
                nd3d12::buffer_t* dxbuffer = GetComponent<ngfx::buffer_t, nd3d12::buffer_t>(device, buffer);
                return dxbuffer->m_pBuffer->GetGPUVirtualAddress();
            }

            u32 GetRequiredStagingBufferSize(ngfx::device_t* device, ngfx::buffer_t* buffer)
            {
                nd3d12::buffer_t*   dxbuffer = GetComponent<ngfx::buffer_t, nd3d12::buffer_t>(device, buffer);
                nd3d12::device_t*   dxdevice = GetComponent<ngfx::device_t, nd3d12::device_t>(device, device);
                D3D12_RESOURCE_DESC desc     = dxbuffer->m_pBuffer->GetDesc();

                u64 size;
                dxdevice->m_pDevice->GetCopyableFootprints(&desc, 0, 1, 0, nullptr, nullptr, nullptr, &size);
                return (u32)size;
            }
        }  // namespace nd3d12
    }  // namespace ngfx
}  // namespace ncore

#endif