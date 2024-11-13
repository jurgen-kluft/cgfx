#ifndef __CGFX_D3D12_RT_BLAS_H__
#define __CGFX_D3D12_RT_BLAS_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
    #pragma once
#endif

#include "cgfx/d3d12/d3d12_header.h"
#include "cgfx/gfx_rt_blas.h"

namespace ncore
{
    namespace ngfx
    {
        class D3D12Device;

        namespace D3D12MA
        {
            class Allocation;
        }

        class D3D12RayTracingBLAS : public IGfxRayTracingBLAS
        {
        public:
            D3D12RayTracingBLAS(D3D12Device* pDevice, const GfxRayTracingBLASDesc& desc, const char* name);
            ~D3D12RayTracingBLAS();

            virtual void* GetHandle() const override { return m_pASBuffer; }

            bool                                                      Create();
            const D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC* GetBuildDesc() const { return &m_buildDesc; }
            D3D12_GPU_VIRTUAL_ADDRESS                                 GetGpuAddress() const { return m_pASBuffer->GetGPUVirtualAddress(); }

            void GetUpdateDesc(D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC& desc, D3D12_RAYTRACING_GEOMETRY_DESC& geometry, IGfxBuffer* vertex_buffer, u32 vertex_buffer_offset);

        private:
            template<typename T>
            struct vector_t
            {
				T* data = nullptr;
				u32 size = 0;
				u32 capacity = 0;
			};

            vector_t<D3D12_RAYTRACING_GEOMETRY_DESC>      m_geometries;
            D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC m_buildDesc;

            ID3D12Resource*      m_pASBuffer          = nullptr;
            D3D12MA::Allocation* m_pASAllocation      = nullptr;
            ID3D12Resource*      m_pScratchBuffer     = nullptr;
            D3D12MA::Allocation* m_pScratchAllocation = nullptr;
        };
    }  // namespace ngfx
}  // namespace ncore
#endif
