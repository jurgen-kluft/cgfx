#ifndef __CGFX_D3D12_FENCE_H__
#define __CGFX_D3D12_FENCE_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
    #pragma once
#endif

#include "cgfx/gfx_fence.h"
#include "cgfx/d3d12/d3d12_header.h"

namespace ncore
{
    namespace ngfx
    {
#ifdef TARGET_PC
        namespace nd3d12
        {
            struct fence_t
            {
                D_GFX_CS_COMPONENT_SET(enums::ComponentD3D12Fence);
                ID3D12Fence* m_pFence = nullptr;
                HANDLE       m_hEvent = nullptr;
                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            void CreateFence(ngfx::device_t* device, ngfx::fence_t* fence);
            void DestroyFence(ngfx::device_t* device, ngfx::fence_t* fence);
            void Destroy(ngfx::device_t* device, ngfx::fence_t* fence);
            void Wait(ngfx::device_t* device, ngfx::fence_t* fence, u64 value);
            void Signal(ngfx::device_t* device, ngfx::fence_t* fence, u64 value);
        }  // namespace nd3d12
#else
        namespace nd3d12
        {
            inline void CreateFence(ngfx::device_t* device, resource_t* resource, ngfx::fence_t* fence) {}
            inline void DestroyFence(ngfx::device_t* device, ngfx::fence_t* fence) {}
            inline void Destroy(ngfx::device_t* device, ngfx::fence_t* fence) {}
            inline void Wait(ngfx::device_t* device, ngfx::fence_t* fence, u64 value) {}
            inline void Signal(ngfx::device_t* device, ngfx::fence_t* fence, u64 value) {}
        }  // namespace nd3d12
#endif
    }  // namespace ngfx
}  // namespace ncore
#endif
