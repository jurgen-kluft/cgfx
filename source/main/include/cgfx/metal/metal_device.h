#ifndef __CGFX_METAL_DEVICE_H__
#define __CGFX_METAL_DEVICE_H__

#include "cgfx/metal/metal_utils.h"
#include "cgfx/gfx_device.h"

struct IRDescriptorTableEntry;

namespace ncore
{
    namespace ngfx
    {
        namespace nmetal
        {
            class MetalConstantBufferAllocator;
            class MetalDescriptorAllocator;
            class MetalDescriptorAllocator;

            struct device_t
            {
                MTL::Device*       m_pDevice         = nullptr;
                MTL::CommandQueue* m_pQueue          = nullptr;
                MTL::ResidencySet* m_pResidencySet   = nullptr;
                bool               m_bResidencyDirty = false;

                MetalConstantBufferAllocator* m_pConstantBufferAllocators[GFX_MAX_INFLIGHT_FRAMES];
                MetalDescriptorAllocator*     m_pResDescriptorAllocator;
                MetalDescriptorAllocator*     m_pSamplerAllocator;

                struct pair_t
                {
                    u32 first;
                    u64 second;
                };

                // We may need a ring buffer or something of the sort
                s32     m_resDescriptorDeletionQueueCount;
                s32     m_samplerDescriptorDeletionQueueCount;
                pair_t* m_resDescriptorDeletionQueue;
                pair_t* m_samplerDescriptorDeletionQueue;
            };

            bool  Create(device_t* device);
            void  Destroy(device_t* device);
            void* GetHandle(device_t* device);
            void  BeginFrame(device_t* device);
            void  EndFrame(device_t* device);

            bool DumpMemoryStats(device_t* device, const char* file);
        }  // namespace nmetal

        // class MetalConstantBufferAllocator;
        // class MetalDescriptorAllocator;
        // class MetalDescriptorAllocator;

        // class MetalDevice : public device_t
        // {
        // public:
        //     MetalDevice(const device_desc_t& desc);
        //     virtual ~MetalDevice();

        //     virtual bool  Create() override;
        //     virtual void* GetHandle() const override { return m_pDevice; }
        //     virtual void  BeginFrame() override;
        //     virtual void  EndFrame() override;

        //     virtual swapchain_t*      CreateSwapchain(const swapchain_desc_t& desc, const char* name) override;
        //     virtual command_list_t*    CreateCommandList(GfxCommandQueue queue_type, const char* name) override;
        //     virtual fence_t*          CreateFence(const char* name) override;
        //     virtual heap_t*           CreateHeap(const heap_desc_t& desc, const char* name) override;
        //     virtual buffer_t*         CreateBuffer(const buffer_desc_t& desc, const char* name) override;
        //     virtual texture_t*        CreateTexture(const texture_desc_t& desc, const char* name) override;
        //     virtual shader_t*         CreateShader(const shader_desc_t& desc, byte* data_ptr, u32 data_len, const char* name) override;
        //     virtual pipeline_state_t*  CreateGraphicsPipelineState(const graphics_pipeline_desc_t& desc, const char* name) override;
        //     virtual pipeline_state_t*  CreateMeshShadingPipelineState(const mesh_shading_pipeline_desc_t& desc, const char* name) override;
        //     virtual pipeline_state_t*  CreateComputePipelineState(const compute_pipeline_desc_t& desc, const char* name) override;
        //     virtual descriptor_t*     CreateShaderResourceView(resource_t* resource, const srv_desc_t& desc, const char* name) override;
        //     virtual descriptor_t*     CreateUnorderedAccessView(resource_t* resource, const uav_desc_t& desc, const char* name) override;
        //     virtual descriptor_t*     CreateConstantBufferView(buffer_t* buffer, const cbv_desc_t& desc, const char* name) override;
        //     virtual descriptor_t*     CreateSampler(const sampler_desc_t& desc, const char* name) override;
        //     virtual blas_t* CreateRayTracingBLAS(const GfxRayTracingBLASDesc& desc, const char* name) override;
        //     virtual tlas_t* CreateRayTracingTLAS(const GfxRayTracingTLASDesc& desc, const char* name) override;

        //     virtual u32  GetAllocationSize(const texture_desc_t& desc) override;
        //     virtual bool DumpMemoryStats(const char* file) override;

        //     MTL::CommandQueue* GetQueue() const { return m_pQueue; }

        //     u64 AllocateConstantBuffer(const void* data, size_t data_size);

        //     u32          AllocateResourceDescriptor(IRDescriptorTableEntry** descriptor);
        //     u32          AllocateSamplerDescriptor(IRDescriptorTableEntry** descriptor);
        //     void         FreeResourceDescriptor(u32 index);
        //     void         FreeSamplerDescriptor(u32 index);
        //     MTL::Buffer* GetResourceDescriptorBuffer() const;
        //     MTL::Buffer* GetSamplerDescriptorBuffer() const;

        //     void MakeResident(const MTL::Allocation* allocation);
        //     void Evict(const MTL::Allocation* allocation);

        // private:
        //     MTL::Device*       m_pDevice         = nullptr;
        //     MTL::CommandQueue* m_pQueue          = nullptr;
        //     MTL::ResidencySet* m_pResidencySet   = nullptr;
        //     bool               m_bResidencyDirty = false;

        //     MetalConstantBufferAllocator* m_pConstantBufferAllocators[GFX_MAX_INFLIGHT_FRAMES];
        //     MetalDescriptorAllocator*     m_pResDescriptorAllocator;
        //     MetalDescriptorAllocator*     m_pSamplerAllocator;

        //     struct pair_t
        //     {
        //         u32 first;
        //         u64 second;
        //     };
        //     // We may need a ring buffer or something of the sort
        //     s32     m_resDescriptorDeletionQueueCount;
        //     s32     m_samplerDescriptorDeletionQueueCount;
        //     pair_t* m_resDescriptorDeletionQueue;
        //     pair_t* m_samplerDescriptorDeletionQueue;
        // };

    }  // namespace ngfx
}  // namespace ncore
#endif
