#ifndef __CGFX_D3D12_RT_BLAS_H__
#define __CGFX_D3D12_RT_BLAS_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
    #pragma once
#endif

#include "cgfx/gfx_defines.h"
#include "cgfx/gfx_rt_blas.h"
#include "cgfx/d3d12/d3d12_header.h"

namespace ncore
{
    namespace ngfx
    {
        namespace nd3d12
        {
            struct blas_t
            {
                D_GFX_OCS_COMPONENT_SET(enums::ComponentD3D12Blas);
                carray_t<D3D12_RAYTRACING_GEOMETRY_DESC>           m_geometries;
                D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC m_buildDesc;

                ID3D12Resource*      m_pASBuffer          = nullptr;
                D3D12MA::Allocation* m_pASAllocation      = nullptr;
                ID3D12Resource*      m_pScratchBuffer     = nullptr;
                D3D12MA::Allocation* m_pScratchAllocation = nullptr;
                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            ngfx::blas_t* CreateRayTracingBLAS(ngfx::device_t* pDevice, ngfx::blas_t* pBLAS);
            void          Destroy(ngfx::device_t* pDevice, ngfx::blas_t* pBLAS);
            bool          Create(ngfx::device_t* pDevice, ngfx::blas_t* pBLAS);
            void*         GetHandle(ngfx::device_t* pDevice, const ngfx::blas_t* pBLAS);
        }  // namespace nd3d12
    }  // namespace ngfx
}  // namespace ncore

#endif
