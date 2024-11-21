#ifndef __CGFX_D3D12_DESCRIPTOR_H__
#define __CGFX_D3D12_DESCRIPTOR_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
    #pragma once
#endif

// #include "cgfx/d3d12/d3d12_header.h"
#include "cgfx/gfx_descriptor.h"
#include "cgfx/gfx_buffer.h"

namespace ncore
{
    namespace ngfx
    {
        namespace nd3d12
        {
            ngfx::descriptor_t* CreateSrv(ngfx::device_t* pDevice, ngfx::descriptor_t* descriptor, texture_t* texture, const ngfx::srv_desc_t& desc);
            ngfx::descriptor_t* CreateSrv(ngfx::device_t* pDevice, ngfx::descriptor_t* descriptor, buffer_t* buffer, const ngfx::srv_desc_t& desc);
            ngfx::descriptor_t* CreateSrv(ngfx::device_t* pDevice, ngfx::descriptor_t* descriptor, tlas_t* tlas, const ngfx::srv_desc_t& desc);
            ngfx::descriptor_t* CreateUav(ngfx::device_t* pDevice, ngfx::descriptor_t* descriptor, texture_t* buffer, const uav_desc_t& desc);
            ngfx::descriptor_t* CreateUav(ngfx::device_t* pDevice, ngfx::descriptor_t* descriptor, buffer_t* buffer, const uav_desc_t& desc);
            ngfx::descriptor_t* CreateCbv(ngfx::device_t* pDevice, ngfx::descriptor_t* descriptor, buffer_t* buffer, const cbv_desc_t& desc);
            ngfx::descriptor_t* CreateSampler(ngfx::device_t* pDevice, ngfx::descriptor_t* descriptor, const sampler_desc_t& desc);

            void  Destroy(ngfx::device_t* pDevice, ngfx::descriptor_t* d);
            bool  Create(ngfx::device_t* pDevice, ngfx::descriptor_t* d);
            void* GetHandle(ngfx::device_t* pDevice, ngfx::descriptor_t* d);
            u32   GetHeapIndex(ngfx::device_t* pDevice, ngfx::descriptor_t* d);

        }  // namespace nd3d12

        // class D3D12Device;

        // class D3D12ShaderResourceView : public descriptor_t
        // {
        // public:
        //     D3D12ShaderResourceView(D3D12Device* pDevice, resource_t* pResource, const srv_desc_t& desc, const char* name);
        //     ~D3D12ShaderResourceView();

        //     virtual void* GetHandle() const override { return m_pResource->GetHandle(); }
        //     virtual u32   GetHeapIndex() const override { return m_descriptor.index; }

        //     bool Create();

        // private:
        //     resource_t*             m_pResource = nullptr;
        //     srv_desc_t m_desc      = {};
        //     D3D12Descriptor           m_descriptor;
        // };

        // class D3D12UnorderedAccessView : public descriptor_t
        // {
        // public:
        //     D3D12UnorderedAccessView(D3D12Device* pDevice, resource_t* pResource, const uav_desc_t& desc, const char* name);
        //     ~D3D12UnorderedAccessView();

        //     virtual void* GetHandle() const override { return m_pResource->GetHandle(); }
        //     virtual u32   GetHeapIndex() const override { return m_descriptor.index; }

        //     bool            Create();
        //     D3D12Descriptor GetShaderVisibleDescriptor() const { return m_descriptor; }
        //     D3D12Descriptor GetNonShaderVisibleDescriptor() const { return m_nonShaderVisibleDescriptor; }

        // private:
        //     resource_t*              m_pResource = nullptr;
        //     uav_desc_t m_desc      = {};
        //     D3D12Descriptor            m_descriptor;

        //     D3D12Descriptor m_nonShaderVisibleDescriptor;  // for uav clear
        // };

        // class D3D12ConstantBufferView : public descriptor_t
        // {
        // public:
        //     D3D12ConstantBufferView(D3D12Device* pDevice, buffer_t* buffer, const cbv_desc_t& desc, const char* name);
        //     ~D3D12ConstantBufferView();

        //     virtual void* GetHandle() const override { return m_pBuffer->GetHandle(); }
        //     virtual u32   GetHeapIndex() const override { return m_descriptor.index; }

        //     bool Create();

        // private:
        //     buffer_t*               m_pBuffer = nullptr;
        //     cbv_desc_t m_desc    = {};
        //     D3D12Descriptor           m_descriptor;
        // };

        // class D3D12Sampler : public descriptor_t
        // {
        // public:
        //     D3D12Sampler(D3D12Device* pDevice, const sampler_desc_t& desc, const char* name);
        //     ~D3D12Sampler();

        //     virtual void* GetHandle() const override { return nullptr; }
        //     virtual u32   GetHeapIndex() const override { return m_descriptor.index; }

        //     bool Create();

        // private:
        //     sampler_desc_t  m_desc;
        //     D3D12Descriptor m_descriptor;
        // };
    }  // namespace ngfx
}  // namespace ncore

#endif
