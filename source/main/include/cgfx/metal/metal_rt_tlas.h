#ifndef __CGFX_METAL_RT_TLAS_H__
#define __CGFX_METAL_RT_TLAS_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
    #pragma once
#endif

#include "cgfx/metal/metal_utils.h"
#include "cgfx/gfx_rt_tlas.h"

namespace ncore
{
    namespace ngfx
    {

        class MetalDevice;

        class MetalRayTracingTLAS : public IGfxRayTracingTLAS
        {
        public:
            MetalRayTracingTLAS(MetalDevice* pDevice, const GfxRayTracingTLASDesc& desc, const char* name);
            ~MetalRayTracingTLAS();

            bool                                          Create();
            void                                          UpdateInstance(const GfxRayTracingInstance* instances, u32 instance_count);
            MTL::AccelerationStructure*                   GetAccelerationStructure() const { return m_pAccelerationStructure; }
            MTL::InstanceAccelerationStructureDescriptor* GetDescriptor() const { return m_pDescriptor; }
            MTL::Buffer*                                  GetScratchBuffer() const { return m_pScratchBuffer; }
            MTL::Buffer*                                  GetGPUHeaderBuffer() const { return m_pGPUHeaderBuffer; }

            virtual void* GetHandle() const override { return m_pAccelerationStructure; }

        private:
            MTL::AccelerationStructure*                   m_pAccelerationStructure      = nullptr;
            MTL::InstanceAccelerationStructureDescriptor* m_pDescriptor                 = nullptr;
            MTL::Buffer*                                  m_pScratchBuffer              = nullptr;
            MTL::Buffer*                                  m_pInstanceBuffer             = nullptr;
            MTL::Buffer*                                  m_pGPUHeaderBuffer            = nullptr;
            u32                                           m_instanceBufferSize          = 0;
            u32                                           m_currentInstanceBufferOffset = 0;
            void*                                         m_instanceBufferCpuAddress    = nullptr;
        };

    }  // namespace ngfx
}  // namespace ncore
#endif
