#ifndef __CGFX_GFX_DEVICE_H__
#define __CGFX_GFX_DEVICE_H__

#include "cgfx/gfx_defines.h"

namespace ncore
{
    namespace ngfx
    {
        class IGfxResource;
        class IGfxBuffer;
        class IGfxTexture;
        class IGfxFence;
        class IGfxSwapchain;
        class IGfxCommandList;
        class IGfxShader;
        class IGfxPipelineState;
        class IGfxDescriptor;
        class IGfxHeap;
        class IGfxRayTracingBLAS;
        class IGfxRayTracingTLAS;

        class IGfxDevice
        {
        public:
            virtual ~IGfxDevice() {}

            const GfxDeviceDesc& GetDesc() const { return m_desc; }
            u64                  GetFrameID() const { return m_frameID; }
            GfxVendor            GetVendor() const { return m_vendor; }

            virtual bool  Create()          = 0;
            virtual void* GetHandle() const = 0;
            virtual void  BeginFrame()      = 0;
            virtual void  EndFrame()        = 0;

            virtual IGfxSwapchain*      CreateSwapchain(const GfxSwapchainDesc& desc, const char* name)                                             = 0;
            virtual IGfxCommandList*    CreateCommandList(GfxCommandQueue queue_type, const char* name)                                             = 0;
            virtual IGfxFence*          CreateFence(const char* name)                                                                               = 0;
            virtual IGfxHeap*           CreateHeap(const GfxHeapDesc& desc, const char* name)                                                       = 0;
            virtual IGfxBuffer*         CreateBuffer(const GfxBufferDesc& desc, const char* name)                                                   = 0;
            virtual IGfxTexture*        CreateTexture(const GfxTextureDesc& desc, const char* name)                                                 = 0;
            virtual IGfxShader*         CreateShader(const GfxShaderDesc& desc, byte* data_ptr, u32 data_len, const char* name)                             = 0;
            virtual IGfxPipelineState*  CreateGraphicsPipelineState(const GfxGraphicsPipelineDesc& desc, const char* name)                          = 0;
            virtual IGfxPipelineState*  CreateMeshShadingPipelineState(const GfxMeshShadingPipelineDesc& desc, const char* name)                    = 0;
            virtual IGfxPipelineState*  CreateComputePipelineState(const GfxComputePipelineDesc& desc, const char* name)                            = 0;
            virtual IGfxDescriptor*     CreateShaderResourceView(IGfxResource* resource, const GfxShaderResourceViewDesc& desc, const char* name)   = 0;
            virtual IGfxDescriptor*     CreateUnorderedAccessView(IGfxResource* resource, const GfxUnorderedAccessViewDesc& desc, const char* name) = 0;
            virtual IGfxDescriptor*     CreateConstantBufferView(IGfxBuffer* buffer, const GfxConstantBufferViewDesc& desc, const char* name)       = 0;
            virtual IGfxDescriptor*     CreateSampler(const GfxSamplerDesc& desc, const char* name)                                                 = 0;
            virtual IGfxRayTracingBLAS* CreateRayTracingBLAS(const GfxRayTracingBLASDesc& desc, const char* name)                                   = 0;
            virtual IGfxRayTracingTLAS* CreateRayTracingTLAS(const GfxRayTracingTLASDesc& desc, const char* name)                                   = 0;

            virtual u32  GetAllocationSize(const GfxTextureDesc& desc) = 0;
            virtual bool DumpMemoryStats(const char* file)    = 0;

        protected:
            GfxDeviceDesc m_desc;
            GfxVendor     m_vendor  = GfxVendor::Unkown;
            u64           m_frameID = 0;
        };

    }  // namespace ngfx
}  // namespace ncore

#endif
