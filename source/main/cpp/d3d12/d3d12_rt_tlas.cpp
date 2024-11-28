#include "cgfx/d3d12/d3d12_rt_tlas.h"
#include "cgfx/d3d12/d3d12_rt_blas.h"
#include "cgfx/d3d12/d3d12_device.h"
#include "cd3d12/ma/D3D12MemAlloc.h"
#include "ccore/c_math.h"

namespace ncore
{
    namespace ngfx
    {
#ifdef TARGET_PC
        namespace nd3d12
        {
            void CreateRayTracingTLAS(ngfx::device_t* device, ngfx::tlas_t* tlas) { nd3d12::tlas_t* dxtlas = CreateComponent<ngfx::tlas_t, nd3d12::tlas_t>(device, tlas); }

            void DestroyRayTracingTLAS(ngfx::device_t* device, ngfx::tlas_t* tlas)
            {
                nd3d12::tlas_t* dxtlas = GetComponent<ngfx::tlas_t, nd3d12::tlas_t>(device, tlas);
                if (dxtlas)
                {
                    nd3d12::Destroy(device, tlas);
                    DestroyComponent<ngfx::tlas_t, nd3d12::tlas_t>(device, tlas);
                }
            }

            void Destroy(ngfx::device_t* device, ngfx::tlas_t* tlas)
            {
                nd3d12::tlas_t* dxtlas = GetComponent<ngfx::tlas_t, nd3d12::tlas_t>(device, tlas);
                if (dxtlas)
                {
                    nd3d12::device_t* dxdevice = GetComponent<ngfx::device_t, nd3d12::device_t>(device, device);
                    nd3d12::Delete(dxdevice, dxtlas->m_pASBuffer);
                    nd3d12::Delete(dxdevice, dxtlas->m_pASAllocation);
                    nd3d12::Delete(dxdevice, dxtlas->m_pScratchBuffer);
                    nd3d12::Delete(dxdevice, dxtlas->m_pScratchAllocation);
                    nd3d12::Delete(dxdevice, dxtlas->m_pInstanceBuffer);
                    nd3d12::Delete(dxdevice, dxtlas->m_pInstanceAllocation);
                }
            }

            bool Create(ngfx::device_t* device, ngfx::tlas_t* tlas)
            {
                nd3d12::device_t* dxdevice = GetComponent<ngfx::device_t, nd3d12::device_t>(device, device);
                nd3d12::tlas_t*   dxtlas   = GetComponent<ngfx::tlas_t, nd3d12::tlas_t>(device, tlas);

                ID3D12Device12*                                       dx12Device       = dxdevice->m_pDevice;
                D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS prebuildDesc = {};
                prebuildDesc.Type                                                 = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL;
                prebuildDesc.DescsLayout                                          = D3D12_ELEMENTS_LAYOUT_ARRAY;
                prebuildDesc.NumDescs                                             = tlas->m_desc.instance_count;
                prebuildDesc.Flags                                                = d3d12_rt_as_flags(tlas->m_desc.flags);

                D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO info = {};
                dx12Device->GetRaytracingAccelerationStructurePrebuildInfo(&prebuildDesc, &info);

                // todo : better memory allocation and compaction like what RTXMU does
                D3D12MA::Allocator*      pAllocator     = dxdevice->m_pResourceAllocator;
                D3D12MA::ALLOCATION_DESC allocationDesc = {};
                allocationDesc.HeapType                 = D3D12_HEAP_TYPE_DEFAULT;

                CD3DX12_RESOURCE_DESC asBufferDesc      = CD3DX12_RESOURCE_DESC::Buffer(info.ResultDataMaxSizeInBytes, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
                CD3DX12_RESOURCE_DESC scratchBufferDesc = CD3DX12_RESOURCE_DESC::Buffer(info.ScratchDataSizeInBytes, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
                pAllocator->CreateResource(&allocationDesc, &asBufferDesc, D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE, nullptr, &dxtlas->m_pASAllocation, IID_PPV_ARGS(&dxtlas->m_pASBuffer));
                pAllocator->CreateResource(&allocationDesc, &scratchBufferDesc, D3D12_RESOURCE_STATE_COMMON, nullptr, &dxtlas->m_pScratchAllocation, IID_PPV_ARGS(&dxtlas->m_pScratchBuffer));

                allocationDesc.HeapType                  = D3D12_HEAP_TYPE_UPLOAD;
                dxtlas->m_nInstanceBufferSize            = math::g_roundUpPow2(sizeof(D3D12_RAYTRACING_INSTANCE_DESC) * tlas->m_desc.instance_count, D3D12_RAYTRACING_INSTANCE_DESCS_BYTE_ALIGNMENT) * GFX_MAX_INFLIGHT_FRAMES;
                CD3DX12_RESOURCE_DESC instanceBufferDesc = CD3DX12_RESOURCE_DESC::Buffer(dxtlas->m_nInstanceBufferSize);
                pAllocator->CreateResource(&allocationDesc, &instanceBufferDesc, D3D12_RESOURCE_STATE_COMMON, nullptr, &dxtlas->m_pInstanceAllocation, IID_PPV_ARGS(&dxtlas->m_pInstanceBuffer));

                // TODO : set name
                // m_pASBuffer->SetName(string_to_wstring(m_name).c_str());
                // m_pASAllocation->SetName(string_to_wstring(m_name).c_str());

                CD3DX12_RANGE readRange(0, 0);
                dxtlas->m_pInstanceBuffer->Map(0, &readRange, reinterpret_cast<void**>(&dxtlas->m_pInstanceBufferCpuAddress));

                return true;
            }

            void GetBuildDesc(ngfx::device_t* device, ngfx::tlas_t* tlas, nd3d12::tlas_t* dxtlas, D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC& desc, const rt_instance_t* instances, u32 instance_count)
            {
                ASSERT(instance_count <= tlas->m_desc.instance_count);

                if (dxtlas->m_nCurrentInstanceBufferOffset + sizeof(D3D12_RAYTRACING_INSTANCE_DESC) * instance_count > dxtlas->m_nInstanceBufferSize)
                {
                    dxtlas->m_nCurrentInstanceBufferOffset = 0;
                }

                desc.Inputs.Type                      = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL;
                desc.Inputs.DescsLayout               = D3D12_ELEMENTS_LAYOUT_ARRAY;
                desc.Inputs.InstanceDescs             = dxtlas->m_pInstanceBuffer->GetGPUVirtualAddress() + dxtlas->m_nCurrentInstanceBufferOffset;
                desc.Inputs.NumDescs                  = instance_count;
                desc.Inputs.Flags                     = d3d12_rt_as_flags(tlas->m_desc.flags);
                desc.DestAccelerationStructureData    = dxtlas->m_pASBuffer->GetGPUVirtualAddress();
                desc.ScratchAccelerationStructureData = dxtlas->m_pScratchBuffer->GetGPUVirtualAddress();

                D3D12_RAYTRACING_INSTANCE_DESC* instanceDescs = (D3D12_RAYTRACING_INSTANCE_DESC*)((char*)dxtlas->m_pInstanceBufferCpuAddress + dxtlas->m_nCurrentInstanceBufferOffset);
                for (u32 i = 0; i < instance_count; ++i)
                {
                    nmem::memcpy(instanceDescs[i].Transform, instances[i].transform, sizeof(float) * 12);
                    instanceDescs[i].InstanceID                          = instances[i].instance_id;
                    instanceDescs[i].InstanceMask                        = instances[i].instance_mask;
                    instanceDescs[i].InstanceContributionToHitGroupIndex = 0;  // DXR 1.1 does't need it
                    instanceDescs[i].Flags                               = d3d12_rt_instance_flags(instances[i].instance_flags);

                    // TODO It seems that a single rt_instance can have a blas_t, so why not make those components of the rt_instance_t?

                    nd3d12::blas_t* dxblas                 = GetComponent<ngfx::blas_t, nd3d12::blas_t>(device, instances[i].blas);
                    instanceDescs[i].AccelerationStructure = dxblas->m_pASBuffer->GetGPUVirtualAddress();
                    // instanceDescs[i].AccelerationStructure               = ((D3D12RayTracingBLAS*)instances[i].blas)->GetGpuAddress();
                }

                dxtlas->m_nCurrentInstanceBufferOffset += sizeof(D3D12_RAYTRACING_INSTANCE_DESC) * instance_count;
                dxtlas->m_nCurrentInstanceBufferOffset = math::g_roundUpPow2(dxtlas->m_nCurrentInstanceBufferOffset, D3D12_RAYTRACING_INSTANCE_DESCS_BYTE_ALIGNMENT);
            }

            void*                     GetHandle(ngfx::device_t* device, const ngfx::tlas_t* tlas);
            D3D12_GPU_VIRTUAL_ADDRESS GetGpuAddress(ngfx::device_t* device, const ngfx::tlas_t* tlas);
        }  // namespace nd3d12
#endif
    }  // namespace ngfx
}  // namespace ncore
