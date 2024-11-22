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
        namespace nd3d12
        {
            struct fence_t
            {
                D_GFX_OCS_COMPONENT_SET(enums::ComponentD3D12Fence);
                ID3D12Fence* m_pFence = nullptr;
                HANDLE       m_hEvent = nullptr;
                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            fence_t* CreateFence(device_t* device, resource_t* resource, fence_t* fence);
            void     Destroy(device_t* device, fence_t* fence);
            void     Wait(device_t* device, fence_t* fence, u64 value);
            void     Signal(device_t* device, fence_t* fence, u64 value);
        }  // namespace nd3d12
    }  // namespace ngfx
}  // namespace ncore
#endif
