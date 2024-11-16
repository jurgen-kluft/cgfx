#include "cbase/c_integer.h"
#include "cgfx/metal/metal_rt_blas.h"
#include "cgfx/metal/metal_device.h"
#include "cgfx/metal/metal_buffer.h"

namespace ncore
{
    namespace ngfx
    {
        // MetalRayTracingBLAS::MetalRayTracingBLAS(MetalDevice* pDevice, const GfxRayTracingBLASDesc& desc, const char* name)
        // {
        //     m_pDevice          = pDevice;
        //     m_desc             = desc;
        //     m_name             = name;
        //     m_geometries       = nullptr;
        //     m_geometries_count = 0;
        // }

        // MetalRayTracingBLAS::~MetalRayTracingBLAS()
        // {
        //     m_pAccelerationStructure->release();
        //     m_pDescriptor->release();
        //     m_pScratchBuffer->release();

        //     for (size_t i = 0; i < m_geometries_count; ++i)
        //     {
        //         m_geometries[i]->release();
        //     }
        // }

        // bool MetalRayTracingBLAS::Create()
        // {
        //     // TODO, allocate memory for geometries
        //     // m_geometries.reserve(m_desc.geometry_counth);

        //     for (size_t i = 0; i < m_desc.geometries_count; ++i)
        //     {
        //         const GfxRayTracingGeometry& geometry = m_desc.geometries[i];

        //         MTL::AccelerationStructureTriangleGeometryDescriptor* geometryDescriptor = MTL::AccelerationStructureTriangleGeometryDescriptor::alloc()->init();
        //         geometryDescriptor->setOpaque(geometry.opaque);
        //         geometryDescriptor->setVertexBuffer((MTL::Buffer*)geometry.vertex_buffer->GetHandle());
        //         geometryDescriptor->setVertexBufferOffset((NS::UInteger)geometry.vertex_buffer_offset);
        //         geometryDescriptor->setVertexStride((NS::UInteger)geometry.vertex_stride);
        //         geometryDescriptor->setVertexFormat(ToAttributeFormat(geometry.vertex_format));
        //         geometryDescriptor->setIndexBuffer((MTL::Buffer*)geometry.index_buffer->GetHandle());
        //         geometryDescriptor->setIndexBufferOffset((NS::UInteger)geometry.index_buffer_offset);
        //         geometryDescriptor->setIndexType(geometry.index_format == Gfx::R16UI ? MTL::IndexTypeUInt16 : MTL::IndexTypeUInt32);
        //         geometryDescriptor->setTriangleCount((NS::UInteger)geometry.index_count / 3);

        //         m_geometries[m_geometries_count++] = geometryDescriptor;
        //     }

        //     NS::Array* geometryDescriptors = NS::Array::alloc()->init((NS::Object**)m_geometries, (NS::UInteger)m_geometries_count);

        //     m_pDescriptor = MTL::PrimitiveAccelerationStructureDescriptor::alloc()->init();
        //     m_pDescriptor->setGeometryDescriptors(geometryDescriptors);
        //     m_pDescriptor->setUsage(ToAccelerationStructureUsage(m_desc.flags));

        //     MTL::Device*                    device  = (MTL::Device*)m_pDevice->GetHandle();
        //     MTL::AccelerationStructureSizes asSizes = device->accelerationStructureSizes(m_pDescriptor);

        //     m_pAccelerationStructure = device->newAccelerationStructure(asSizes.accelerationStructureSize);
        //     m_pScratchBuffer         = device->newBuffer(math::max(asSizes.buildScratchBufferSize, asSizes.refitScratchBufferSize), MTL::ResourceStorageModePrivate);

        //     geometryDescriptors->release();

        //     if (m_pAccelerationStructure == nullptr || m_pScratchBuffer == nullptr)
        //     {
        //         // RE_ERROR("[MetalRayTracingBLAS] failed to create : {}", m_name);
        //         return false;
        //     }

        //     ((MetalDevice*)m_pDevice)->MakeResident(m_pAccelerationStructure);
        //     ((MetalDevice*)m_pDevice)->MakeResident(m_pScratchBuffer);

        //     NS::String* label = NS::String::alloc()->init(m_name, NS::StringEncoding::UTF8StringEncoding);
        //     m_pAccelerationStructure->setLabel(label);
        //     label->release();

        //     return true;
        // }

        // void MetalRayTracingBLAS::UpdateVertexBuffer(buffer_t* vertex_buffer, u32 vertex_buffer_offset)
        // {
        //     ASSERT(m_desc.flags & GfxRayTracingASFlagAllowUpdate);
        //     ASSERT(m_geometries_count == 1);  // todo : suppport more than 1

        //     m_geometries[0]->setVertexBuffer((MTL::Buffer*)vertex_buffer->GetHandle());
        //     m_geometries[0]->setVertexBufferOffset((NS::UInteger)vertex_buffer_offset);

        //     NS::Array* geometryDescriptors = NS::Array::alloc()->init((NS::Object**)m_geometries, (NS::UInteger)m_geometries_count);
        //     m_pDescriptor->setGeometryDescriptors(geometryDescriptors);
        //     geometryDescriptors->release();
        // }
    }  // namespace ngfx
}  // namespace ncore
