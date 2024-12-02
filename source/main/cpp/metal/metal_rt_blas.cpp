#include "cbase/c_integer.h"
#include "cgfx/metal/metal_rt_blas.h"
#include "cgfx/metal/metal_device.h"
#include "cgfx/metal/metal_buffer.h"

namespace ncore
{
    namespace ngfx
    {
        namespace nmetal
        {
            void CreateRayTracingBLAS(ngfx::device_t* device, ngfx::blas_t* blas) { mblas_t* mblas = CreateComponent<ngfx::blas_t, mblas_t>(device, blas); }

            void Destroy(ngfx::device_t* device, ngfx::blas_t* blas)
            {
                mblas_t* mblas = GetComponent<ngfx::blas_t, mblas_t>(device, blas);
                if (mblas)
                {
                    mblas->m_pAccelerationStructure->release();
                    mblas->m_pDescriptor->release();
                    mblas->m_pScratchBuffer->release();
                    for (u32 i = 0; i < mblas->m_geometries.size(); ++i)
                    {
                        mblas->m_geometries[i]->release();
                    }
                }
            }

            bool Create(ngfx::device_t* device, ngfx::blas_t* blas)
            {
                // TODO, allocate memory for geometries
                // m_geometries.reserve(m_desc.geometry_counth);

                mblas_t* mblas = GetComponent<ngfx::blas_t, mblas_t>(device, blas);
                for (u32 i = 0; i < blas->m_desc.geometries.size(); ++i)
                {
                    const rt_geometry_t* geometry = blas->m_desc.geometries.mArray[i];

                    nmetal::mbuffer_t* mvertexBuffer = GetComponent<ngfx::buffer_t, nmetal::mbuffer_t>(device, geometry->vertex_buffer);
                    nmetal::mbuffer_t* mindexBuffer  = GetComponent<ngfx::buffer_t, nmetal::mbuffer_t>(device, geometry->index_buffer);

                    MTL::AccelerationStructureTriangleGeometryDescriptor* geometryDescriptor = MTL::AccelerationStructureTriangleGeometryDescriptor::alloc()->init();
                    geometryDescriptor->setOpaque(geometry->opaque);
                    geometryDescriptor->setVertexBuffer(mvertexBuffer->m_pBuffer);
                    geometryDescriptor->setVertexBufferOffset((NS::UInteger)geometry->vertex_buffer_offset);
                    geometryDescriptor->setVertexStride((NS::UInteger)geometry->vertex_stride);
                    geometryDescriptor->setVertexFormat(ToAttributeFormat(geometry->vertex_format));
                    geometryDescriptor->setIndexBuffer(mindexBuffer->m_pBuffer);
                    geometryDescriptor->setIndexBufferOffset((NS::UInteger)geometry->index_buffer_offset);
                    geometryDescriptor->setIndexType(geometry->index_format == enums::FORMAT_R16UI ? MTL::IndexTypeUInt16 : MTL::IndexTypeUInt32);
                    geometryDescriptor->setTriangleCount((NS::UInteger)geometry->index_count / 3);

                    mblas->m_geometries[mblas->m_geometries.mSize++] = geometryDescriptor;
                }

                NS::Array* geometryDescriptors = NS::Array::alloc()->init((NS::Object**)mblas->m_geometries.mArray, (NS::UInteger)mblas->m_geometries.size());

                mblas->m_pDescriptor = MTL::PrimitiveAccelerationStructureDescriptor::alloc()->init();
                mblas->m_pDescriptor->setGeometryDescriptors(geometryDescriptors);
                mblas->m_pDescriptor->setUsage(ToAccelerationStructureUsage(blas->m_desc.flags));

                nmetal::device_t*               mdevice   = GetComponent<ngfx::device_t, nmetal::device_t>(device, device);
                MTL::Device*                    mtlDevice = mdevice->m_pDevice;
                MTL::AccelerationStructureSizes asSizes   = mtlDevice->accelerationStructureSizes(mblas->m_pDescriptor);

                mblas->m_pAccelerationStructure = mtlDevice->newAccelerationStructure(asSizes.accelerationStructureSize);
                mblas->m_pScratchBuffer         = mtlDevice->newBuffer(math::g_max(asSizes.buildScratchBufferSize, asSizes.refitScratchBufferSize), MTL::ResourceStorageModePrivate);

                geometryDescriptors->release();

                if (mblas->m_pAccelerationStructure == nullptr || mblas->m_pScratchBuffer == nullptr)
                {
                    // RE_ERROR("[MetalRayTracingBLAS] failed to create : {}", m_name);
                    return false;
                }

                MakeResident(mdevice, mblas->m_pAccelerationStructure);
                MakeResident(mdevice, mblas->m_pScratchBuffer);

                name_t const* name  = GetComponent<ngfx::blas_t, name_t>(device, blas);
                NS::String*   label = NS::String::alloc()->init(name->m_name, NS::StringEncoding::UTF8StringEncoding);
                mblas->m_pAccelerationStructure->setLabel(label);
                label->release();

                return true;
            }

            MTL::AccelerationStructure* GetHandle(ngfx::device_t* device, ngfx::blas_t const* blas)
            {
                mblas_t* mblas = GetComponent<ngfx::blas_t, mblas_t>(device, blas);
                return mblas->m_pAccelerationStructure;
            }

            void UpdateVertexBuffer(ngfx::device_t* device, ngfx::blas_t* blas, buffer_t* vertex_buffer, u32 vertex_buffer_offset)
            {
                ASSERT(blas->m_desc.flags & enums::rt::AsFlagAllowUpdate);

                nmetal::mbuffer_t* mvertexBuffer = GetComponent<ngfx::buffer_t, nmetal::mbuffer_t>(device, vertex_buffer);
                mblas_t*           mblas         = GetComponent<ngfx::blas_t, mblas_t>(device, blas);
                ASSERT(mblas->m_geometries.size() == 1);  // todo : suppport more than 1
                mblas->m_geometries.mArray[0]->setVertexBuffer(mvertexBuffer->m_pBuffer);
                mblas->m_geometries.mArray[0]->setVertexBufferOffset((NS::UInteger)vertex_buffer_offset);

                NS::Array* geometryDescriptors = NS::Array::alloc()->init((NS::Object**)mblas->m_geometries.mArray, (NS::UInteger)mblas->m_geometries.size());
                mblas->m_pDescriptor->setGeometryDescriptors(geometryDescriptors);
                geometryDescriptors->release();
            }
        }  // namespace nmetal
    }  // namespace ngfx
}  // namespace ncore
