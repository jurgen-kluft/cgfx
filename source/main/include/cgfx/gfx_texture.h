#ifndef __CGFX_TEXTURE_H__
#define __CGFX_TEXTURE_H__

#include "cgfx/gfx_resource.h"

namespace ncore
{
    namespace ngfx
    {
        namespace enums
        {
            typedef u8 texture_usage_t;
            enum texture_usage
            {
                TextureUsageNone            = 0,
                TextureUsageRenderTarget    = 1 << 0,
                TextureUsageDepthStencil    = 1 << 1,
                TextureUsageUnorderedAccess = 1 << 2,
                TextureUsageShared          = 1 << 3,
            };

            typedef u8 texture_type_t;
            enum texture_type
            {
                TextureType2D        = 0,
                TextureType2DArray   = 1,
                TextureType3D        = 2,
                TextureTypeCube      = 3,
                TextureTypeCubeArray = 4,
            };
        }  // namespace enums

        struct texture_desc_t
        {
            u32                    width       = 1;
            u32                    height      = 1;
            u32                    depth       = 1;
            u32                    mip_levels  = 1;
            u32                    array_size  = 1;
            enums::texture_type_t  type        = enums::TextureType2D;
            enums::format_t        format      = enums::FORMAT_UNKNOWN;
            enums::memory_t        memory_type = enums::MemoryGpuOnly;
            enums::allocation_t    alloc_type  = enums::AllocationPlaced;
            enums::texture_usage_t usage       = enums::TextureUsageNone;
            u32                    heap_offset = 0;
            heap_t*                heap        = nullptr;
        };

        inline bool operator==(const texture_desc_t& lhs, const texture_desc_t& rhs)
        {
            return lhs.width == rhs.width && lhs.height == rhs.height && lhs.depth == rhs.depth && lhs.mip_levels == rhs.mip_levels && lhs.array_size == rhs.array_size && lhs.type == rhs.type && lhs.format == rhs.format &&
                   lhs.memory_type == rhs.memory_type && lhs.alloc_type == rhs.alloc_type && lhs.usage == rhs.usage;
        }

        struct texture_t
        {
            D_GFX_OCS_COMPONENT;
            texture_desc_t m_desc = {};
        };

        texture_t*               CreateTexture(device_t* device, const texture_desc_t& desc, const char* name);
        bool                     Create(device_t* device, texture_t* resource);
        void                     Destroy(device_t* device, texture_t* resource);
        u32                      GetAllocationSize(device_t* device, const texture_desc_t& desc);
        u32                      GetRequiredStagingBufferSize(device_t* device, texture_t* texture);
        u32                      GetRowPitch(device_t* device, texture_t* texture, u32 mip_level = 0);
        GfxTilingDesc            GetTilingDesc(device_t* device, texture_t* texture);
        GfxSubresourceTilingDesc GetSubResourceTilingDesc(device_t* device, texture_t* texture, u32 subresource = 0);
        void*                    GetSharedHandle(device_t* device, texture_t* texture);

    }  // namespace ngfx
}  // namespace ncore

#endif
