#ifndef __CGFX_MOCK_DESCRIPTOR_H__
#define __CGFX_MOCK_DESCRIPTOR_H__

#include "cgfx/gfx_descriptor.h"

namespace ncore
{
    namespace ngfx
    {
        namespace nmock
        {
            ngfx::descriptor_t* CreateSrv(ngfx::device_t* pDevice, descriptor_t* descriptor, texture_t* texture, const ngfx::srv_desc_t& desc);
            ngfx::descriptor_t* CreateSrv(ngfx::device_t* pDevice, descriptor_t* descriptor, buffer_t* buffer, const ngfx::srv_desc_t& desc);
            ngfx::descriptor_t* CreateSrv(ngfx::device_t* pDevice, descriptor_t* descriptor, tlas_t* tlas, const ngfx::srv_desc_t& desc);
            ngfx::descriptor_t* CreateUav(ngfx::device_t* pDevice, descriptor_t* descriptor, texture_t* buffer, const uav_desc_t& desc);
            ngfx::descriptor_t* CreateUav(ngfx::device_t* pDevice, descriptor_t* descriptor, buffer_t* buffer, const uav_desc_t& desc);
            ngfx::descriptor_t* CreateCbv(ngfx::device_t* pDevice, descriptor_t* descriptor, buffer_t* buffer, const cbv_desc_t& desc);
            ngfx::descriptor_t* CreateSampler(ngfx::device_t* pDevice, descriptor_t* descriptor, const sampler_desc_t& desc);

            void  Destroy(ngfx::device_t* pDevice, ngfx::descriptor_t* d);
            bool  Create(ngfx::device_t* pDevice, ngfx::descriptor_t* d);
            void* GetHandle(ngfx::device_t* pDevice, ngfx::descriptor_t* d);
            u32   GetHeapIndex(ngfx::device_t* pDevice, ngfx::descriptor_t* d);

        }  // namespace nmock
    }  // namespace ngfx
}  // namespace ncore

#endif
