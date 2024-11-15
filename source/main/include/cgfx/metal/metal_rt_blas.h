#ifndef __CGFX_METAL_RT_BLAS_H__
#define __CGFX_METAL_RT_BLAS_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
    #pragma once
#endif

#include "cgfx/metal/metal_utils.h"
#include "cgfx/gfx_rt_blas.h"

namespace ncore
{
    namespace ngfx
    {

        class MetalDevice;

        class MetalRayTracingBLAS : public blas_t
        {
        public:
            MetalRayTracingBLAS(MetalDevice* pDevice, const GfxRayTracingBLASDesc& desc, const char* name);
            ~MetalRayTracingBLAS();

            bool                                           Create();
            void                                           UpdateVertexBuffer(buffer_t* vertex_buffer, u32 vertex_buffer_offset);
            MTL::AccelerationStructure*                    GetAccelerationStructure() const { return m_pAccelerationStructure; }
            MTL::PrimitiveAccelerationStructureDescriptor* GetDescriptor() const { return m_pDescriptor; }
            MTL::Buffer*                                   GetScratchBuffer() const { return m_pScratchBuffer; }

            virtual void* GetHandle() const override { return m_pAccelerationStructure; }

        private:
            MTL::AccelerationStructure*                    m_pAccelerationStructure = nullptr;
            MTL::PrimitiveAccelerationStructureDescriptor* m_pDescriptor            = nullptr;
            MTL::Buffer*                                   m_pScratchBuffer         = nullptr;

            MTL::AccelerationStructureTriangleGeometryDescriptor** m_geometries;
            u32                                                    m_geometries_count;
        };

    }  // namespace ngfx
}  // namespace ncore
#endif
