#ifndef __CGFX_D3D12_RT_TLAS_H__
#define __CGFX_D3D12_RT_TLAS_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
    #pragma once
#endif

#include "cgfx/d3d12/d3d12_header.h"
#include "cgfx/gfx_rt_tlas.h"

namespace ncore
{
    namespace ngfx
    {
        class D3D12Device;

        namespace D3D12MA
        {
            class Allocation;
        }

        class D3D12RayTracingTLAS : public IGfxRayTracingTLAS
        {
        public:
            D3D12RayTracingTLAS(D3D12Device* pDevice, const GfxRayTracingTLASDesc& desc, const char* name);
            ~D3D12RayTracingTLAS();

            virtual void*             GetHandle() const override { return m_pASBuffer; }
            D3D12_GPU_VIRTUAL_ADDRESS GetGpuAddress() const { return m_pASBuffer->GetGPUVirtualAddress(); }

            bool Create();
            void GetBuildDesc(D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC& desc, const GfxRayTracingInstance* instances, u32 instance_count);

        private:
            ID3D12Resource*      m_pASBuffer          = nullptr;
            D3D12MA::Allocation* m_pASAllocation      = nullptr;
            ID3D12Resource*      m_pScratchBuffer     = nullptr;
            D3D12MA::Allocation* m_pScratchAllocation = nullptr;

            ID3D12Resource*      m_pInstanceBuffer              = nullptr;
            D3D12MA::Allocation* m_pInstanceAllocation          = nullptr;
            void*                m_pInstanceBufferCpuAddress    = nullptr;
            u32                  m_nInstanceBufferSize          = 0;
            u32                  m_nCurrentInstanceBufferOffset = 0;
        };
    }  // namespace ngfx
}  // namespace ncore
#endif
