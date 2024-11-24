#include "cgfx/metal/metal_rt_tlas.h"
#include "cgfx/metal/metal_device.h"
#include "cgfx/metal/metal_rt_blas.h"

#include "cmacos/Metal/Metal.hpp"
#include "cmacos/MetalShaderConverter/IRConverterRuntime/ir_converter_runtime.h"

namespace ncore
{
    namespace ngfx
    {
        namespace nmetal
        {
            void CreateRayTracingTLAS(ngfx::device_t* device, ngfx::tlas_t* tlas) { mtlas_t* mtlas = CreateComponent<ngfx::tlas_t, mtlas_t>(device, tlas); }

            void Destroy(ngfx::device_t* device, ngfx::tlas_t* tlas)
            {
                mtlas_t* mtlas = GetComponent<ngfx::tlas_t, mtlas_t>(device, tlas);
                mtlas->m_pAccelerationStructure->release();
                mtlas->m_pDescriptor->release();
                mtlas->m_pScratchBuffer->release();
                mtlas->m_pInstanceBuffer->release();
                mtlas->m_pGPUHeaderBuffer->release();
            }

            bool Create(ngfx::device_t* device, ngfx::tlas_t* tlas)
            {
                mtlas_t*          mtlas     = GetComponent<ngfx::tlas_t, mtlas_t>(device, tlas);
                nmetal::device_t* mdevice   = GetComponent<ngfx::device_t, nmetal::device_t>(device, device);
                MTL::Device*      mtlDevice = mdevice->m_pDevice;

                mtlas->m_instanceBufferSize       = sizeof(MTL::AccelerationStructureUserIDInstanceDescriptor) * tlas->m_desc.instance_count * GFX_MAX_INFLIGHT_FRAMES;
                mtlas->m_pInstanceBuffer          = mtlDevice->newBuffer(mtlas->m_instanceBufferSize, MTL::ResourceStorageModeShared);
                mtlas->m_instanceBufferCpuAddress = mtlas->m_pInstanceBuffer->contents();

                mtlas->m_pDescriptor = MTL::InstanceAccelerationStructureDescriptor::alloc()->init();
                mtlas->m_pDescriptor->setUsage(ToAccelerationStructureUsage(tlas->m_desc.flags));
                mtlas->m_pDescriptor->setInstanceCount((NS::UInteger)tlas->m_desc.instance_count);
                mtlas->m_pDescriptor->setInstanceDescriptorBuffer(mtlas->m_pInstanceBuffer);
                mtlas->m_pDescriptor->setInstanceDescriptorType(MTL::AccelerationStructureInstanceDescriptorTypeUserID);

                MTL::AccelerationStructureSizes asSizes = mtlDevice->accelerationStructureSizes(mtlas->m_pDescriptor);

                mtlas->m_pAccelerationStructure = mtlDevice->newAccelerationStructure(asSizes.accelerationStructureSize);
                mtlas->m_pScratchBuffer         = mtlDevice->newBuffer(asSizes.buildScratchBufferSize, MTL::ResourceStorageModePrivate);

                if (mtlas->m_pAccelerationStructure == nullptr || mtlas->m_pScratchBuffer == nullptr)
                {
                    // RE_ERROR("[MetalRayTracingTLAS] failed to create : {}", m_name);
                    return false;
                }

                MakeResident(device, mtlas->m_pAccelerationStructure);
                MakeResident(device, mtlas->m_pScratchBuffer);
                MakeResident(device, mtlas->m_pInstanceBuffer);

                name_t const* name  = GetComponent<ngfx::tlas_t, name_t>(device, tlas);
                NS::String*   label = NS::String::alloc()->init(name->m_name, NS::StringEncoding::UTF8StringEncoding);
                mtlas->m_pAccelerationStructure->setLabel(label);
                label->release();

                NS::UInteger headerSize   = sizeof(IRRaytracingAccelerationStructureGPUHeader) + sizeof(u32) * tlas->m_desc.instance_count;
                mtlas->m_pGPUHeaderBuffer = mtlDevice->newBuffer(headerSize, MTL::ResourceStorageModeShared);
                MakeResident(device, mtlas->m_pGPUHeaderBuffer);

                // TODO allocate
                u32* instanceContributions = nullptr;
                // ize(m_desc.instance_count, 0);

                IRRaytracingSetAccelerationStructure((u8*)mtlas->m_pGPUHeaderBuffer->contents(), mtlas->m_pAccelerationStructure->gpuResourceID(), (u8*)mtlas->m_pGPUHeaderBuffer->contents() + sizeof(IRRaytracingAccelerationStructureGPUHeader),
                                                     instanceContributions, tlas->m_desc.instance_count);

                return true;
            }

            MTL::AccelerationStructure* GetHandle(ngfx::device_t* device, const ngfx::tlas_t* tlas)
            {
                mtlas_t* mtlas = GetComponent<ngfx::tlas_t, mtlas_t>(device, tlas);
                return mtlas->m_pAccelerationStructure;
            }

            void UpdateInstance(ngfx::device_t* device, ngfx::tlas_t* tlas, const rt_instance_t* instances, u32 instance_count)
            {
                ASSERT(instance_count <= m_desc.instance_count);

                mtlas_t* mtlas = GetComponent<ngfx::tlas_t, mtlas_t>(device, tlas);
                if (mtlas->m_currentInstanceBufferOffset + sizeof(MTL::AccelerationStructureUserIDInstanceDescriptor) * instance_count > mtlas->m_instanceBufferSize)
                {
                    mtlas->m_currentInstanceBufferOffset = 0;
                }

                s32                          accelerationStructuresCount = 0;
                MTL::AccelerationStructure** accelerationStructures      = nullptr;
                // TODO allocate
                // accelerationStructures.reserve(instance_count);

                MTL::AccelerationStructureUserIDInstanceDescriptor* instanceDescriptors = (MTL::AccelerationStructureUserIDInstanceDescriptor*)((char*)mtlas->m_instanceBufferCpuAddress + mtlas->m_currentInstanceBufferOffset);

                for (u32 i = 0; i < instance_count; ++i)
                {
                    const rt_instance_t& instance = instances[i];
                    nmetal::mblas_t*     blas     = GetComponent<ngfx::blas_t, nmetal::mblas_t>(device, instance.blas);
                    // accelerationStructures[accelerationStructuresCount++] = ((MTL::AccelerationStructure*)instance.blas->GetHandle());
                    accelerationStructures[accelerationStructuresCount++] = blas->m_pAccelerationStructure;

                    instanceDescriptors[i].accelerationStructureIndex      = i;
                    instanceDescriptors[i].options                         = ToAccelerationStructureInstanceOptions(instance.instance_flags);
                    instanceDescriptors[i].userID                          = instance.instance_id;
                    instanceDescriptors[i].mask                            = instance.instance_mask;
                    instanceDescriptors[i].intersectionFunctionTableOffset = 0;

                    // 3x4 -> 4x3
                    instanceDescriptors[i].transformationMatrix[0][0] = instance.transform[0];
                    instanceDescriptors[i].transformationMatrix[0][1] = instance.transform[4];
                    instanceDescriptors[i].transformationMatrix[0][2] = instance.transform[8];
                    instanceDescriptors[i].transformationMatrix[1][0] = instance.transform[1];
                    instanceDescriptors[i].transformationMatrix[1][1] = instance.transform[5];
                    instanceDescriptors[i].transformationMatrix[1][2] = instance.transform[9];
                    instanceDescriptors[i].transformationMatrix[2][0] = instance.transform[2];
                    instanceDescriptors[i].transformationMatrix[2][1] = instance.transform[6];
                    instanceDescriptors[i].transformationMatrix[2][2] = instance.transform[10];
                    instanceDescriptors[i].transformationMatrix[3][0] = instance.transform[3];
                    instanceDescriptors[i].transformationMatrix[3][1] = instance.transform[7];
                    instanceDescriptors[i].transformationMatrix[3][2] = instance.transform[11];
                }

                NS::Array* instancedAccelerationStructures = NS::Array::alloc()->init((NS::Object**)accelerationStructures, (NS::UInteger)accelerationStructuresCount);
                mtlas->m_pDescriptor->setInstancedAccelerationStructures(instancedAccelerationStructures);
                mtlas->m_pDescriptor->setInstanceDescriptorBufferOffset((NS::UInteger)mtlas->m_currentInstanceBufferOffset);

                mtlas->m_currentInstanceBufferOffset += sizeof(MTL::AccelerationStructureUserIDInstanceDescriptor) * instance_count;
                instancedAccelerationStructures->release();
            }

        }  // namespace nmetal

    }  // namespace ngfx
}  // namespace ncore
