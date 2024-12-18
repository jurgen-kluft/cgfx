#ifndef __CGFX_D3D12_TEXTURE_H__
#define __CGFX_D3D12_TEXTURE_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
    #pragma once
#endif

#include "cbase/c_carray.h"
#include "cgfx/gfx_texture.h"
#include "cgfx/d3d12/d3d12_header.h"

namespace ncore
{
    namespace ngfx
    {
#ifdef TARGET_PC
        namespace nd3d12
        {
            struct texture_t
            {
                D_GFX_CS_COMPONENT_SET(enums::ComponentD3D12Texture);
                ID3D12Resource*      m_pTexture    = nullptr;
                D3D12MA::Allocation* m_pAllocation = nullptr;

                carray_t<D3D12Descriptor> m_RTV;
                carray_t<D3D12Descriptor> m_DSV;
                carray_t<D3D12Descriptor> m_readonlyDSV;

                HANDLE m_sharedHandle = 0;
                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            void                      CreateTexture(ngfx::device_t* device, ngfx::texture_t* texture, const texture_desc_t& desc);
            void                      DestroyTexture(ngfx::device_t* device, ngfx::texture_t* texture);
            bool                      Create(ngfx::device_t* device, ngfx::texture_t* texture);
            void                      Destroy(ngfx::device_t* device, ngfx::texture_t* texture);
            u32                       GetRequiredStagingBufferSize(ngfx::device_t* device, ngfx::texture_t* texture);
            u32                       GetRowPitch(ngfx::device_t* device, ngfx::texture_t* texture, u32 mip_level = 0);
            tiling_desc_t             GetTilingDesc(ngfx::device_t* device, ngfx::texture_t* texture);
            subresource_tiling_desc_t GetSubResourceTilingDesc(ngfx::device_t* device, ngfx::texture_t* texture, u32 subresource = 0);
            void*                     GetSharedHandle(ngfx::device_t* device, ngfx::texture_t* texture);

            D3D12_CPU_DESCRIPTOR_HANDLE GetRTV(ngfx::device_t* device, ngfx::texture_t* texture, u32 mip_slice, u32 array_slice);
            D3D12_CPU_DESCRIPTOR_HANDLE GetDSV(ngfx::device_t* device, ngfx::texture_t* texture, u32 mip_slice, u32 array_slice);
            D3D12_CPU_DESCRIPTOR_HANDLE GetReadOnlyDSV(ngfx::device_t* device, ngfx::texture_t* texture, u32 mip_slice, u32 array_slice);

        }  // namespace nd3d12
#else
        namespace nd3d12
        {
            inline void                      CreateTexture(ngfx::device_t* device, ngfx::texture_t* texture, const texture_desc_t& desc) { }
            inline void                      DestroyTexture(ngfx::device_t* device, ngfx::texture_t* texture) {}
            inline bool                      Create(ngfx::device_t* device, ngfx::texture_t* texture) { return false; }
            inline void                      Destroy(ngfx::device_t* device, ngfx::texture_t* texture) {}
            inline u32                       GetRequiredStagingBufferSize(ngfx::device_t* device, ngfx::texture_t* texture) { return 0; }
            inline u32                       GetRowPitch(ngfx::device_t* device, ngfx::texture_t* texture, u32 mip_level = 0) { return 0; }
            inline tiling_desc_t             GetTilingDesc(ngfx::device_t* device, ngfx::texture_t* texture) { return {}; }
            inline subresource_tiling_desc_t GetSubResourceTilingDesc(ngfx::device_t* device, ngfx::texture_t* texture, u32 subresource = 0) { return {}; }
            inline void*                     GetSharedHandle(ngfx::device_t* device, ngfx::texture_t* texture) { return nullptr; }
        }  // namespace nd3d12
#endif
        // class D3D12Device;
        // class D3D12Heap;

        // namespace D3D12MA
        // {
        //     class Allocation;
        // }

        // class D3D12Texture : public texture_t
        // {
        // public:
        //     D3D12Texture(D3D12Device* pDevice, const texture_desc_t& desc, const char* name);
        //     ~D3D12Texture();

        //     virtual void*                    GetHandle() const override { return m_pTexture; }
        //     virtual u32                      GetRequiredStagingBufferSize() const override;
        //     virtual u32                      GetRowPitch(u32 mip_level = 0) const override;
        //     virtual tiling_desc_t            GetTilingDesc() const override;
        //     virtual subresource_tiling_desc_t GetTilingDesc(u32 subresource = 0) const override;
        //     virtual void*                    GetSharedHandle() const { return m_sharedHandle; }

        //     bool                        Create();
        //     D3D12_CPU_DESCRIPTOR_HANDLE GetRTV(u32 mip_slice, u32 array_slice);
        //     D3D12_CPU_DESCRIPTOR_HANDLE GetDSV(u32 mip_slice, u32 array_slice);
        //     D3D12_CPU_DESCRIPTOR_HANDLE GetReadOnlyDSV(u32 mip_slice, u32 array_slice);

        // private:
        //     ID3D12Resource*      m_pTexture    = nullptr;
        //     D3D12MA::Allocation* m_pAllocation = nullptr;

        //     template<typename T>
        //     struct vector_t
        //     {
        // 		T* data = nullptr;
        // 		u32 size = 0;
        //         u32 capacity = 0;
        // 	};

        //     vector_t<D3D12Descriptor> m_RTV;
        //     vector_t<D3D12Descriptor> m_DSV;
        //     vector_t<D3D12Descriptor> m_readonlyDSV;

        //     HANDLE m_sharedHandle = 0;

        // private:
        //     friend class D3D12Swapchain;
        // };
    }  // namespace ngfx
}  // namespace ncore
#endif
