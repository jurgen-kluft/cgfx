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
#ifdef TARGET_MAC
        namespace nmetal
        {
            struct mtlas_t
            {
                D_GFX_CS_COMPONENT_SET(enums::ComponentMetalTlas);
                MTL::AccelerationStructure*                   m_pAccelerationStructure      = nullptr;
                MTL::InstanceAccelerationStructureDescriptor* m_pDescriptor                 = nullptr;
                MTL::Buffer*                                  m_pScratchBuffer              = nullptr;
                MTL::Buffer*                                  m_pInstanceBuffer             = nullptr;
                MTL::Buffer*                                  m_pGPUHeaderBuffer            = nullptr;
                u32                                           m_instanceBufferSize          = 0;
                u32                                           m_currentInstanceBufferOffset = 0;
                void*                                         m_instanceBufferCpuAddress    = nullptr;
                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            void                        CreateRayTracingTLAS(ngfx::device_t* device, ngfx::tlas_t* tlas);
            void                        Destroy(ngfx::device_t* device, ngfx::tlas_t* tlas);
            bool                        Create(ngfx::device_t* device, ngfx::tlas_t* tlas);
            void                        UpdateInstance(ngfx::device_t* device, ngfx::tlas_t* tlas, const rt_instance_t* instances, u32 instance_count);
        }  // namespace nmetal
#else
        namespace nmetal
        {
            inline void  CreateRayTracingTLAS(ngfx::device_t* device, ngfx::tlas_t* tlas) {}
            inline void  Destroy(ngfx::device_t* device, ngfx::tlas_t* tlas) {}
            inline bool  Create(ngfx::device_t* device, ngfx::tlas_t* tlas) { return false; }
            inline void  UpdateInstance(ngfx::device_t* device, ngfx::tlas_t* tlas, const rt_instance_t* instances, u32 instance_count) {}
        }  // namespace nmetal
#endif
    }  // namespace ngfx
}  // namespace ncore
#endif
