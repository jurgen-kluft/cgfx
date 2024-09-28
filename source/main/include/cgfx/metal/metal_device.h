#ifndef __CGFX_METAL_DEVICE_H__
#define __CGFX_METAL_DEVICE_H__

#include "cgfx/metal/metal_utils.h"
#include "cgfx/gfx_device.h"

struct IRDescriptorTableEntry;

namespace ncore
{
    namespace ngfx
    {
        class MetalConstantBufferAllocator;
        class MetalDescriptorAllocator;
        class MetalDescriptorAllocator;

        class MetalDevice : public IGfxDevice
        {
        public:
            MetalDevice(const GfxDeviceDesc& desc);
            virtual ~MetalDevice();

            virtual bool  Create() override;
            virtual void* GetHandle() const override { return m_pDevice; }
            virtual void  BeginFrame() override;
            virtual void  EndFrame() override;

            virtual IGfxSwapchain*      CreateSwapchain(const GfxSwapchainDesc& desc, const eastl::string& name) override;
            virtual IGfxCommandList*    CreateCommandList(GfxCommandQueue queue_type, const eastl::string& name) override;
            virtual IGfxFence*          CreateFence(const eastl::string& name) override;
            virtual IGfxHeap*           CreateHeap(const GfxHeapDesc& desc, const eastl::string& name) override;
            virtual IGfxBuffer*         CreateBuffer(const GfxBufferDesc& desc, const eastl::string& name) override;
            virtual IGfxTexture*        CreateTexture(const GfxTextureDesc& desc, const eastl::string& name) override;
            virtual IGfxShader*         CreateShader(const GfxShaderDesc& desc, eastl::span<u8> data, const eastl::string& name) override;
            virtual IGfxPipelineState*  CreateGraphicsPipelineState(const GfxGraphicsPipelineDesc& desc, const eastl::string& name) override;
            virtual IGfxPipelineState*  CreateMeshShadingPipelineState(const GfxMeshShadingPipelineDesc& desc, const eastl::string& name) override;
            virtual IGfxPipelineState*  CreateComputePipelineState(const GfxComputePipelineDesc& desc, const eastl::string& name) override;
            virtual IGfxDescriptor*     CreateShaderResourceView(IGfxResource* resource, const GfxShaderResourceViewDesc& desc, const eastl::string& name) override;
            virtual IGfxDescriptor*     CreateUnorderedAccessView(IGfxResource* resource, const GfxUnorderedAccessViewDesc& desc, const eastl::string& name) override;
            virtual IGfxDescriptor*     CreateConstantBufferView(IGfxBuffer* buffer, const GfxConstantBufferViewDesc& desc, const eastl::string& name) override;
            virtual IGfxDescriptor*     CreateSampler(const GfxSamplerDesc& desc, const eastl::string& name) override;
            virtual IGfxRayTracingBLAS* CreateRayTracingBLAS(const GfxRayTracingBLASDesc& desc, const eastl::string& name) override;
            virtual IGfxRayTracingTLAS* CreateRayTracingTLAS(const GfxRayTracingTLASDesc& desc, const eastl::string& name) override;

            virtual u32  GetAllocationSize(const GfxTextureDesc& desc) override;
            virtual bool DumpMemoryStats(const eastl::string& file) override;

            MTL::CommandQueue* GetQueue() const { return m_pQueue; }

            u64 AllocateConstantBuffer(const void* data, size_t data_size);

            u32          AllocateResourceDescriptor(IRDescriptorTableEntry** descriptor);
            u32          AllocateSamplerDescriptor(IRDescriptorTableEntry** descriptor);
            void         FreeResourceDescriptor(u32 index);
            void         FreeSamplerDescriptor(u32 index);
            MTL::Buffer* GetResourceDescriptorBuffer() const;
            MTL::Buffer* GetSamplerDescriptorBuffer() const;

            void MakeResident(const MTL::Allocation* allocation);
            void Evict(const MTL::Allocation* allocation);

        private:
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

    }  // namespace ngfx
}  // namespace ncore
#endif
