#ifndef __CGFX_METAL_BUFFER_H__
#define __CGFX_METAL_BUFFER_H__

// #include "cgfx/metal/metal_utils.h"
#include "cgfx/gfx_buffer.h"

namespace ncore
{
    namespace ngfx
    {
        namespace nmetal
        {
            ngfx::buffer_t* CreateBuffer(ngfx::device_t* device, ngfx::buffer_t*);
            bool            Create(ngfx::device_t* device, ngfx::buffer_t*);
            void            Destroy(ngfx::device_t*, ngfx::buffer_t*);
            void*           GetHandle(ngfx::device_t* device, ngfx::buffer_t*);
            void*           GetCpuAddress(ngfx::device_t* device, ngfx::buffer_t*);
            u64             GetGpuAddress(ngfx::device_t* device, ngfx::buffer_t*);
            u32             GetRequiredStagingBufferSize(ngfx::device_t* device, ngfx::buffer_t*);
        }  // namespace nmetal

    }  // namespace ngfx
}  // namespace ncore

#endif
