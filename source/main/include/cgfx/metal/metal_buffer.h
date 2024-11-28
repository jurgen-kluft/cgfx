#ifndef __CGFX_METAL_BUFFER_H__
#define __CGFX_METAL_BUFFER_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
    #pragma once
#endif

#include "cgfx/gfx_buffer.h"
#include "cgfx/metal/metal_utils.h"

namespace ncore
{
    namespace ngfx
    {
#ifdef TARGET_MAC
        namespace nmetal
        {
            struct mbuffer_t
            {
                D_GFX_OCS_COMPONENT_SET(enums::ComponentMetalBuffer);
                MTL::Buffer* m_pBuffer     = nullptr;
                void*        m_pCpuAddress = nullptr;
                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            void         CreateBuffer(ngfx::device_t* device, ngfx::buffer_t*);
            bool         Create(ngfx::device_t* device, ngfx::buffer_t*);
            void         Destroy(ngfx::device_t*, ngfx::buffer_t*);
            MTL::Buffer* GetHandle(ngfx::device_t* device, ngfx::buffer_t*);
            void*        GetCpuAddress(ngfx::device_t* device, ngfx::buffer_t*);
            u64          GetGpuAddress(ngfx::device_t* device, ngfx::buffer_t*);
            u32          GetRequiredStagingBufferSize(ngfx::device_t* device, ngfx::buffer_t*);
        }  // namespace nmetal
#else
        namespace nmetal
        {
            inline void  CreateBuffer(device_t* pDevice, ngfx::buffer_t* b) {}
            inline bool  Create(device_t* pDevice, ngfx::buffer_t*) { return false; }
            inline void  Destroy(device_t*, ngfx::buffer_t*) {}
            inline void* GetHandle(device_t* pDevice, ngfx::buffer_t*) { return nullptr; }
            inline void* GetCpuAddress(device_t* pDevice, ngfx::buffer_t*) { return nullptr; }
            inline u64   GetGpuAddress(device_t* pDevice, ngfx::buffer_t*) { return 0; }
            inline u32   GetRequiredStagingBufferSize(device_t* pDevice, ngfx::buffer_t*) { return 0; }
        }  // namespace nmetal
#endif
    }  // namespace ngfx
}  // namespace ncore

#endif
