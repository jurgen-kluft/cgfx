#ifndef __CGFX_D3D12_BUFFER_H__
#define __CGFX_D3D12_BUFFER_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
    #pragma once
#endif

// #include "cgfx/d3d12/d3d12_header.h"
#include "cgfx/gfx_buffer.h"

namespace ncore
{
    namespace ngfx
    {
        // class D3D12Device;
        // class D3D12Heap;

        // namespace D3D12MA
        // {
        //     class Allocation;
        // }

        // class D3D12Buffer : public buffer_t
        // {
        // public:
        //     D3D12Buffer(D3D12Device* pDevice, const buffer_desc_t& desc, const char* name);
        //     ~D3D12Buffer();

        //     virtual void* GetHandle() const override { return m_pBuffer; }
        //     virtual void* GetCpuAddress() override;
        //     virtual u64   GetGpuAddress() override;
        //     virtual u32   GetRequiredStagingBufferSize() const override;

        //     bool Create();

        // private:
        //     ID3D12Resource*      m_pBuffer     = nullptr;
        //     D3D12MA::Allocation* m_pAllocation = nullptr;
        //     void*                m_pCpuAddress = nullptr;
        // };

        namespace nd3d12
        {
            ngfx::buffer_t* CreateBuffer(device_t* pDevice, ngfx::buffer_t*);
            bool            Create(device_t* pDevice, ngfx::buffer_t*);
            void            Destroy(device_t*, ngfx::buffer_t*);
            void*           GetHandle(device_t* pDevice, ngfx::buffer_t*);
            void*           GetCpuAddress(device_t* pDevice, ngfx::buffer_t*);
            u64             GetGpuAddress(device_t* pDevice, ngfx::buffer_t*);
            u32             GetRequiredStagingBufferSize(device_t* pDevice, ngfx::buffer_t*);
        }  // namespace nd3d12

    }  // namespace ngfx
}  // namespace ncore

#endif
