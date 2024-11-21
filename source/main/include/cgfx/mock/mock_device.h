#ifndef __CGFX_MOCK_DEVICE_H__
#define __CGFX_MOCK_DEVICE_H__

#include "cgfx/gfx_device.h"

namespace ncore
{
    namespace ngfx
    {
        namespace nmock
        {
            bool  Create(device_t* device);
            void  Destroy(device_t* device);
            void* GetHandle(device_t* device);
            void  BeginFrame(device_t* device);
            void  EndFrame(device_t* device);

            bool DumpMemoryStats(device_t* device, const char* file);
        }  // namespace nmock
    }  // namespace ngfx
}  // namespace ncore

#endif
