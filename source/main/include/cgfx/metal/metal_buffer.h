#ifndef __CGFX_METAL_BUFFER_H__
#define __CGFX_METAL_BUFFER_H__

#include "cgfx/metal/metal_utils.h"
#include "cgfx/gfx_buffer.h"

namespace ncore
{
    namespace ngfx
    {
        class MetalDevice;

        class MetalBuffer : public IGfxBuffer
        {
        public:
            MetalBuffer(MetalDevice* pDevice, const GfxBufferDesc& desc, const char* name);
            ~MetalBuffer();

            bool Create();

            virtual void* GetHandle() const override { return m_pBuffer; }
            virtual void* GetCpuAddress() override { return m_pCpuAddress; }
            virtual u64   GetGpuAddress() override;
            virtual u32   GetRequiredStagingBufferSize() const override;

        private:
            MTL::Buffer* m_pBuffer     = nullptr;
            void*        m_pCpuAddress = nullptr;
        };

    }  // namespace ngfx
}  // namespace ncore

#endif
