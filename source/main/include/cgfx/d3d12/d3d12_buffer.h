#ifndef __CGFX_D3D12_BUFFER_H__
#define __CGFX_D3D12_BUFFER_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
    #pragma once
#endif

#include "cgfx/gfx_buffer.h"
#include "cgfx/d3d12/d3d12_header.h"

namespace ncore
{
    namespace ngfx
    {
#ifdef TARGET_PC
        namespace nd3d12
        {
            struct buffer_t
            {
                D_GFX_OCS_COMPONENT_SET(enums::ComponentD3D12Buffer);
                ID3D12Resource*      m_pBuffer     = nullptr;
                D3D12MA::Allocation* m_pAllocation = nullptr;
                void*                m_pCpuAddress = nullptr;
                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            ngfx::buffer_t* CreateBuffer(device_t* pDevice, ngfx::buffer_t*);
            bool            Create(device_t* pDevice, ngfx::buffer_t*);
            void            Destroy(device_t*, ngfx::buffer_t*);
            void*           GetHandle(device_t* pDevice, ngfx::buffer_t*);
            void*           GetCpuAddress(device_t* pDevice, ngfx::buffer_t*);
            u64             GetGpuAddress(device_t* pDevice, ngfx::buffer_t*);
            u32             GetRequiredStagingBufferSize(device_t* pDevice, ngfx::buffer_t*);
        }  // namespace nd3d12
#else
        namespace nd3d12
        {
            ngfx::buffer_t* CreateBuffer(device_t* pDevice, ngfx::buffer_t* b) { return b; }
            bool            Create(device_t* pDevice, ngfx::buffer_t*) { return false; }
            void            Destroy(device_t*, ngfx::buffer_t*) {}
            void*           GetHandle(device_t* pDevice, ngfx::buffer_t*) { return nullptr; }
            void*           GetCpuAddress(device_t* pDevice, ngfx::buffer_t*) { return nullptr; }
            u64             GetGpuAddress(device_t* pDevice, ngfx::buffer_t*) { return 0; }
            u32             GetRequiredStagingBufferSize(device_t* pDevice, ngfx::buffer_t*) { return 0; }
        }  // namespace nd3d12
#endif

    }  // namespace ngfx
}  // namespace ncore

#endif
