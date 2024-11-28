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
                D_GFX_CS_COMPONENT_SET(enums::ComponentD3D12Buffer);
                ID3D12Resource*      m_pBuffer     = nullptr;
                D3D12MA::Allocation* m_pAllocation = nullptr;
                void*                m_pCpuAddress = nullptr;
                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            void  CreateBuffer(ngfx::device_t* device, ngfx::buffer_t* buffer);
            void  DestroyBuffer(ngfx::device_t* device, ngfx::buffer_t* buffer);
            bool  Create(ngfx::device_t* device, ngfx::buffer_t* buffer);
            void  Destroy(ngfx::device_t* device, ngfx::buffer_t* buffer);
            void* GetCpuAddress(ngfx::device_t* device, ngfx::buffer_t* buffer);
            u64   GetGpuAddress(ngfx::device_t* device, ngfx::buffer_t* buffer);
            u32   GetRequiredStagingBufferSize(ngfx::device_t* device, ngfx::buffer_t* buffer);
        }  // namespace nd3d12
#else
        namespace nd3d12
        {
            void  CreateBuffer(device_t* device, ngfx::buffer_t* buffer) {}
            void  DestroyBuffer(ngfx::device_t* device, ngfx::buffer_t* buffer) {}
            bool  Create(device_t* device, ngfx::buffer_t* buffer) { return false; }
            void  Destroy(device_t* device, ngfx::buffer_t* buffer) {}
            void* GetCpuAddress(device_t* device, ngfx::buffer_t* buffer) { return nullptr; }
            u64   GetGpuAddress(device_t* device, ngfx::buffer_t* buffer) { return 0; }
            u32   GetRequiredStagingBufferSize(device_t* device, ngfx::buffer_t* buffer) { return 0; }
        }  // namespace nd3d12
#endif

    }  // namespace ngfx
}  // namespace ncore

#endif
