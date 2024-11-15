#ifndef __CGFX_D3D12_DEVICE_H__
#define __CGFX_D3D12_DEVICE_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
    #pragma once
#endif

//#include "cgfx/d3d12/d3d12_header.h"
#include "cgfx/gfx_device.h"

namespace ncore
{
    namespace ngfx
    {
        namespace nd3d12
        {
            bool                    Create(device_t* device);
            void                    Destroy(device_t* device);
            void*                   GetHandle(device_t* device);
            void                    BeginFrame(device_t* device);
            void                    EndFrame(device_t* device);

            bool                    DumpMemoryStats(device_t* device, const char* file);
        }

//         namespace D3D12MA
//         {
//             class Allocator;
//             class Allocation;
//         }  // namespace D3D12MA

//         class D3D12DescriptorAllocator
//         {
//         public:
//             D3D12DescriptorAllocator(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE type, bool shader_visible, u32 descriptor_count, const char* name);
//             ~D3D12DescriptorAllocator();

//             D3D12Descriptor Allocate();
//             void            Free(const D3D12Descriptor& descriptor);

//             ID3D12DescriptorHeap* GetHeap() const { return m_pHeap; }
//             D3D12Descriptor       GetDescriptor(u32 index) const;

//         private:
//             ID3D12DescriptorHeap* m_pHeap           = nullptr;
//             u32                   m_descriptorSize  = 0;
//             u32                   m_descirptorCount = 0;
//             u32                   m_allocatedCount  = 0;
//             bool                  m_bShaderVisible  = false;

//             u32              m_freeDescriptorSize;
//             u32              m_freeDescriptorMax;
//             D3D12Descriptor* m_freeDescriptors;
//         };

//         class D3D12Device;

//         class D3D12ConstantBufferAllocator
//         {
//         public:
//             D3D12ConstantBufferAllocator(D3D12Device* device, u32 buffer_size, const char* name);

//             void Allocate(u32 size, void** cpu_address, u64* gpu_address);
//             void Reset();

//         private:
//             buffer_t* m_pBuffer       = nullptr;
//             u32                           m_allocatedSize = 0;
//         };

//         class D3D12Device : public device_t
//         {
//         public:
//             D3D12Device(const device_desc_t& desc);
//             ~D3D12Device();

//             virtual bool  Create() override;
//             virtual void* GetHandle() const override { return m_pDevice; }
//             virtual void  BeginFrame() override;
//             virtual void  EndFrame() override;

//             virtual swapchain_t*      CreateSwapchain(const swapchain_desc_t& desc, const char* name) override;
//             virtual command_list_t*    CreateCommandList(GfxCommandQueue queue_type, const char* name) override;
//             virtual fence_t*          CreateFence(const char* name) override;
//             virtual heap_t*           CreateHeap(const heap_desc_t& desc, const char* name) override;
//             virtual buffer_t*         CreateBuffer(const buffer_desc_t& desc, const char* name) override;
//             virtual texture_t*        CreateTexture(const texture_desc_t& desc, const char* name) override;
//             virtual shader_t*         CreateShader(const shader_desc_t& desc, byte* data_ptr, u32 data_len, const char* name) override;
//             virtual pipeline_state_t*  CreateGraphicsPipelineState(const graphics_pipeline_desc_t& desc, const char* name) override;
//             virtual pipeline_state_t*  CreateMeshShadingPipelineState(const mesh_shading_pipeline_desc_t& desc, const char* name) override;
//             virtual pipeline_state_t*  CreateComputePipelineState(const compute_pipeline_desc_t& desc, const char* name) override;
//             virtual descriptor_t*     CreateShaderResourceView(resource_t* resource, const srv_desc_t& desc, const char* name) override;
//             virtual descriptor_t*     CreateUnorderedAccessView(resource_t* resource, const uav_desc_t& desc, const char* name) override;
//             virtual descriptor_t*     CreateConstantBufferView(buffer_t* buffer, const cbv_desc_t& desc, const char* name) override;
//             virtual descriptor_t*     CreateSampler(const sampler_desc_t& desc, const char* name) override;
//             virtual blas_t* CreateRayTracingBLAS(const GfxRayTracing::BLASDesc& desc, const char* name) override;
//             virtual tlas_t* CreateRayTracingTLAS(const GfxRayTracing::TLASDesc& desc, const char* name) override;

//             virtual u32  GetAllocationSize(const texture_desc_t& desc) override;
//             virtual bool DumpMemoryStats(const char* file) override;

//             IDXGIFactory5*          GetDxgiFactory() const { return m_pDxgiFactory; }
//             ID3D12CommandQueue*     GetGraphicsQueue() const { return m_pGraphicsQueue; }
//             ID3D12CommandQueue*     GetComputeQueue() const { return m_pComputeQueue; }
//             ID3D12CommandQueue*     GetCopyQueue() const { return m_pCopyQueue; }
//             D3D12MA::Allocator*     GetResourceAllocator() const { return m_pResourceAllocator; }
//             ID3D12DescriptorHeap*   GetResourceDescriptorHeap() const { return m_pResDescriptorAllocator->GetHeap(); }
//             ID3D12DescriptorHeap*   GetSamplerDescriptorHeap() const { return m_pSamplerAllocator->GetHeap(); }
//             ID3D12RootSignature*    GetRootSignature() const { return m_pRootSignature; }
//             ID3D12CommandSignature* GetDrawSignature() const { return m_pDrawSignature; }
//             ID3D12CommandSignature* GetDrawIndexedSignature() const { return m_pDrawIndexedSignature; }
//             ID3D12CommandSignature* GetDispatchSignature() const { return m_pDispatchSignature; }
//             ID3D12CommandSignature* GetDispatchMeshSignature() const { return m_pDispatchMeshSignature; }
//             ID3D12CommandSignature* GetMultiDrawSignature() const { return m_pMultiDrawSignature; }
//             ID3D12CommandSignature* GetMultiDrawIndexedSignature() const { return m_pMultiDrawIndexedSignature; }
//             ID3D12CommandSignature* GetMultiDispatchSignature() const { return m_pMultiDispatchSignature; }
//             ID3D12CommandSignature* GetMultiDispatchMeshSignature() const { return m_pMultiDispatchMeshSignature; }

//             D3D12_GPU_VIRTUAL_ADDRESS AllocateConstantBuffer(const void* data, size_t data_size);

//             void FlushDeferredDeletions();
//             void Delete(IUnknown* object);
//             void Delete(D3D12MA::Allocation* allocation);

//             D3D12Descriptor AllocateRTV();
//             D3D12Descriptor AllocateDSV();
//             D3D12Descriptor AllocateResourceDescriptor();
//             D3D12Descriptor AllocateSampler();
//             D3D12Descriptor AllocateNonShaderVisibleUAV();
//             void            DeleteRTV(const D3D12Descriptor& descriptor);
//             void            DeleteDSV(const D3D12Descriptor& descriptor);
//             void            DeleteResourceDescriptor(const D3D12Descriptor& descriptor);
//             void            DeleteSampler(const D3D12Descriptor& descriptor);
//             void            DeleteNonShaderVisibleUAV(const D3D12Descriptor& descriptor);

// #if MICROPROFILE_GPU_TIMERS_D3D12
//             int GetProfileGraphicsQueue() const { return m_nProfileGraphicsQueue; }
//             int GetProfileComputeQueue() const { return m_nProfileComputeQueue; }
//             int GetProfileCopyQueue() const { return m_nProfileCopyQueue; }
// #endif

//             bool IsSteamDeck() const { return m_bSteamDeck; }

//         private:
//             void DoDeferredDeletion(bool force_delete = false);
//             void CreateRootSignature();

//             void CreateIndirectCommandSignatures();

//         private:
//             CD3DX12FeatureSupport m_featureSupport;

//             IDXGIFactory6* m_pDxgiFactory = nullptr;
//             IDXGIAdapter1* m_pDxgiAdapter = nullptr;

//             ID3D12Device10*      m_pDevice        = nullptr;
//             ID3D12CommandQueue*  m_pGraphicsQueue = nullptr;
//             ID3D12CommandQueue*  m_pComputeQueue  = nullptr;
//             ID3D12CommandQueue*  m_pCopyQueue     = nullptr;
//             ID3D12RootSignature* m_pRootSignature = nullptr;

//             ID3D12CommandSignature* m_pDrawSignature              = nullptr;
//             ID3D12CommandSignature* m_pDrawIndexedSignature       = nullptr;
//             ID3D12CommandSignature* m_pDispatchSignature          = nullptr;
//             ID3D12CommandSignature* m_pDispatchMeshSignature      = nullptr;
//             ID3D12CommandSignature* m_pMultiDrawSignature         = nullptr;
//             ID3D12CommandSignature* m_pMultiDrawIndexedSignature  = nullptr;
//             ID3D12CommandSignature* m_pMultiDispatchSignature     = nullptr;
//             ID3D12CommandSignature* m_pMultiDispatchMeshSignature = nullptr;

//             D3D12MA::Allocator*                             m_pResourceAllocator = nullptr;
//             D3D12ConstantBufferAllocator* m_pConstantBufferAllocators[GFX_MAX_INFLIGHT_FRAMES];
//             D3D12DescriptorAllocator*     m_pRTVAllocator;
//             D3D12DescriptorAllocator*     m_pDSVAllocator;
//             D3D12DescriptorAllocator*     m_pResDescriptorAllocator;
//             D3D12DescriptorAllocator*     m_pSamplerAllocator;
//             D3D12DescriptorAllocator*     m_pNonShaderVisibleUavAllocator;

//             template<typename T>
//             struct queue_t
//             {
//                 u32 m_queueSize;
//                 u32 m_queueMax;
//                 T*  m_queue;
//             };

//             struct ObjectDeletion
//             {
//                 IUnknown* object;
//                 u64       frame;
//             };
//             queue_t<ObjectDeletion> m_deletionQueue;

//             struct AllocationDeletion
//             {
//                 D3D12MA::Allocation* allocation;
//                 u64                  frame;
//             };
//             queue_t<AllocationDeletion> m_allocationDeletionQueue;

//             struct DescriptorDeletion
//             {
//                 D3D12Descriptor descriptor;
//                 u64             frame;
//             };
//             queue_t<DescriptorDeletion> m_rtvDeletionQueue;
//             queue_t<DescriptorDeletion> m_dsvDeletionQueue;
//             queue_t<DescriptorDeletion> m_resourceDeletionQueue;
//             queue_t<DescriptorDeletion> m_samplerDeletionQueue;
//             queue_t<DescriptorDeletion> m_nonShaderVisibleUAVDeletionQueue;

// #if MICROPROFILE_GPU_TIMERS_D3D12
//             int m_nProfileGraphicsQueue = -1;
//             int m_nProfileComputeQueue  = -1;
//             int m_nProfileCopyQueue     = -1;
// #endif

//             bool m_bSteamDeck = false;
//         };

    }  // namespace ngfx
}  // namespace ncore

#endif
