#ifndef __CGFX_BUFFER_H__
#define __CGFX_BUFFER_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
    #pragma once
#endif

#include "cgfx/gfx_resource.h"

namespace ncore
{
    namespace ngfx
    {
        struct buffer_desc_t
        {
            enums::format_t       format      = enums::FORMAT_UNKNOWN;
            enums::memory_t       memory_type = enums::MemoryGpuOnly;
            enums::allocation_t   alloc_type  = enums::AllocationPlaced;
            enums::buffer_usage_t usage       = enums::BufferUsageNone;
            u32                   stride      = 1;
            u32                   size        = 1;
            u32                   heap_offset = 0;
            heap_t*               heap        = nullptr;
        };

        inline bool operator==(const buffer_desc_t& lhs, const buffer_desc_t& rhs)
        {
            return lhs.stride == rhs.stride && lhs.size == rhs.size && lhs.format == rhs.format && lhs.memory_type == rhs.memory_type && lhs.alloc_type == rhs.alloc_type && lhs.usage == rhs.usage;
        }

        struct buffer_t
        {
            D_GFX_CS_COMPONENT_SET(enums::ComponentBuffer);
            buffer_desc_t m_desc = {};
            DCORE_CLASS_PLACEMENT_NEW_DELETE
        };

        buffer_t* CreateBuffer(device_t* device, const buffer_desc_t& desc, const char* name);
        void      Destroy(device_t* device, buffer_t* resource);
        bool      Create(device_t* pDevice, buffer_t* pBuffer);
        void      Destroy(device_t* pDevice, buffer_t* pBuffer);
        void*     GetCpuAddress(device_t* pDevice, buffer_t* buffer);
        u64       GetGpuAddress(device_t* pDevice, buffer_t* buffer);
        u32       GetRequiredStagingBufferSize(device_t* pDevice, buffer_t* buffer);

    }  // namespace ngfx
}  // namespace ncore

#endif
