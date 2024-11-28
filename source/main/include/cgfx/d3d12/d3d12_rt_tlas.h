#ifndef __CGFX_D3D12_RT_TLAS_H__
#define __CGFX_D3D12_RT_TLAS_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
    #pragma once
#endif

#include "cbase/c_carray.h"
#include "cgfx/gfx_defines.h"
#include "cgfx/gfx_rt_tlas.h"
#include "cgfx/d3d12/d3d12_header.h"

namespace ncore
{
    namespace ngfx
    {
#ifdef TARGET_PC
        namespace nd3d12
        {
            struct tlas_t
            {
                D_GFX_CS_COMPONENT_SET(enums::ComponentD3D12Tlas);
                ID3D12Resource*      m_pASBuffer          = nullptr;
                D3D12MA::Allocation* m_pASAllocation      = nullptr;
                ID3D12Resource*      m_pScratchBuffer     = nullptr;
                D3D12MA::Allocation* m_pScratchAllocation = nullptr;

                ID3D12Resource*      m_pInstanceBuffer              = nullptr;
                D3D12MA::Allocation* m_pInstanceAllocation          = nullptr;
                void*                m_pInstanceBufferCpuAddress    = nullptr;
                u32                  m_nInstanceBufferSize          = 0;
                u32                  m_nCurrentInstanceBufferOffset = 0;
                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            void                      CreateRayTracingTLAS(ngfx::device_t* device, ngfx::tlas_t* tlas);
            void                      DestroyRayTracingTLAS(ngfx::device_t* device, ngfx::tlas_t* tlas);
            void                      Destroy(ngfx::device_t* device, ngfx::tlas_t* tlas);
            bool                      Create(ngfx::device_t* device, ngfx::tlas_t* tlas);
            void*                     GetHandle(ngfx::device_t* device, ngfx::tlas_t* tlas);
        }  // namespace nd3d12
#else
        namespace nd3d12
        {
            inline void  CreateRayTracingTLAS(ngfx::device_t* device, ngfx::tlas_t* tlas) {}
            inline void  DestroyRayTracingTLAS(ngfx::device_t* device, ngfx::tlas_t* tlas) {}
            inline void  Destroy(ngfx::device_t* device, ngfx::tlas_t* tlas) {}
            inline bool  Create(ngfx::device_t* device, ngfx::tlas_t* tlas) { return false; }
            inline void* GetHandle(ngfx::device_t* device, ngfx::tlas_t* tlas) { return nullptr; }
        }  // namespace nd3d12
#endif
    }  // namespace ngfx
}  // namespace ncore

#endif
