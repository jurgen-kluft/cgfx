#include "cgfx/mock/mock_descriptor.h"
#include "cgfx/mock/mock_device.h"

namespace ncore
{
    namespace ngfx
    {
        MockShaderResourceView::MockShaderResourceView(MockDevice* pDevice, IGfxResource* pResource, const GfxShaderResourceViewDesc& desc, const eastl::string& name)
        {
            m_pDevice   = pDevice;
            m_name      = name;
            m_pResource = pResource;
            m_desc      = desc;
        }

        MockShaderResourceView::~MockShaderResourceView() {}

        bool MockShaderResourceView::Create() { return true; }

        void* MockShaderResourceView::GetHandle() const { return nullptr; }

        u32 MockShaderResourceView::GetHeapIndex() const { return 0; }

        MockUnorderedAccessView::MockUnorderedAccessView(MockDevice* pDevice, IGfxResource* pResource, const GfxUnorderedAccessViewDesc& desc, const eastl::string& name)
        {
            m_pDevice   = pDevice;
            m_name      = name;
            m_pResource = pResource;
            m_desc      = desc;
        }

        MockUnorderedAccessView::~MockUnorderedAccessView() {}

        bool MockUnorderedAccessView::Create() { return true; }

        void* MockUnorderedAccessView::GetHandle() const { return nullptr; }

        u32 MockUnorderedAccessView::GetHeapIndex() const { return 0; }

        MockConstantBufferView::MockConstantBufferView(MockDevice* pDevice, IGfxBuffer* buffer, const GfxConstantBufferViewDesc& desc, const eastl::string& name)
        {
            m_pDevice = pDevice;
            m_name    = name;
            m_pBuffer = buffer;
            m_desc    = desc;
        }

        MockConstantBufferView::~MockConstantBufferView() {}

        bool MockConstantBufferView::Create() { return true; }

        void* MockConstantBufferView::GetHandle() const { return nullptr; }

        u32 MockConstantBufferView::GetHeapIndex() const { return 0; }

        MockSampler::MockSampler(MockDevice* pDevice, const GfxSamplerDesc& desc, const eastl::string& name)
        {
            m_pDevice = pDevice;
            m_name    = name;
            m_desc    = desc;
        }

        MockSampler::~MockSampler() {}

        bool MockSampler::Create() { return true; }

        void* MockSampler::GetHandle() const { return nullptr; }

        u32 MockSampler::GetHeapIndex() const { return 0; }

    }  // namespace ngfx
}  // namespace ncore
