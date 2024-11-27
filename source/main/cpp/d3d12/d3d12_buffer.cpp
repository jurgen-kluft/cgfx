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
            void CreateBuffer(ngfx::device_t* device, ngfx::buffer_t* buffer)
            {
                nd3d12::buffer_t* dxbuffer = CreateComponent<ngfx::buffer_t, nd3d12::buffer_t>(device, buffer);
                return buffer;
            }

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
                D3D12_RESOURCE_FLAGS flags          = (buffer->m_desc.usage & GfxBufferUsageUnorderedAccess) ? D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS : D3D12_RESOURCE_FLAG_NONE;
                D3D12_RESOURCE_DESC1 resourceDesc1  = CD3DX12_RESOURCE_DESC1::Buffer(buffer->m_desc.size, flags);
                D3D12_BARRIER_LAYOUT initial_layout = D3D12_BARRIER_LAYOUT_UNDEFINED;

                HRESULT hr;
                if (buffer->m_desc.heap != nullptr)
                {
                    ASSERT(buffer->m_desc.alloc_type == enums::AllocationTypePlaced);
                    ASSERT(buffer->m_desc.memory_type == buffer->m_desc.heap->m_desc.memory_type);
                    ASSERT(buffer->m_desc.size + buffer->m_desc.heap_offset <= buffer->m_desc.heap->m_desc.size);
                    hr = pAllocator->CreateAliasingResource2((D3D12MA::Allocation*)GetHandle(device, buffer->m_desc.heap), buffer->m_desc.heap_offset, &resourceDesc1, initial_layout, nullptr, 0, nullptr, IID_PPV_ARGS(&dxbuffer->m_pBuffer));
                }
                else
                {
                    D3D12MA::ALLOCATION_DESC allocationDesc = {};
                    allocationDesc.HeapType                 = d3d12_heap_type(buffer->m_desc.memory_type);
                    allocationDesc.Flags                    = buffer->m_desc.alloc_type == GfxAllocationType::Committed ? D3D12MA::ALLOCATION_FLAG_COMMITTED : D3D12MA::ALLOCATION_FLAG_NONE;
                    hr                                      = pAllocator->CreateResource3(&allocationDesc, &resourceDesc1, initial_layout, nullptr, 0, nullptr, &dxbuffer->m_pAllocation, IID_PPV_ARGS(&dxbuffer->m_pBuffer));
                }

                if (FAILED(hr))
                {
                    // RE_ERROR("[D3D12Buffer] failed to create {}", m_name);
                    return false;
                }

                const name_t* name = GetComponent<ngfx::buffer_t, name_t>(device, buffer);
                m_pBuffer->SetName(name->m_wname);
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

        //         bool D3D12Buffer::Create()
        //         {
        //             D3D12MA::Allocator* pAllocator = ((D3D12Device*)m_pDevice)->GetResourceAllocator();
        //             D3D12_RESOURCE_FLAGS flags         = (m_desc.usage & GfxBufferUsageUnorderedAccess) ? D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS : D3D12_RESOURCE_FLAG_NONE;
        //             D3D12_RESOURCE_DESC1 resourceDesc1 = CD3DX12_RESOURCE_DESC1::Buffer(m_desc.size, flags);

        //             D3D12_BARRIER_LAYOUT initial_layout = D3D12_BARRIER_LAYOUT_UNDEFINED;

        //             HRESULT hr;

        //             if (m_desc.heap != nullptr)
        //             {
        //                 ASSERT(m_desc.alloc_type == GfxAllocationType::Placed);
        //                 ASSERT(m_desc.memory_type == m_desc.heap->GetDesc().memory_type);
        //                 ASSERT(m_desc.size + m_desc.heap_offset <= m_desc.heap->GetDesc().size);

        //                 hr = pAllocator->CreateAliasingResource2((D3D12MA::Allocation*)m_desc.heap->GetHandle(), m_desc.heap_offset, &resourceDesc1, initial_layout, nullptr, 0, nullptr, IID_PPV_ARGS(&m_pBuffer));
        //             }
        //             else
        //             {
        //                 D3D12MA::ALLOCATION_DESC allocationDesc = {};
        //                 allocationDesc.HeapType                 = d3d12_heap_type(m_desc.memory_type);
        //                 allocationDesc.Flags                    = m_desc.alloc_type == GfxAllocationType::Committed ? D3D12MA::ALLOCATION_FLAG_COMMITTED : D3D12MA::ALLOCATION_FLAG_NONE;

        //                 hr = pAllocator->CreateResource3(&allocationDesc, &resourceDesc1, initial_layout, nullptr, 0, nullptr, &m_pAllocation, IID_PPV_ARGS(&m_pBuffer));
        //             }

        //             if (FAILED(hr))
        //             {
        //                 // RE_ERROR("[D3D12Buffer] failed to create {}", m_name);
        //                 return false;
        //             }

        //             eastl::wstring name_wstr = string_to_wstring(m_name);
        //             m_pBuffer->SetName(name_wstr.c_str());
        //             if (m_pAllocation)
        //             {
        //                 m_pAllocation->SetName(name_wstr.c_str());
        //             }

        //             if (m_desc.memory_type != GfxMemoryType::GpuOnly)
        //             {
        //                 CD3DX12_RANGE readRange(0, 0);
        //                 m_pBuffer->Map(0, &readRange, reinterpret_cast<void**>(&m_pCpuAddress));
        //             }
        //             return true;
        //         }

    }  // namespace ngfx
}  // namespace ncore

#endif