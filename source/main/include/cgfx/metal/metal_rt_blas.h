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
        namespace nmetal
        {
            struct mblas_t
            {
                D_GFX_OCS_COMPONENT;
                MTL::AccelerationStructure*                            m_pAccelerationStructure = nullptr;
                MTL::PrimitiveAccelerationStructureDescriptor*         m_pDescriptor            = nullptr;
                MTL::Buffer*                                           m_pScratchBuffer         = nullptr;
                MTL::AccelerationStructureTriangleGeometryDescriptor** m_geometries;
                u32                                                    m_geometries_count;
            };

            ngfx::blas_t* CreateRayTracingBLAS(ngfx::device_t* pDevice, ngfx::blas_t* pBLAS);
            void          Destroy(ngfx::device_t* pDevice, ngfx::blas_t* pBLAS);
            bool          Create(ngfx::device_t* pDevice, ngfx::blas_t* pBLAS);
            void*         GetHandle(ngfx::device_t* pDevice, ngfx::blas_t const* pBLAS);
            void          UpdateVertexBuffer(ngfx::device_t* pDevice, ngfx::blas_t* pBLAS, buffer_t* vertex_buffer, u32 vertex_buffer_offset);
        }  // namespace nmetal
    }  // namespace ngfx
}  // namespace ncore
#endif
