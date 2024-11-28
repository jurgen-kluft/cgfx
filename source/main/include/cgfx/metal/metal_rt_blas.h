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
#ifdef TARGET_MAC
        namespace nmetal
        {
            struct mblas_t
            {
                D_GFX_CS_COMPONENT_SET(enums::ComponentMetalBlas);
                MTL::AccelerationStructure*                            m_pAccelerationStructure = nullptr;
                MTL::PrimitiveAccelerationStructureDescriptor*         m_pDescriptor            = nullptr;
                MTL::Buffer*                                           m_pScratchBuffer         = nullptr;
                MTL::AccelerationStructureTriangleGeometryDescriptor** m_geometries;
                u32                                                    m_geometries_count;
                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            void                        CreateRayTracingBLAS(ngfx::device_t* pDevice, ngfx::blas_t* pBLAS);
            void                        Destroy(ngfx::device_t* pDevice, ngfx::blas_t* pBLAS);
            bool                        Create(ngfx::device_t* pDevice, ngfx::blas_t* pBLAS);
            MTL::AccelerationStructure* GetHandle(ngfx::device_t* pDevice, ngfx::blas_t const* pBLAS);
            void                        UpdateVertexBuffer(ngfx::device_t* pDevice, ngfx::blas_t* pBLAS, buffer_t* vertex_buffer, u32 vertex_buffer_offset);
        }  // namespace nmetal
#else
        namespace nmetal
        {
            void  CreateRayTracingBLAS(ngfx::device_t* pDevice, ngfx::blas_t* pBLAS) {}
            void  Destroy(ngfx::device_t* pDevice, ngfx::blas_t* pBLAS) {}
            bool  Create(ngfx::device_t* pDevice, ngfx::blas_t* pBLAS) { return false; }
            void* GetHandle(ngfx::device_t* pDevice, ngfx::blas_t const* pBLAS) { return nullptr; }
        }  // namespace nmetal
#endif
    }  // namespace ngfx
}  // namespace ncore

#endif
