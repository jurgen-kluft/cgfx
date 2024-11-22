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
        // swapchain_t* MockDevice::CreateSwapchain(const swapchain_desc_t& desc, const char* name)
        // {
        //     MockSwapchain* swapchain = new MockSwapchain(this, desc, name);
        //     if (!swapchain->Create())
        //     {
        //         delete swapchain;
        //         return nullptr;
        //     }
        //     return swapchain;
        // }

        // command_list_t* MockDevice::CreateCommandList(GfxCommandQueue queue_type, const char* name)
        // {
        //     MockCommandList* commandList = new MockCommandList(this, queue_type, name);
        //     if (!commandList->Create())
        //     {
        //         delete commandList;
        //         return nullptr;
        //     }
        //     return commandList;
        // }

        // fence_t* MockDevice::CreateFence(const char* name)
        // {
        //     MockFence* fence = new MockFence(this, name);
        //     if (!fence->Create())
        //     {
        //         delete fence;
        //         return nullptr;
        //     }
        //     return fence;
        // }

        // heap_t* MockDevice::CreateHeap(const heap_desc_t& desc, const char* name)
        // {
        //     MockHeap* heap = new MockHeap(this, desc, name);
        //     if (!heap->Create())
        //     {
        //         delete heap;
        //         return nullptr;
        //     }
        //     return heap;
        // }

        // buffer_t* MockDevice::CreateBuffer(const buffer_desc_t& desc, const char* name)
        // {
        //     MockBuffer* buffer = new MockBuffer(this, desc, name);
        //     if (!buffer->Create())
        //     {
        //         delete buffer;
        //         return nullptr;
        //     }
        //     return buffer;
        // }

        // texture_t* MockDevice::CreateTexture(const texture_desc_t& desc, const char* name)
        // {
        //     MockTexture* texture = new MockTexture(this, desc, name);
        //     if (!texture->Create())
        //     {
        //         delete texture;
        //         return nullptr;
        //     }
        //     return texture;
        // }

        // shader_t* MockDevice::CreateShader(const shader_desc_t& desc, byte* data_ptr, u32 data_len, const char* name)
        // {
        //     MockShader* shader = new MockShader(this, desc, name);
        //     if (!shader->Create(data_ptr, data_len))
        //     {
        //         delete shader;
        //         return nullptr;
        //     }
        //     return shader;
        // }

        // pipeline_state_t* MockDevice::CreateGraphicsPipelineState(const graphics_pipeline_desc_t& desc, const char* name)
        // {
        //     MockGraphicsPipelineState* pso = new MockGraphicsPipelineState(this, desc, name);
        //     if (!pso->Create())
        //     {
        //         delete pso;
        //         return nullptr;
        //     }
        //     return pso;
        // }

        // pipeline_state_t* MockDevice::CreateMeshShadingPipelineState(const mesh_shading_pipeline_desc_t& desc, const char* name)
        // {
        //     MockMeshShadingPipelineState* pso = new MockMeshShadingPipelineState(this, desc, name);
        //     if (!pso->Create())
        //     {
        //         delete pso;
        //         return nullptr;
        //     }
        //     return pso;
        // }

        // pipeline_state_t* MockDevice::CreateComputePipelineState(const compute_pipeline_desc_t& desc, const char* name)
        // {
        //     MockComputePipelineState* pso = new MockComputePipelineState(this, desc, name);
        //     if (!pso->Create())
        //     {
        //         delete pso;
        //         return nullptr;
        //     }
        //     return pso;
        // }

        // descriptor_t* MockDevice::CreateShaderResourceView(resource_t* resource, const srv_desc_t& desc, const char* name)
        // {
        //     MockShaderResourceView* srv = new MockShaderResourceView(this, resource, desc, name);
        //     if (!srv->Create())
        //     {
        //         delete srv;
        //         return nullptr;
        //     }
        //     return srv;
        // }

        // descriptor_t* MockDevice::CreateUnorderedAccessView(resource_t* resource, const uav_desc_t& desc, const char* name)
        // {
        //     MockUnorderedAccessView* uav = new MockUnorderedAccessView(this, resource, desc, name);
        //     if (!uav->Create())
        //     {
        //         delete uav;
        //         return nullptr;
        //     }
        //     return uav;
        // }

        // descriptor_t* MockDevice::CreateConstantBufferView(buffer_t* buffer, const cbv_desc_t& desc, const char* name)
        // {
        //     MockConstantBufferView* cbv = new MockConstantBufferView(this, buffer, desc, name);
        //     if (!cbv->Create())
        //     {
        //         delete cbv;
        //         return nullptr;
        //     }
        //     return cbv;
        // }

        // descriptor_t* MockDevice::CreateSampler(const sampler_desc_t& desc, const char* name)
        // {
        //     MockSampler* sampler = new MockSampler(this, desc, name);
        //     if (!sampler->Create())
        //     {
        //         delete sampler;
        //         return nullptr;
        //     }
        //     return sampler;
        // }

        // blas_t* MockDevice::CreateRayTracingBLAS(const GfxRayTracing::BLASDesc& desc, const char* name)
        // {
        //     MockRayTracingBLAS* blas = new MockRayTracingBLAS(this, desc, name);
        //     if (!blas->Create())
        //     {
        //         delete blas;
        //         return nullptr;
        //     }
        //     return blas;
        // }

        // tlas_t* MockDevice::CreateRayTracingTLAS(const GfxRayTracing::TLASDesc& desc, const char* name)
        // {
        //     MockRayTracingTLAS* tlas = new MockRayTracingTLAS(this, desc, name);
        //     if (!tlas->Create())
        //     {
        //         delete tlas;
        //         return nullptr;
        //     }
        //     return tlas;
        // }

        namespace nmock
        {
            void  CreateDevice(ngfx::device_t* device) {}
            bool  Create(device_t* device) { return true; }
            void  Destroy(device_t* device) {}
            void* GetHandle(device_t* device) { return nullptr; }
            void  BeginFrame(device_t* device) {}
            void  EndFrame(device_t* device) { ++device->m_frameID; }

            bool DumpMemoryStats(device_t* device, const char* file) { return false; }

        }  // namespace nmock

    }  // namespace ngfx
}  // namespace ncore
