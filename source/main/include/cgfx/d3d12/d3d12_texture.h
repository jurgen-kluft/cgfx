#ifndef __CGFX_D3D12_TEXTURE_H__
#define __CGFX_D3D12_TEXTURE_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
    #pragma once
#endif

#include "cgfx/d3d12/d3d12_header.h"
#include "cgfx/gfx_texture.h"

namespace ncore
{
    namespace ngfx
    {
        class D3D12Device;
        class D3D12Heap;

        namespace D3D12MA
        {
            class Allocation;
        }

        class D3D12Texture : public texture_t
        {
        public:
            D3D12Texture(D3D12Device* pDevice, const texture_desc_t& desc, const char* name);
            ~D3D12Texture();

            virtual void*                    GetHandle() const override { return m_pTexture; }
            virtual u32                      GetRequiredStagingBufferSize() const override;
            virtual u32                      GetRowPitch(u32 mip_level = 0) const override;
            virtual GfxTilingDesc            GetTilingDesc() const override;
            virtual GfxSubresourceTilingDesc GetTilingDesc(u32 subresource = 0) const override;
            virtual void*                    GetSharedHandle() const { return m_sharedHandle; }

            bool                        Create();
            D3D12_CPU_DESCRIPTOR_HANDLE GetRTV(u32 mip_slice, u32 array_slice);
            D3D12_CPU_DESCRIPTOR_HANDLE GetDSV(u32 mip_slice, u32 array_slice);
            D3D12_CPU_DESCRIPTOR_HANDLE GetReadOnlyDSV(u32 mip_slice, u32 array_slice);

        private:
            ID3D12Resource*      m_pTexture    = nullptr;
            D3D12MA::Allocation* m_pAllocation = nullptr;

            template<typename T>
            struct vector_t
            {
				T* data = nullptr;
				u32 size = 0;
                u32 capacity = 0;
			};

            vector_t<D3D12Descriptor> m_RTV;
            vector_t<D3D12Descriptor> m_DSV;
            vector_t<D3D12Descriptor> m_readonlyDSV;

            HANDLE m_sharedHandle = 0;

        private:
            friend class D3D12Swapchain;
        };
    }  // namespace ngfx
}  // namespace ncore
#endif
