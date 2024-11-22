#ifndef __CGFX_MOCK_DEVICE_H__
#define __CGFX_MOCK_DEVICE_H__

#include "cgfx/gfx_device.h"

namespace ncore
{
    namespace ngfx
    {
        namespace nmock
        {
            void  CreateDevice(ngfx::device_t* device);
            bool  Create(ngfx::device_t* device);
            void  Destroy(ngfx::device_t* device);
            void* GetHandle(ngfx::device_t* device);
            void  BeginFrame(ngfx::device_t* device);
            void  EndFrame(ngfx::device_t* device);

            bool DumpMemoryStats(ngfx::device_t* device, const char* file);
        }  // namespace nmock
    }  // namespace ngfx
}  // namespace ncore

#endif
