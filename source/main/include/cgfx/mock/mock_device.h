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

            virtual IGfxSwapchain*      CreateSwapchain(const GfxSwapchainDesc& desc, const char* name) override;
            virtual IGfxCommandList*    CreateCommandList(GfxCommandQueue queue_type, const char* name) override;
            virtual IGfxFence*          CreateFence(const char* name) override;
            virtual IGfxHeap*           CreateHeap(const GfxHeapDesc& desc, const char* name) override;
            virtual IGfxBuffer*         CreateBuffer(const GfxBufferDesc& desc, const char* name) override;
            virtual IGfxTexture*        CreateTexture(const GfxTextureDesc& desc, const char* name) override;
            virtual IGfxShader*         CreateShader(const GfxShaderDesc& desc, byte* data_ptr, u32 data_len, const char* name) override;
            virtual IGfxPipelineState*  CreateGraphicsPipelineState(const GfxGraphicsPipelineDesc& desc, const char* name) override;
            virtual IGfxPipelineState*  CreateMeshShadingPipelineState(const GfxMeshShadingPipelineDesc& desc, const char* name) override;
            virtual IGfxPipelineState*  CreateComputePipelineState(const GfxComputePipelineDesc& desc, const char* name) override;
            virtual IGfxDescriptor*     CreateShaderResourceView(IGfxResource* resource, const GfxShaderResourceViewDesc& desc, const char* name) override;
            virtual IGfxDescriptor*     CreateUnorderedAccessView(IGfxResource* resource, const GfxUnorderedAccessViewDesc& desc, const char* name) override;
            virtual IGfxDescriptor*     CreateConstantBufferView(IGfxBuffer* buffer, const GfxConstantBufferViewDesc& desc, const char* name) override;
            virtual IGfxDescriptor*     CreateSampler(const GfxSamplerDesc& desc, const char* name) override;
            virtual IGfxRayTracingBLAS* CreateRayTracingBLAS(const GfxRayTracingBLASDesc& desc, const char* name) override;
            virtual IGfxRayTracingTLAS* CreateRayTracingTLAS(const GfxRayTracingTLASDesc& desc, const char* name) override;

            virtual u32  GetAllocationSize(const GfxTextureDesc& desc) override;
            virtual bool DumpMemoryStats(const char* file) override;
        };

    }  // namespace ngfx
}  // namespace ncore

#endif
