#ifndef __CGFX_METAL_BUFFER_H__
#define __CGFX_METAL_BUFFER_H__

// #include "cgfx/metal/metal_utils.h"
#include "cgfx/gfx_buffer.h"

namespace ncore
{
    namespace ngfx
    {
        // class MetalDevice;

        // class MetalBuffer : public buffer_t
        // {
        // public:
        //     MetalBuffer(MetalDevice* pDevice, const buffer_desc_t& desc, const char* name);
        //     ~MetalBuffer();

        //     bool Create();

        //     virtual void* GetHandle() const override { return m_pBuffer; }
        //     virtual void* GetCpuAddress() override { return m_pCpuAddress; }
        //     virtual u64   GetGpuAddress() override;
        //     virtual u32   GetRequiredStagingBufferSize() const override;

        // private:
        //     MTL::Buffer* m_pBuffer     = nullptr;
        //     void*        m_pCpuAddress = nullptr;
        // };
        namespace nmetal
        {
            ngfx::buffer_t* CreateBuffer(device_t* pDevice, ngfx::buffer_t*);
            bool            Create(device_t* pDevice, ngfx::buffer_t*);
            void            Destroy(device_t*, ngfx::buffer_t*);
            void*           GetHandle(device_t* pDevice, ngfx::buffer_t*);
            void*           GetCpuAddress(device_t* pDevice, ngfx::buffer_t*);
            u64             GetGpuAddress(device_t* pDevice, ngfx::buffer_t*);
            u32             GetRequiredStagingBufferSize(device_t* pDevice, ngfx::buffer_t*);
        }  // namespace nmetal

    }  // namespace ngfx
}  // namespace ncore

#endif
