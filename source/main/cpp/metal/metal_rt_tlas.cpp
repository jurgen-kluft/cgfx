#include "cgfx/metal/metal_rt_tlas.h"
#include "cgfx/metal/metal_device.h"
#include "cgfx/metal/metal_rt_blas.h"

#include "cmacos/Metal/Metal.hpp"
#include "cmacos/MetalShaderConverter/IRConverterRuntime/ir_converter_runtime.h"

namespace ncore
{
    namespace ngfx
    {
        MetalRayTracingTLAS::MetalRayTracingTLAS(MetalDevice* pDevice, const GfxRayTracingTLASDesc& desc, const eastl::string& name)
        {
            m_pDevice = pDevice;
            m_desc    = desc;
            m_name    = name;
        }

        MetalRayTracingTLAS::~MetalRayTracingTLAS()
        {
            m_pAccelerationStructure->release();
            m_pDescriptor->release();
            m_pScratchBuffer->release();
            m_pInstanceBuffer->release();
            m_pGPUHeaderBuffer->release();
        }

        bool MetalRayTracingTLAS::Create()
        {
            MTL::Device* device = (MTL::Device*)m_pDevice->GetHandle();

            m_instanceBufferSize       = sizeof(MTL::AccelerationStructureUserIDInstanceDescriptor) * m_desc.instance_count * GFX_MAX_INFLIGHT_FRAMES;
            m_pInstanceBuffer          = device->newBuffer(m_instanceBufferSize, MTL::ResourceStorageModeShared);
            m_instanceBufferCpuAddress = m_pInstanceBuffer->contents();

            m_pDescriptor = MTL::InstanceAccelerationStructureDescriptor::alloc()->init();
            m_pDescriptor->setUsage(ToAccelerationStructureUsage(m_desc.flags));
            m_pDescriptor->setInstanceCount((NS::UInteger)m_desc.instance_count);
            m_pDescriptor->setInstanceDescriptorBuffer(m_pInstanceBuffer);
            m_pDescriptor->setInstanceDescriptorType(MTL::AccelerationStructureInstanceDescriptorTypeUserID);

            MTL::AccelerationStructureSizes asSizes = device->accelerationStructureSizes(m_pDescriptor);

            m_pAccelerationStructure = device->newAccelerationStructure(asSizes.accelerationStructureSize);
            m_pScratchBuffer         = device->newBuffer(asSizes.buildScratchBufferSize, MTL::ResourceStorageModePrivate);

            if (m_pAccelerationStructure == nullptr || m_pScratchBuffer == nullptr)
            {
                //RE_ERROR("[MetalRayTracingTLAS] failed to create : {}", m_name);
                return false;
            }

            ((MetalDevice*)m_pDevice)->MakeResident(m_pAccelerationStructure);
            ((MetalDevice*)m_pDevice)->MakeResident(m_pScratchBuffer);
            ((MetalDevice*)m_pDevice)->MakeResident(m_pInstanceBuffer);

            NS::String* label = NS::String::alloc()->init(m_name.c_str(), NS::StringEncoding::UTF8StringEncoding);
            m_pAccelerationStructure->setLabel(label);
            label->release();

            NS::UInteger headerSize = sizeof(IRRaytracingAccelerationStructureGPUHeader) + sizeof(u32) * m_desc.instance_count;
            m_pGPUHeaderBuffer      = device->newBuffer(headerSize, MTL::ResourceStorageModeShared);
            ((MetalDevice*)m_pDevice)->MakeResident(m_pGPUHeaderBuffer);

            //TODO allocate
            u32* instanceContributions = nullptr;
            //ize(m_desc.instance_count, 0);

            IRRaytracingSetAccelerationStructure((u8*)m_pGPUHeaderBuffer->contents(), m_pAccelerationStructure->gpuResourceID(), (u8*)m_pGPUHeaderBuffer->contents() + sizeof(IRRaytracingAccelerationStructureGPUHeader), instanceContributions,
                                                 m_desc.instance_count);

            return true;
        }

        void MetalRayTracingTLAS::UpdateInstance(const GfxRayTracingInstance* instances, u32 instance_count)
        {
            ASSERT(instance_count <= m_desc.instance_count);

            if (m_currentInstanceBufferOffset + sizeof(MTL::AccelerationStructureUserIDInstanceDescriptor) * instance_count > m_instanceBufferSize)
            {
                m_currentInstanceBufferOffset = 0;
            }

            s32 accelerationStructuresCount = 0;
            MTL::AccelerationStructure** accelerationStructures = nullptr;
            // TODO allocate
            //accelerationStructures.reserve(instance_count);

            MTL::AccelerationStructureUserIDInstanceDescriptor* instanceDescriptors = (MTL::AccelerationStructureUserIDInstanceDescriptor*)((char*)m_instanceBufferCpuAddress + m_currentInstanceBufferOffset);

            for (u32 i = 0; i < instance_count; ++i)
            {
                const GfxRayTracingInstance& instance = instances[i];
                accelerationStructures[accelerationStructuresCount++] = ((MTL::AccelerationStructure*)instance.blas->GetHandle());

                instanceDescriptors[i].accelerationStructureIndex      = i;
                instanceDescriptors[i].options                         = ToAccelerationStructureInstanceOptions(instance.flags);
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
            m_pDescriptor->setInstancedAccelerationStructures(instancedAccelerationStructures);
            m_pDescriptor->setInstanceDescriptorBufferOffset((NS::UInteger)m_currentInstanceBufferOffset);

            m_currentInstanceBufferOffset += sizeof(MTL::AccelerationStructureUserIDInstanceDescriptor) * instance_count;
            instancedAccelerationStructures->release();
        }

    }  // namespace ngfx
}  // namespace ncore
