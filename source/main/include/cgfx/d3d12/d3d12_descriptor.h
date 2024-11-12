#ifndef __CGFX_D3D12_DESCRIPTOR_H__
#define __CGFX_D3D12_DESCRIPTOR_H__

#include "cgfx/d3d12/d3d12_header.h"
#include "cgfx/gfx_descriptor.h"
#include "cgfx/gfx_buffer.h"

namespace ncore
{
    namespace ngfx
    {
        class D3D12Device;

        class D3D12ShaderResourceView : public IGfxDescriptor
        {
        public:
            D3D12ShaderResourceView(D3D12Device* pDevice, IGfxResource* pResource, const GfxShaderResourceViewDesc& desc, const char* name);
            ~D3D12ShaderResourceView();

            virtual void* GetHandle() const override { return m_pResource->GetHandle(); }
            virtual u32   GetHeapIndex() const override { return m_descriptor.index; }

            bool Create();

        private:
            IGfxResource*             m_pResource = nullptr;
            GfxShaderResourceViewDesc m_desc      = {};
            D3D12Descriptor           m_descriptor;
        };

        class D3D12UnorderedAccessView : public IGfxDescriptor
        {
        public:
            D3D12UnorderedAccessView(D3D12Device* pDevice, IGfxResource* pResource, const GfxUnorderedAccessViewDesc& desc, const char* name);
            ~D3D12UnorderedAccessView();

            virtual void* GetHandle() const override { return m_pResource->GetHandle(); }
            virtual u32   GetHeapIndex() const override { return m_descriptor.index; }

            bool            Create();
            D3D12Descriptor GetShaderVisibleDescriptor() const { return m_descriptor; }
            D3D12Descriptor GetNonShaderVisibleDescriptor() const { return m_nonShaderVisibleDescriptor; }

        private:
            IGfxResource*              m_pResource = nullptr;
            GfxUnorderedAccessViewDesc m_desc      = {};
            D3D12Descriptor            m_descriptor;

            D3D12Descriptor m_nonShaderVisibleDescriptor;  // for uav clear
        };

        class D3D12ConstantBufferView : public IGfxDescriptor
        {
        public:
            D3D12ConstantBufferView(D3D12Device* pDevice, IGfxBuffer* buffer, const GfxConstantBufferViewDesc& desc, const char* name);
            ~D3D12ConstantBufferView();

            virtual void* GetHandle() const override { return m_pBuffer->GetHandle(); }
            virtual u32   GetHeapIndex() const override { return m_descriptor.index; }

            bool Create();

        private:
            IGfxBuffer*               m_pBuffer = nullptr;
            GfxConstantBufferViewDesc m_desc    = {};
            D3D12Descriptor           m_descriptor;
        };

        class D3D12Sampler : public IGfxDescriptor
        {
        public:
            D3D12Sampler(D3D12Device* pDevice, const GfxSamplerDesc& desc, const char* name);
            ~D3D12Sampler();

            virtual void* GetHandle() const override { return nullptr; }
            virtual u32   GetHeapIndex() const override { return m_descriptor.index; }

            bool Create();

        private:
            GfxSamplerDesc  m_desc;
            D3D12Descriptor m_descriptor;
        };
    }  // namespace ngfx
}  // namespace ncore

#endif
