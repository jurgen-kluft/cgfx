#ifndef __CGFX_METAL_BUFFER_H__
#define __CGFX_METAL_BUFFER_H__

#include "cgfx/gfx_buffer.h"
#include "cgfx/metal/metal_utils.h"

namespace ncore
{
    namespace ngfx
    {
#ifdef TARGET_MAC
        namespace nmetal
        {
            struct mbuffer_t
            {
                D_GFX_OCS_COMPONENT;
                MTL::Buffer* m_pBuffer     = nullptr;
                void*        m_pCpuAddress = nullptr;
                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            ngfx::buffer_t* CreateBuffer(ngfx::device_t* device, ngfx::buffer_t*);
            bool            Create(ngfx::device_t* device, ngfx::buffer_t*);
            void            Destroy(ngfx::device_t*, ngfx::buffer_t*);
            void*           GetHandle(ngfx::device_t* device, ngfx::buffer_t*);
            void*           GetCpuAddress(ngfx::device_t* device, ngfx::buffer_t*);
            u64             GetGpuAddress(ngfx::device_t* device, ngfx::buffer_t*);
            u32             GetRequiredStagingBufferSize(ngfx::device_t* device, ngfx::buffer_t*);
        }  // namespace nmetal
#else
        namespace nmetal
        {
            ngfx::buffer_t* CreateBuffer(device_t* pDevice, ngfx::buffer_t* b) { return b; }
            bool            Create(device_t* pDevice, ngfx::buffer_t*) { return false; }
            void            Destroy(device_t*, ngfx::buffer_t*) {}
            void*           GetHandle(device_t* pDevice, ngfx::buffer_t*) { return nullptr; }
            void*           GetCpuAddress(device_t* pDevice, ngfx::buffer_t*) { return nullptr; }
            u64             GetGpuAddress(device_t* pDevice, ngfx::buffer_t*) { return 0; }
            u32             GetRequiredStagingBufferSize(device_t* pDevice, ngfx::buffer_t*) { return 0; }
        }  // namespace nmetal
#endif
    }  // namespace ngfx
}  // namespace ncore

#endif
