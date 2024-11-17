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
            ngfx::blas_t* CreateRayTracingBLAS(ngfx::device_t* pDevice, ngfx::blas_t* pBLAS)
            {
                mblas_t* mblas            = AddAnotherComponent<ngfx::blas_t, mblas_t>(pDevice, pBLAS);
                mblas->m_geometries       = nullptr;
                mblas->m_geometries_count = 0;
                return pBLAS;
            }

            void Destroy(ngfx::device_t* pDevice, ngfx::blas_t* pBLAS)
            {
                mblas_t* mblas = GetOtherComponent<ngfx::blas_t, mblas_t>(pDevice, pBLAS);
                if (mblas)
                {
                    mblas->m_pAccelerationStructure->release();
                    mblas->m_pDescriptor->release();
                    mblas->m_pScratchBuffer->release();
                    for (u32 i = 0; i < mblas->m_geometries_count; ++i)
                    {
                        mblas->m_geometries[i]->release();
                    }
                }
            }

            bool Create(ngfx::device_t* pDevice, ngfx::blas_t* pBLAS)
            {
                // TODO, allocate memory for geometries
                // m_geometries.reserve(m_desc.geometry_counth);

                mblas_t* mblas = GetOtherComponent<ngfx::blas_t, mblas_t>(pDevice, pBLAS);
                for (u32 i = 0; i < pBLAS->m_desc.geometries_count; ++i)
                {
                    const rt_geometry_t& geometry = pBLAS->m_desc.geometries[i];

                    MTL::AccelerationStructureTriangleGeometryDescriptor* geometryDescriptor = MTL::AccelerationStructureTriangleGeometryDescriptor::alloc()->init();
                    geometryDescriptor->setOpaque(geometry.opaque);
                    geometryDescriptor->setVertexBuffer((MTL::Buffer*)GetHandle(pDevice, geometry.vertex_buffer));
                    geometryDescriptor->setVertexBufferOffset((NS::UInteger)geometry.vertex_buffer_offset);
                    geometryDescriptor->setVertexStride((NS::UInteger)geometry.vertex_stride);
                    geometryDescriptor->setVertexFormat(ToAttributeFormat(geometry.vertex_format));
                    geometryDescriptor->setIndexBuffer((MTL::Buffer*)GetHandle(pDevice, geometry.index_buffer));
                    geometryDescriptor->setIndexBufferOffset((NS::UInteger)geometry.index_buffer_offset);
                    geometryDescriptor->setIndexType(geometry.index_format == enums::FORMAT_R16UI ? MTL::IndexTypeUInt16 : MTL::IndexTypeUInt32);
                    geometryDescriptor->setTriangleCount((NS::UInteger)geometry.index_count / 3);

                    mblas->m_geometries[mblas->m_geometries_count++] = geometryDescriptor;
                }

                NS::Array* geometryDescriptors = NS::Array::alloc()->init((NS::Object**)mblas->m_geometries, (NS::UInteger)mblas->m_geometries_count);

                mblas->m_pDescriptor = MTL::PrimitiveAccelerationStructureDescriptor::alloc()->init();
                mblas->m_pDescriptor->setGeometryDescriptors(geometryDescriptors);
                mblas->m_pDescriptor->setUsage(ToAccelerationStructureUsage(pBLAS->m_desc.flags));

                nmetal::device_t*               mdevice   = GetOtherComponent<ngfx::device_t, nmetal::device_t>(pDevice, pDevice);
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

                MakeResident(pDevice, mblas->m_pAccelerationStructure);
                MakeResident(pDevice, mblas->m_pScratchBuffer);

                name_t const* name  = GetOtherComponent<ngfx::blas_t, name_t>(pDevice, pBLAS);
                NS::String*   label = NS::String::alloc()->init(name->m_name, NS::StringEncoding::UTF8StringEncoding);
                mblas->m_pAccelerationStructure->setLabel(label);
                label->release();

                return true;
            }

            void* GetHandle(ngfx::device_t* pDevice, ngfx::blas_t* pBLAS)
            {
                mblas_t* mblas = GetOtherComponent<ngfx::blas_t, mblas_t>(pDevice, pBLAS);
                return mblas->m_pAccelerationStructure;
            }

            void UpdateVertexBuffer(ngfx::device_t* pDevice, ngfx::blas_t* pBLAS, buffer_t* vertex_buffer, u32 vertex_buffer_offset)
            {
                ASSERT(pBLAS->m_desc.flags & enums::rt::AsFlagAllowUpdate);
                ASSERT(pBLAS->m_geometries_count == 1);  // todo : suppport more than 1

                mblas_t* mblas = GetOtherComponent<ngfx::blas_t, mblas_t>(pDevice, pBLAS);
                mblas->m_geometries[0]->setVertexBuffer((MTL::Buffer*)GetHandle(pDevice, vertex_buffer));
                mblas->m_geometries[0]->setVertexBufferOffset((NS::UInteger)vertex_buffer_offset);

                NS::Array* geometryDescriptors = NS::Array::alloc()->init((NS::Object**)mblas->m_geometries, (NS::UInteger)mblas->m_geometries_count);
                mblas->m_pDescriptor->setGeometryDescriptors(geometryDescriptors);
                geometryDescriptors->release();
            }
        }  // namespace nmetal
    }  // namespace ngfx
}  // namespace ncore
