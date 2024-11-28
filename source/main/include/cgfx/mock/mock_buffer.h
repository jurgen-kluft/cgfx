#ifndef __CGFX_MOCK_BUFFER_H__
#define __CGFX_MOCK_BUFFER_H__

#include "cgfx/gfx_buffer.h"

namespace ncore
{
    namespace ngfx
    {
        namespace nmock
        {
            struct buffer_t
            {
                D_GFX_CS_COMPONENT_SET(enums::ComponentMockBuffer);
                void* m_pCpuAddress = nullptr;
                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };
            
            ngfx::buffer_t* CreateBuffer(device_t* pDevice, ngfx::buffer_t*);
            bool            Create(device_t* pDevice, ngfx::buffer_t*);
            void            Destroy(device_t*, ngfx::buffer_t*);
            void*           GetHandle(device_t* pDevice, ngfx::buffer_t*);
            void*           GetCpuAddress(device_t* pDevice, ngfx::buffer_t*);
            u64             GetGpuAddress(device_t* pDevice, ngfx::buffer_t*);
            u32             GetRequiredStagingBufferSize(device_t* pDevice, ngfx::buffer_t*);
        }  // namespace nmock
    }  // namespace ngfx
}  // namespace ncore
#endif
