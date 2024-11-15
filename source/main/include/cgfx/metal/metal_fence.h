#ifndef __CGFX_METAL_FENCE_H__
#define __CGFX_METAL_FENCE_H__

#include "cgfx/metal/metal_utils.h"
#include "cgfx/gfx_fence.h"

namespace ncore
{
    namespace ngfx
    {
        namespace nmetal
        {
            fence_t* CreateFence(device_t* device, resource_t* resource, fence_t* fence);
            void     Destroy(device_t* device, fence_t* fence);
            void     Wait(device_t* device, fence_t* fence, u64 value);
            void     Signal(device_t* device, fence_t* fence, u64 value);
        }  // namespace nmetal

        // class MetalDevice;

        // class MetalFence : public fence_t
        // {
        // public:
        //     MetalFence(MetalDevice* pDevice, const char* name);
        //     ~MetalFence();

        //     bool Create();

        //     virtual void* GetHandle() const override { return m_pEvent; }
        //     virtual void  Wait(u64 value) override;
        //     virtual void  Signal(u64 value) override;

        // private:
        //     MTL::SharedEvent* m_pEvent = nullptr;
        // };

    }  // namespace ngfx
}  // namespace ncore
#endif
