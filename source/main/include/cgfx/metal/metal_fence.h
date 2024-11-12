#ifndef __CGFX_METAL_FENCE_H__
#define __CGFX_METAL_FENCE_H__

#include "cgfx/metal/metal_utils.h"
#include "cgfx/gfx_fence.h"

namespace ncore
{
    namespace ngfx
    {

        class MetalDevice;

        class MetalFence : public IGfxFence
        {
        public:
            MetalFence(MetalDevice* pDevice, const char* name);
            ~MetalFence();

            bool Create();

            virtual void* GetHandle() const override { return m_pEvent; }
            virtual void  Wait(u64 value) override;
            virtual void  Signal(u64 value) override;

        private:
            MTL::SharedEvent* m_pEvent = nullptr;
        };

    }  // namespace ngfx
}  // namespace ncore
#endif
