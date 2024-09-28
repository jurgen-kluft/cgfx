#ifndef __CGFX_MOCK_DEVICE_H__
#define __CGFX_MOCK_DEVICE_H__

#include "cgfx/gfx_device.h"

namespace ncore
{
    namespace ngfx
    {
        class MockDevice : public IGfxDevice
        {
        public:
            MockDevice(const GfxDeviceDesc& desc);
            ~MockDevice();

            virtual bool  Create() override;
            virtual void* GetHandle() const override;
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
        };

    }  // namespace ngfx
}  // namespace ncore

#endif
