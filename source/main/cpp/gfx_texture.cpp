#include "cgfx/gfx.h"

#include "cgfx/d3d12/d3d12_device.h"
#include "cgfx/d3d12/d3d12_texture.h"

#include "cgfx/metal/metal_device.h"
#include "cgfx/metal/metal_texture.h"

#include "cgfx/mock/mock_device.h"
#include "cgfx/mock/mock_texture.h"

#include "ccore/c_math.h"

namespace ncore
{
    namespace ngfx
    {
        u32 GetAllocationSize(const texture_desc_t& desc)
        {
            u32 size       = 0;
            u32 min_width  = GetFormatBlockWidth(enums::cast<enums::format>(desc.format));
            u32 min_height = GetFormatBlockHeight(enums::cast<enums::format>(desc.format));

            for (u32 layer = 0; layer < desc.array_size; ++layer)
            {
                for (u32 mip = 0; mip < desc.mip_levels; ++mip)
                {
                    u32 width  = math::g_max(desc.width >> mip, min_width);
                    u32 height = math::g_max(desc.height >> mip, min_height);
                    u32 depth  = math::g_max(desc.depth >> mip, 1u);

                    size += GetFormatRowPitch(enums::cast<enums::format>(desc.format), width) * height * depth;
                }
            }
            return size;
        }

        u32  CalcSubresource(const texture_desc_t& desc, u32 mip, u32 slice) { return mip + desc.mip_levels * slice; }
        void DecomposeSubresource(const texture_desc_t& desc, u32 subresource, u32& mip, u32& slice)
        {
            mip   = subresource % desc.mip_levels;
            slice = (subresource / desc.mip_levels) % desc.array_size;
        }

        texture_t* CreateTexture(device_t* device, const texture_desc_t& desc, const char* name)
        {
            resource_t* resource = CreateObject<resource_t>(device, name);
            texture_t*  texture  = AddComponent<resource_t, texture_t>(device, resource);
            texture->m_desc      = desc;
            switch (device->m_desc.backend)
            {
                case enums::Backend_D3D12: return nd3d12::CreateTexture(device, texture, desc);
                case enums::Backend_Metal: return nmetal::CreateTexture(device, texture, desc);
                case enums::Backend_Mock: return nmock::CreateTexture(device, texture, desc);
            }
            return texture;
        }

        bool Create(device_t* device, texture_t* resource)
        {
            switch (device->m_desc.backend)
            {
                case enums::Backend_D3D12: return nd3d12::Create(device, resource);
                case enums::Backend_Metal: return nmetal::Create(device, resource);
                case enums::Backend_Mock: return nmock::Create(device, resource);
            }
            return false;
        }

        void Destroy(device_t* device, texture_t* resource)
        {
            switch (device->m_desc.backend)
            {
                case enums::Backend_D3D12: nd3d12::Destroy(device, resource); break;
                case enums::Backend_Metal: nmetal::Destroy(device, resource); break;
                case enums::Backend_Mock: nmock::Destroy(device, resource); break;
            }
        }

        u32 GetAllocationSize(device_t* device, const texture_desc_t& desc) { return GetAllocationSize(desc); }

        u32 GetRequiredStagingBufferSize(device_t* device, texture_t* texture)
        {
            switch (device->m_desc.backend)
            {
                case enums::Backend_D3D12: return nd3d12::GetRequiredStagingBufferSize(device, texture);
                case enums::Backend_Metal: return nmetal::GetRequiredStagingBufferSize(device, texture);
                case enums::Backend_Mock: return nmock::GetRequiredStagingBufferSize(device, texture);
            }
            return 0;
        }

        u32 GetRowPitch(device_t* device, texture_t* texture, u32 mip_level)
        {
            switch (device->m_desc.backend)
            {
                case enums::Backend_D3D12: return nd3d12::GetRowPitch(device, texture, mip_level);
                case enums::Backend_Metal: return nmetal::GetRowPitch(device, texture, mip_level);
                case enums::Backend_Mock: return nmock::GetRowPitch(device, texture, mip_level);
            }
            return 0;
        }

        tiling_desc_t GetTilingDesc(device_t* device, texture_t* texture)
        {
            switch (device->m_desc.backend)
            {
                case enums::Backend_D3D12: return nd3d12::GetTilingDesc(device, texture);
                case enums::Backend_Metal: return nmetal::GetTilingDesc(device, texture);
                case enums::Backend_Mock: return nmock::GetTilingDesc(device, texture);
            }
            return {};
        }

        subresource_tiling_desc_t GetSubResourceTilingDesc(device_t* device, texture_t* texture, u32 subresource)
        {
            switch (device->m_desc.backend)
            {
                case enums::Backend_D3D12: return nd3d12::GetSubResourceTilingDesc(device, texture, subresource);
                case enums::Backend_Metal: return nmetal::GetSubResourceTilingDesc(device, texture, subresource);
                case enums::Backend_Mock: return nmock::GetSubResourceTilingDesc(device, texture, subresource);
            }
            return {};
        }

        void* GetSharedHandle(device_t* device, texture_t* texture)
        {
            switch (device->m_desc.backend)
            {
                case enums::Backend_D3D12: return nd3d12::GetSharedHandle(device, texture);
                case enums::Backend_Metal: return nmetal::GetSharedHandle(device, texture);
                case enums::Backend_Mock: return nmock::GetSharedHandle(device, texture);
            }
            return nullptr;
        }

    }  // namespace ngfx
}  // namespace ncore
