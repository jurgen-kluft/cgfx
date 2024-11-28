#ifndef __CGFX_D3D12_DEVICE_H__
#define __CGFX_D3D12_DEVICE_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
    #pragma once
#endif

#include "cgfx/gfx_device.h"
#include "cgfx/d3d12/d3d12_header.h"

namespace ncore
{
    namespace ngfx
    {
#ifdef TARGET_PC
        namespace nd3d12
        {
            struct ConstantBufferAllocator;
            struct DescriptorAllocator;

            struct device_t
            {
                D_GFX_CS_COMPONENT_SET(enums::ComponentD3D12Device);
                CD3DX12FeatureSupport m_featureSupport;

                IDXGIFactory6* m_pDxgiFactory = nullptr;
                IDXGIAdapter1* m_pDxgiAdapter = nullptr;

                ID3D12Device12*      m_pDevice        = nullptr;
                ID3D12CommandQueue*  m_pGraphicsQueue = nullptr;
                ID3D12CommandQueue*  m_pComputeQueue  = nullptr;
                ID3D12CommandQueue*  m_pCopyQueue     = nullptr;
                ID3D12RootSignature* m_pRootSignature = nullptr;

                ID3D12CommandSignature* m_pDrawSignature              = nullptr;
                ID3D12CommandSignature* m_pDrawIndexedSignature       = nullptr;
                ID3D12CommandSignature* m_pDispatchSignature          = nullptr;
                ID3D12CommandSignature* m_pDispatchMeshSignature      = nullptr;
                ID3D12CommandSignature* m_pMultiDrawSignature         = nullptr;
                ID3D12CommandSignature* m_pMultiDrawIndexedSignature  = nullptr;
                ID3D12CommandSignature* m_pMultiDispatchSignature     = nullptr;
                ID3D12CommandSignature* m_pMultiDispatchMeshSignature = nullptr;

                D3D12MA::Allocator*      m_pResourceAllocator = nullptr;
                ConstantBufferAllocator* m_pConstantBufferAllocators[GFX_MAX_INFLIGHT_FRAMES];
                DescriptorAllocator*     m_pRTVAllocator;
                DescriptorAllocator*     m_pDSVAllocator;
                DescriptorAllocator*     m_pResDescriptorAllocator;
                DescriptorAllocator*     m_pSamplerAllocator;
                DescriptorAllocator*     m_pNonShaderVisibleUavAllocator;

                template <typename T>
                struct queue_t
                {
                    u32 m_queueSize;
                    u32 m_queueMax;
                    T*  m_queue;
                };

                struct ObjectDeletion
                {
                    IUnknown* object;
                    u64       frame;
                };
                queue_t<ObjectDeletion> m_deletionQueue;

                struct AllocationDeletion
                {
                    D3D12MA::Allocation* allocation;
                    u64                  frame;
                };
                queue_t<AllocationDeletion> m_allocationDeletionQueue;

                struct DescriptorDeletion
                {
                    D3D12Descriptor descriptor;
                    u64             frame;
                };
                queue_t<DescriptorDeletion> m_rtvDeletionQueue;
                queue_t<DescriptorDeletion> m_dsvDeletionQueue;
                queue_t<DescriptorDeletion> m_resourceDeletionQueue;
                queue_t<DescriptorDeletion> m_samplerDeletionQueue;
                queue_t<DescriptorDeletion> m_nonShaderVisibleUAVDeletionQueue;

                s32 m_nProfileGraphicsQueue = -1;
                s32 m_nProfileComputeQueue  = -1;
                s32 m_nProfileCopyQueue     = -1;

                bool m_bSteamDeck = false;

                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            void  CreateDevice(ngfx::device_t* device, u32 max_instances) {}
            bool  Create(ngfx::device_t* device);
            void  Destroy(ngfx::device_t* device);
            void* GetHandle(ngfx::device_t* device);
            void  BeginFrame(ngfx::device_t* device);
            void  EndFrame(ngfx::device_t* device);

            D3D12_GPU_VIRTUAL_ADDRESS AllocateConstantBuffer(nd3d12::device_t* dxdevice, const void* data, size_t data_size);
            void                      FlushDeferredDeletions(nd3d12::device_t* dxdevice);
            void                      Delete(nd3d12::device_t* dxdevice, IUnknown* object);
            void                      Delete(nd3d12::device_t* dxdevice, D3D12MA::Allocation* allocation);
            D3D12Descriptor           AllocateRTV(nd3d12::device_t* dxdevice);
            D3D12Descriptor           AllocateDSV(nd3d12::device_t* dxdevice);
            D3D12Descriptor           AllocateResourceDescriptor(nd3d12::device_t* dxdevice);
            D3D12Descriptor           AllocateSampler(nd3d12::device_t* dxdevice);
            D3D12Descriptor           AllocateNonShaderVisibleUAV(nd3d12::device_t* dxdevice);
            void                      DeleteRTV(nd3d12::device_t* dxdevice, const D3D12Descriptor& descriptor);
            void                      DeleteDSV(nd3d12::device_t* dxdevice, const D3D12Descriptor& descriptor);
            void                      DeleteResourceDescriptor(nd3d12::device_t* dxdevice, const D3D12Descriptor& descriptor);
            void                      DeleteSampler(nd3d12::device_t* dxdevice, const D3D12Descriptor& descriptor);
            void                      DeleteNonShaderVisibleUAV(nd3d12::device_t* dxdevice, const D3D12Descriptor& descriptor);

            bool DumpMemoryStats(ngfx::device_t* device, const char* file);

            struct DescriptorAllocator
            {
                D3D12Descriptor       Allocate();
                void                  Free(const D3D12Descriptor& descriptor);
                ID3D12DescriptorHeap* GetHeap() const { return m_pHeap; }
                D3D12Descriptor       GetDescriptor(u32 index) const;

                ID3D12DescriptorHeap* m_pHeap           = nullptr;
                u32                   m_descriptorSize  = 0;
                u32                   m_descriptorCount = 0;
                u32                   m_allocatedCount  = 0;
                bool                  m_bShaderVisible  = false;
                u32                   m_freeDescriptorSize;
                u32                   m_freeDescriptorMax;
                D3D12Descriptor*      m_freeDescriptors;
            };

            void Setup(DescriptorAllocator* alloc, nd3d12::device_t* device, D3D12_DESCRIPTOR_HEAP_TYPE type, bool shader_visible, u32 descriptor_count, const char* name);
            void Teardown(DescriptorAllocator* alloc);

            struct ConstantBufferAllocator
            {
                void Allocate(u32 size, void** cpu_address, u64* gpu_address);
                void Reset();

                buffer_t* m_pBuffer       = nullptr;
                u32       m_allocatedSize = 0;
            };

            void Setup(ConstantBufferAllocator* alloc, nd3d12::device_t* device, u32 buffer_size, const char* name);
            void Teardown(ConstantBufferAllocator* alloc);

        }  // namespace nd3d12
#else
        namespace nd3d12
        {
            void  CreateDevice(ngfx::device_t* device, u32 max_instances) {}
            bool  Create(ngfx::device_t* device) { return false; }
            void  Destroy(ngfx::device_t* device) {}
            void* GetHandle(ngfx::device_t* device) { return nullptr; }
            void  BeginFrame(ngfx::device_t* device) {}
            void  EndFrame(ngfx::device_t* device) {}

            bool DumpMemoryStats(ngfx::device_t* device, const char* file) { return false; }
        }  // namespace nd3d12
#endif
    }  // namespace ngfx
}  // namespace ncore

#endif
