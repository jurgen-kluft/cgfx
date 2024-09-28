#ifndef __CGFX_MOCK_BUFFER_H__
#define __CGFX_MOCK_BUFFER_H__

#include "cgfx/gfx_buffer.h"

namespace ncore
{
    namespace ngfx
    {

        class MockDevice;

        class MockBuffer : public IGfxBuffer
        {
        public:
            MockBuffer(MockDevice* pDevice, const GfxBufferDesc& desc, const eastl::string& name);
            ~MockBuffer();

            bool Create();

            virtual void* GetHandle() const override;
            virtual void* GetCpuAddress() override;
            virtual u64   GetGpuAddress() override;
            virtual u32   GetRequiredStagingBufferSize() const override;

        private:
            void* m_pCpuAddress = nullptr;
        };

    }  // namespace ngfx
}  // namespace ncore
#endif
