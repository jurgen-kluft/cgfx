#ifndef __CGFX_METAL_DESCRIPTOR_H__
#define __CGFX_METAL_DESCRIPTOR_H__

#include "cgfx/metal/metal_utils.h"
#include "cgfx/gfx_descriptor.h"
#include "cgfx/gfx_buffer.h"

namespace ncore
{
    namespace ngfx
    {

        class MetalDevice;

        class MetalShaderResourceView : public descriptor_t
        {
        public:
            MetalShaderResourceView(MetalDevice* pDevice, resource_t* pResource, const srv_desc_t& desc, const char* name);
            ~MetalShaderResourceView();

            bool Create();

            virtual void* GetHandle() const override { return m_pResource->GetHandle(); }
            virtual u32   GetHeapIndex() const override { return m_heapIndex; }

        private:
            resource_t*             m_pResource    = nullptr;
            srv_desc_t m_desc         = {};
            MTL::Texture*             m_pTextureView = nullptr;
            u32                       m_heapIndex    = GFX_INVALID_RESOURCE;
        };

        class MetalUnorderedAccessView : public descriptor_t
        {
        public:
            MetalUnorderedAccessView(MetalDevice* pDevice, resource_t* pResource, const uav_desc_t& desc, const char* name);
            ~MetalUnorderedAccessView();

            bool                              Create();
            const uav_desc_t& GetDesc() const { return m_desc; }

            virtual void* GetHandle() const override { return m_pResource->GetHandle(); }
            virtual u32   GetHeapIndex() const override { return m_heapIndex; }

        private:
            resource_t*              m_pResource    = nullptr;
            uav_desc_t m_desc         = {};
            MTL::Texture*              m_pTextureView = nullptr;
            u32                        m_heapIndex    = GFX_INVALID_RESOURCE;
        };

        class MetalConstantBufferView : public descriptor_t
        {
        public:
            MetalConstantBufferView(MetalDevice* pDevice, buffer_t* buffer, const cbv_desc_t& desc, const char* name);
            ~MetalConstantBufferView();

            bool Create();

            virtual void* GetHandle() const override { return m_pBuffer->GetHandle(); }
            virtual u32   GetHeapIndex() const override { return m_heapIndex; }

        private:
            buffer_t*               m_pBuffer   = nullptr;
            cbv_desc_t m_desc      = {};
            u32                       m_heapIndex = GFX_INVALID_RESOURCE;
        };

        class MetalSampler : public descriptor_t
        {
        public:
            MetalSampler(MetalDevice* pDevice, const sampler_desc_t& desc, const char* name);
            ~MetalSampler();

            bool Create();

            virtual void* GetHandle() const override { return m_pSampler; }
            virtual u32   GetHeapIndex() const override { return m_heapIndex; }

        private:
            sampler_desc_t     m_desc;
            MTL::SamplerState* m_pSampler  = nullptr;
            u32                m_heapIndex = GFX_INVALID_RESOURCE;
        };

    }  // namespace ngfx
}  // namespace ncore
#endif
