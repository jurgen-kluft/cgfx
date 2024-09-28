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

        class MetalShaderResourceView : public IGfxDescriptor
        {
        public:
            MetalShaderResourceView(MetalDevice* pDevice, IGfxResource* pResource, const GfxShaderResourceViewDesc& desc, const eastl::string& name);
            ~MetalShaderResourceView();

            bool Create();

            virtual void* GetHandle() const override { return m_pResource->GetHandle(); }
            virtual u32   GetHeapIndex() const override { return m_heapIndex; }

        private:
            IGfxResource*             m_pResource    = nullptr;
            GfxShaderResourceViewDesc m_desc         = {};
            MTL::Texture*             m_pTextureView = nullptr;
            u32                       m_heapIndex    = GFX_INVALID_RESOURCE;
        };

        class MetalUnorderedAccessView : public IGfxDescriptor
        {
        public:
            MetalUnorderedAccessView(MetalDevice* pDevice, IGfxResource* pResource, const GfxUnorderedAccessViewDesc& desc, const eastl::string& name);
            ~MetalUnorderedAccessView();

            bool                              Create();
            const GfxUnorderedAccessViewDesc& GetDesc() const { return m_desc; }

            virtual void* GetHandle() const override { return m_pResource->GetHandle(); }
            virtual u32   GetHeapIndex() const override { return m_heapIndex; }

        private:
            IGfxResource*              m_pResource    = nullptr;
            GfxUnorderedAccessViewDesc m_desc         = {};
            MTL::Texture*              m_pTextureView = nullptr;
            u32                        m_heapIndex    = GFX_INVALID_RESOURCE;
        };

        class MetalConstantBufferView : public IGfxDescriptor
        {
        public:
            MetalConstantBufferView(MetalDevice* pDevice, IGfxBuffer* buffer, const GfxConstantBufferViewDesc& desc, const eastl::string& name);
            ~MetalConstantBufferView();

            bool Create();

            virtual void* GetHandle() const override { return m_pBuffer->GetHandle(); }
            virtual u32   GetHeapIndex() const override { return m_heapIndex; }

        private:
            IGfxBuffer*               m_pBuffer   = nullptr;
            GfxConstantBufferViewDesc m_desc      = {};
            u32                       m_heapIndex = GFX_INVALID_RESOURCE;
        };

        class MetalSampler : public IGfxDescriptor
        {
        public:
            MetalSampler(MetalDevice* pDevice, const GfxSamplerDesc& desc, const eastl::string& name);
            ~MetalSampler();

            bool Create();

            virtual void* GetHandle() const override { return m_pSampler; }
            virtual u32   GetHeapIndex() const override { return m_heapIndex; }

        private:
            GfxSamplerDesc     m_desc;
            MTL::SamplerState* m_pSampler  = nullptr;
            u32                m_heapIndex = GFX_INVALID_RESOURCE;
        };

    }  // namespace ngfx
}  // namespace ncore
#endif
