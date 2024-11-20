#ifndef __CGFX_METAL_DESCRIPTOR_H__
#define __CGFX_METAL_DESCRIPTOR_H__

#include "cgfx/metal/metal_utils.h"
#include "cgfx/gfx_descriptor.h"
#include "cgfx/gfx_buffer.h"

namespace ncore
{
    namespace ngfx
    {
        namespace nmetal
        {
            struct mbuffer_t;
            struct mtexture_t;

            struct srv_base_t
            {
                MTL::Texture* m_textureView;
                u32           m_heapIndex = GFX_INVALID_RESOURCE;
                srv_desc_t    m_desc      = {};
            };

            struct srv_texture_t
            {
                D_GFX_OCS_COMPONENT;
                nmetal::mtexture_t* m_texture;
                srv_base_t          m_base;
                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            struct srv_buffer_t
            {
                D_GFX_OCS_COMPONENT;
                nmetal::mbuffer_t* m_buffer;
                srv_base_t         m_base;
                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            struct uav_base_t
            {
                MTL::Texture*    m_textureView;
                u32              m_heapIndex = GFX_INVALID_RESOURCE;
                ngfx::uav_desc_t m_desc      = {};
            };

            struct uav_texture_t
            {
                D_GFX_OCS_COMPONENT;
                nmetal::mtexture_t* m_texture;
                uav_base_t          m_base;
                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            struct uav_buffer_t
            {
                D_GFX_OCS_COMPONENT;
                nmetal::mbuffer_t* m_buffer;
                uav_base_t         m_base;
                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            struct cbv_t
            {
                D_GFX_OCS_COMPONENT;
                nmetal::mbuffer_t* m_buffer;
                u32                m_heapIndex = GFX_INVALID_RESOURCE;
                ngfx::cbv_desc_t   m_desc      = {};
                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            struct sampler_t
            {
                D_GFX_OCS_COMPONENT;
                MTL::SamplerState*   m_pSampler  = nullptr;
                u32                  m_heapIndex = GFX_INVALID_RESOURCE;
                ngfx::sampler_desc_t m_desc;
                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            ngfx::descriptor_t* CreateSrv(ngfx::device_t* pDevice, resource_t* resource, descriptor_t* descriptor, texture_t* texture, const ngfx::srv_desc_t& desc);
            ngfx::descriptor_t* CreateSrv(ngfx::device_t* pDevice, resource_t* resource, descriptor_t* descriptor, buffer_t* buffer, const ngfx::srv_desc_t& desc);
            ngfx::descriptor_t* CreateUav(ngfx::device_t* pDevice, resource_t* resource, descriptor_t* descriptor, texture_t* buffer, const uav_desc_t& desc);
            ngfx::descriptor_t* CreateUav(ngfx::device_t* pDevice, resource_t* resource, descriptor_t* descriptor, buffer_t* buffer, const uav_desc_t& desc);
            ngfx::descriptor_t* CreateCbv(ngfx::device_t* pDevice, resource_t* resource, descriptor_t* descriptor, buffer_t* buffer, const cbv_desc_t& desc);
            ngfx::descriptor_t* CreateSampler(ngfx::device_t* pDevice, resource_t* resource, descriptor_t* descriptor, const sampler_desc_t& desc);

            void  Destroy(ngfx::device_t* pDevice, ngfx::descriptor_t* d);
            bool  Create(ngfx::device_t* pDevice, ngfx::descriptor_t* d);
            void* GetHandle(ngfx::device_t* pDevice, ngfx::descriptor_t* d);
            u32   GetHeapIndex(ngfx::device_t* pDevice, ngfx::descriptor_t* d);

        }  // namespace nmetal

        // class MetalDevice;

        // class MetalShaderResourceView : public descriptor_t
        // {
        // public:
        //     MetalShaderResourceView(MetalDevice* pDevice, resource_t* pResource, const srv_desc_t& desc, const char* name);
        //     ~MetalShaderResourceView();

        //     bool Create();

        //     virtual void* GetHandle() const override { return m_pResource->GetHandle(); }
        //     virtual u32   GetHeapIndex() const override { return m_heapIndex; }

        // private:
        //     resource_t*             m_pResource    = nullptr;
        //     srv_desc_t m_desc         = {};
        //     MTL::Texture*             m_pTextureView = nullptr;
        //     u32                       m_heapIndex    = GFX_INVALID_RESOURCE;
        // };

        // class MetalUnorderedAccessView : public descriptor_t
        // {
        // public:
        //     MetalUnorderedAccessView(MetalDevice* pDevice, resource_t* pResource, const uav_desc_t& desc, const char* name);
        //     ~MetalUnorderedAccessView();

        //     bool                              Create();
        //     const uav_desc_t& GetDesc() const { return m_desc; }

        //     virtual void* GetHandle() const override { return m_pResource->GetHandle(); }
        //     virtual u32   GetHeapIndex() const override { return m_heapIndex; }

        // private:
        //     resource_t*              m_pResource    = nullptr;
        //     uav_desc_t m_desc         = {};
        //     MTL::Texture*              m_pTextureView = nullptr;
        //     u32                        m_heapIndex    = GFX_INVALID_RESOURCE;
        // };

        // class MetalConstantBufferView : public descriptor_t
        // {
        // public:
        //     MetalConstantBufferView(MetalDevice* pDevice, buffer_t* buffer, const cbv_desc_t& desc, const char* name);
        //     ~MetalConstantBufferView();

        //     bool Create();

        //     virtual void* GetHandle() const override { return m_pBuffer->GetHandle(); }
        //     virtual u32   GetHeapIndex() const override { return m_heapIndex; }

        // private:
        //     buffer_t*               m_pBuffer   = nullptr;
        //     cbv_desc_t m_desc      = {};
        //     u32                       m_heapIndex = GFX_INVALID_RESOURCE;
        // };

        // class MetalSampler : public descriptor_t
        // {
        // public:
        //     MetalSampler(MetalDevice* pDevice, const sampler_desc_t& desc, const char* name);
        //     ~MetalSampler();

        //     bool Create();

        //     virtual void* GetHandle() const override { return m_pSampler; }
        //     virtual u32   GetHeapIndex() const override { return m_heapIndex; }

        // private:
        //     sampler_desc_t     m_desc;
        //     MTL::SamplerState* m_pSampler  = nullptr;
        //     u32                m_heapIndex = GFX_INVALID_RESOURCE;
        // };

    }  // namespace ngfx
}  // namespace ncore
#endif
