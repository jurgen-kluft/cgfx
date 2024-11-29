#include "cgfx/d3d12/d3d12_rt_blas.h"
#include "cgfx/d3d12/d3d12_device.h"
#include "cgfx/d3d12/d3d12_buffer.h"
#include "ccore/c_math.h"

#include "cd3d12/ma/D3D12MemAlloc.h"

namespace ncore
{
    namespace ngfx
    {
#ifdef TARGET_PC
        namespace nd3d12
        {
            void CreateRayTracingBLAS(ngfx::device_t* pDevice, ngfx::blas_t* pBLAS) { nd3d12::blas_t* dxblas = CreateComponent<ngfx::blas_t, nd3d12::blas_t>(pDevice, pBLAS); }

            void DestroyRayTracingBLAS(ngfx::device_t* pDevice, ngfx::blas_t* pBLAS)
            {
                nd3d12::blas_t* dxblas = GetComponent<ngfx::blas_t, nd3d12::blas_t>(pDevice, pBLAS);
                if (dxblas)
                {
                    nd3d12::Destroy(pDevice, pBLAS);
                    DestroyComponent<ngfx::blas_t, nd3d12::blas_t>(pDevice, pBLAS);
                }
            }

            void Destroy(ngfx::device_t* pDevice, ngfx::blas_t* pBLAS)
            {
                nd3d12::device_t* dxdevice = GetComponent<ngfx::device_t, nd3d12::device_t>(pDevice, pDevice);
                nd3d12::blas_t*   dxpblas  = GetComponent<ngfx::blas_t, nd3d12::blas_t>(pDevice, pBLAS);
                nd3d12::Delete(dxdevice, dxpblas->m_pASBuffer);
                nd3d12::Delete(dxdevice, dxpblas->m_pASAllocation);
                nd3d12::Delete(dxdevice, dxpblas->m_pScratchBuffer);
                nd3d12::Delete(dxdevice, dxpblas->m_pScratchAllocation);
            }

            bool Create(ngfx::device_t* pDevice, ngfx::blas_t* pBLAS)
            {
                nd3d12::device_t* dxdevice = GetComponent<ngfx::device_t, nd3d12::device_t>(pDevice, pDevice);
                nd3d12::blas_t*   dxblas   = GetComponent<ngfx::blas_t, nd3d12::blas_t>(pDevice, pBLAS);

                // TODO, we need a way to 'allocate' the carray from pDevice->m_allocator
                dxblas->m_geometries.reserve(pBLAS->m_desc.geometries.size());

                for (s32 i = 0; i < pBLAS->m_desc.geometries.size(); ++i)
                {
                    const rt_geometry_t* geometry = pBLAS->m_desc.geometries.mArray[i];

                    D3D12_RAYTRACING_GEOMETRY_DESC d3d12_geometry       = {};
                    d3d12_geometry.Type                                 = D3D12_RAYTRACING_GEOMETRY_TYPE_TRIANGLES;  // todo : support AABB
                    d3d12_geometry.Flags                                = geometry->opaque ? D3D12_RAYTRACING_GEOMETRY_FLAG_OPAQUE : D3D12_RAYTRACING_GEOMETRY_FLAG_NONE;
                    d3d12_geometry.Triangles.IndexFormat                = dxgi_format(geometry->index_format);
                    d3d12_geometry.Triangles.VertexFormat               = dxgi_format(geometry->vertex_format);
                    d3d12_geometry.Triangles.IndexCount                 = geometry->index_count;
                    d3d12_geometry.Triangles.VertexCount                = geometry->vertex_count;
                    nd3d12::buffer_t* dxIndexBuffer                     = GetComponent<ngfx::buffer_t, nd3d12::buffer_t>(pDevice, geometry->index_buffer);
                    nd3d12::buffer_t* dxVertexBuffer                    = GetComponent<ngfx::buffer_t, nd3d12::buffer_t>(pDevice, geometry->vertex_buffer);
                    d3d12_geometry.Triangles.IndexBuffer                = dxIndexBuffer->m_pBuffer->GetGPUVirtualAddress() + geometry->index_buffer_offset;
                    d3d12_geometry.Triangles.VertexBuffer.StartAddress  = dxVertexBuffer->m_pBuffer->GetGPUVirtualAddress() + geometry->vertex_buffer_offset;
                    d3d12_geometry.Triangles.VertexBuffer.StrideInBytes = geometry->vertex_stride;

                    dxblas->m_geometries.push_back(d3d12_geometry);
                }

                ID3D12Device5* device = dxdevice->m_pDevice;

                D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS buildInput;
                buildInput.Type           = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL;
                buildInput.Flags          = d3d12_rt_as_flags(pBLAS->m_desc.flags);
                buildInput.NumDescs       = (UINT)dxblas->m_geometries.size();
                buildInput.DescsLayout    = D3D12_ELEMENTS_LAYOUT_ARRAY;
                buildInput.pGeometryDescs = dxblas->m_geometries.mArray;

                D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO info = {};
                device->GetRaytracingAccelerationStructurePrebuildInfo(&buildInput, &info);

                // todo : better memory allocation and compaction like what RTXMU does
                D3D12MA::Allocator*      pAllocator     = dxdevice->m_pResourceAllocator;
                D3D12MA::ALLOCATION_DESC allocationDesc = {};
                allocationDesc.HeapType                 = D3D12_HEAP_TYPE_DEFAULT;

                CD3DX12_RESOURCE_DESC asBufferDesc      = CD3DX12_RESOURCE_DESC::Buffer(info.ResultDataMaxSizeInBytes, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
                CD3DX12_RESOURCE_DESC scratchBufferDesc = CD3DX12_RESOURCE_DESC::Buffer(math::g_max(info.ScratchDataSizeInBytes, info.UpdateScratchDataSizeInBytes), D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
                pAllocator->CreateResource(&allocationDesc, &asBufferDesc, D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE, nullptr, &dxblas->m_pASAllocation, IID_PPV_ARGS(&dxblas->m_pASBuffer));
                pAllocator->CreateResource(&allocationDesc, &scratchBufferDesc, D3D12_RESOURCE_STATE_COMMON, nullptr, &dxblas->m_pScratchAllocation, IID_PPV_ARGS(&dxblas->m_pScratchBuffer));

                // TODO : set name
                // m_pASBuffer->SetName(string_to_wstring(m_name).c_str());
                // m_pASAllocation->SetName(string_to_wstring(m_name).c_str());

                dxblas->m_buildDesc.Inputs                           = buildInput;
                dxblas->m_buildDesc.DestAccelerationStructureData    = dxblas->m_pASBuffer->GetGPUVirtualAddress();
                dxblas->m_buildDesc.ScratchAccelerationStructureData = dxblas->m_pScratchBuffer->GetGPUVirtualAddress();
                dxblas->m_buildDesc.SourceAccelerationStructureData  = 0;

                return true;
            }

            void GetUpdateDesc(ngfx::device_t* device, ngfx::blas_t* blas, nd3d12::blas_t* dxblas, D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC& desc, D3D12_RAYTRACING_GEOMETRY_DESC& geometry, buffer_t* vertex_buffer, u32 vertex_buffer_offset)
            {
                ASSERT(blas->m_desc.flags & enums::rt::AsFlagAllowUpdate);
                ASSERT(dxblas->m_geometries.size() == 1);  // todo : suppport more than 1

                geometry                                     = dxblas->m_geometries.mArray[0];

                nd3d12::buffer_t* dxBuffer                   = GetComponent<ngfx::buffer_t, nd3d12::buffer_t>(device, blas->m_desc.geometries[0]->vertex_buffer);
                geometry.Triangles.VertexBuffer.StartAddress = dxBuffer->m_pBuffer->GetGPUVirtualAddress() + vertex_buffer_offset;

                D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS updateInputs;
                updateInputs.Type           = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL;
                updateInputs.Flags          = d3d12_rt_as_flags(blas->m_desc.flags) | D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PERFORM_UPDATE;
                updateInputs.NumDescs       = 1;
                updateInputs.DescsLayout    = D3D12_ELEMENTS_LAYOUT_ARRAY;
                updateInputs.pGeometryDescs = &geometry;

                desc.Inputs                           = updateInputs;
                desc.DestAccelerationStructureData    = dxblas->m_pASBuffer->GetGPUVirtualAddress();
                desc.SourceAccelerationStructureData  = dxblas->m_pASBuffer->GetGPUVirtualAddress();
                desc.ScratchAccelerationStructureData = dxblas->m_pScratchBuffer->GetGPUVirtualAddress();
            }

        }  // namespace nd3d12
#endif
    }  // namespace ngfx
}  // namespace ncore
