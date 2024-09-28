#ifndef __CGFX_MOCK_FENCE_H__
#define __CGFX_MOCK_FENCE_H__

#include "cgfx/gfx_fence.h"

namespace ncore
{
    namespace ngfx
    {
        class MockDevice;

        class MockFence : public IGfxFence
        {
        public:
            MockFence(MockDevice* pDevice, const eastl::string& name);
            ~MockFence();

            bool Create();

            virtual void* GetHandle() const override;
            virtual void  Wait(u64 value) override;
            virtual void  Signal(u64 value) override;
        };

    }  // namespace ngfx
}  // namespace ncore
#endif
