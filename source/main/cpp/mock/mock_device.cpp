#include "cgfx/mock/mock_device.h"
#include "cgfx/mock/mock_buffer.h"
#include "cgfx/mock/mock_texture.h"
#include "cgfx/mock/mock_fence.h"
#include "cgfx/mock/mock_swapchain.h"
#include "cgfx/mock/mock_command_list.h"
#include "cgfx/mock/mock_shader.h"
#include "cgfx/mock/mock_pipeline_state.h"
#include "cgfx/mock/mock_descriptor.h"
#include "cgfx/mock/mock_heap.h"
#include "cgfx/mock/mock_rt_blas.h"
#include "cgfx/mock/mock_rt_tlas.h"
#include "cgfx/gfx.h"

#include "ccore/c_math.h"

namespace ncore
{
    namespace ngfx
    {
        MockDevice::MockDevice(const GfxDeviceDesc& desc) { m_desc = desc; }
        MockDevice::~MockDevice() {}

        bool MockDevice::Create() { return true; }

        void* MockDevice::GetHandle() const { return nullptr; }

        void MockDevice::BeginFrame() {}
        void MockDevice::EndFrame() { ++m_frameID; }

        IGfxSwapchain* MockDevice::CreateSwapchain(const GfxSwapchainDesc& desc, const char* name)
        {
            MockSwapchain* swapchain = new MockSwapchain(this, desc, name);
            if (!swapchain->Create())
            {
                delete swapchain;
                return nullptr;
            }
            return swapchain;
        }

        IGfxCommandList* MockDevice::CreateCommandList(GfxCommandQueue queue_type, const char* name)
        {
            MockCommandList* commandList = new MockCommandList(this, queue_type, name);
            if (!commandList->Create())
            {
                delete commandList;
                return nullptr;
            }
            return commandList;
        }

        IGfxFence* MockDevice::CreateFence(const char* name)
        {
            MockFence* fence = new MockFence(this, name);
            if (!fence->Create())
            {
                delete fence;
                return nullptr;
            }
            return fence;
        }

        IGfxHeap* MockDevice::CreateHeap(const GfxHeapDesc& desc, const char* name)
        {
            MockHeap* heap = new MockHeap(this, desc, name);
            if (!heap->Create())
            {
                delete heap;
                return nullptr;
            }
            return heap;
        }

        IGfxBuffer* MockDevice::CreateBuffer(const GfxBufferDesc& desc, const char* name)
        {
            MockBuffer* buffer = new MockBuffer(this, desc, name);
            if (!buffer->Create())
            {
                delete buffer;
                return nullptr;
            }
            return buffer;
        }

        IGfxTexture* MockDevice::CreateTexture(const GfxTextureDesc& desc, const char* name)
        {
            MockTexture* texture = new MockTexture(this, desc, name);
            if (!texture->Create())
            {
                delete texture;
                return nullptr;
            }
            return texture;
        }

        IGfxShader* MockDevice::CreateShader(const GfxShaderDesc& desc, byte* data_ptr, u32 data_len, const char* name)
        {
            MockShader* shader = new MockShader(this, desc, name);
            if (!shader->Create(data_ptr, data_len))
            {
                delete shader;
                return nullptr;
            }
            return shader;
        }

        IGfxPipelineState* MockDevice::CreateGraphicsPipelineState(const GfxGraphicsPipelineDesc& desc, const char* name)
        {
            MockGraphicsPipelineState* pso = new MockGraphicsPipelineState(this, desc, name);
            if (!pso->Create())
            {
                delete pso;
                return nullptr;
            }
            return pso;
        }

        IGfxPipelineState* MockDevice::CreateMeshShadingPipelineState(const GfxMeshShadingPipelineDesc& desc, const char* name)
        {
            MockMeshShadingPipelineState* pso = new MockMeshShadingPipelineState(this, desc, name);
            if (!pso->Create())
            {
                delete pso;
                return nullptr;
            }
            return pso;
        }

        IGfxPipelineState* MockDevice::CreateComputePipelineState(const GfxComputePipelineDesc& desc, const char* name)
        {
            MockComputePipelineState* pso = new MockComputePipelineState(this, desc, name);
            if (!pso->Create())
            {
                delete pso;
                return nullptr;
            }
            return pso;
        }

        IGfxDescriptor* MockDevice::CreateShaderResourceView(IGfxResource* resource, const GfxShaderResourceViewDesc& desc, const char* name)
        {
            MockShaderResourceView* srv = new MockShaderResourceView(this, resource, desc, name);
            if (!srv->Create())
            {
                delete srv;
                return nullptr;
            }
            return srv;
        }

        IGfxDescriptor* MockDevice::CreateUnorderedAccessView(IGfxResource* resource, const GfxUnorderedAccessViewDesc& desc, const char* name)
        {
            MockUnorderedAccessView* uav = new MockUnorderedAccessView(this, resource, desc, name);
            if (!uav->Create())
            {
                delete uav;
                return nullptr;
            }
            return uav;
        }

        IGfxDescriptor* MockDevice::CreateConstantBufferView(IGfxBuffer* buffer, const GfxConstantBufferViewDesc& desc, const char* name)
        {
            MockConstantBufferView* cbv = new MockConstantBufferView(this, buffer, desc, name);
            if (!cbv->Create())
            {
                delete cbv;
                return nullptr;
            }
            return cbv;
        }

        IGfxDescriptor* MockDevice::CreateSampler(const GfxSamplerDesc& desc, const char* name)
        {
            MockSampler* sampler = new MockSampler(this, desc, name);
            if (!sampler->Create())
            {
                delete sampler;
                return nullptr;
            }
            return sampler;
        }

        IGfxRayTracingBLAS* MockDevice::CreateRayTracingBLAS(const GfxRayTracingBLASDesc& desc, const char* name)
        {
            MockRayTracingBLAS* blas = new MockRayTracingBLAS(this, desc, name);
            if (!blas->Create())
            {
                delete blas;
                return nullptr;
            }
            return blas;
        }

        IGfxRayTracingTLAS* MockDevice::CreateRayTracingTLAS(const GfxRayTracingTLASDesc& desc, const char* name)
        {
            MockRayTracingTLAS* tlas = new MockRayTracingTLAS(this, desc, name);
            if (!tlas->Create())
            {
                delete tlas;
                return nullptr;
            }
            return tlas;
        }

        u32 MockDevice::GetAllocationSize(const GfxTextureDesc& desc)
        {
            u32 size = 0;

            u32 min_width  = GetFormatBlockWidth(desc.format);
            u32 min_height = GetFormatBlockHeight(desc.format);

            for (u32 layer = 0; layer < desc.array_size; ++layer)
            {
                for (u32 mip = 0; mip < desc.mip_levels; ++mip)
                {
                    u32 width  = math::max(desc.width >> mip, min_width);
                    u32 height = math::max(desc.height >> mip, min_height);
                    u32 depth  = math::max(desc.depth >> mip, 1u);

                    size += GetFormatRowPitch(desc.format, width) * height * depth;
                }
            }

            return size;
        }

        bool MockDevice::DumpMemoryStats(const char* file) { return false; }

    }  // namespace ngfx
}  // namespace ncore
