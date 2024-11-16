#include "cgfx/metal/metal_device.h"
#include "cgfx/metal/metal_buffer.h"
#include "cgfx/metal/metal_texture.h"
#include "cgfx/metal/metal_fence.h"
#include "cgfx/metal/metal_swapchain.h"
#include "cgfx/metal/metal_command_list.h"
#include "cgfx/metal/metal_shader.h"
#include "cgfx/metal/metal_pipeline_state.h"
#include "cgfx/metal/metal_descriptor.h"
#include "cgfx/metal/metal_heap.h"
#include "cgfx/metal/metal_rt_blas.h"
#include "cgfx/metal/metal_rt_tlas.h"
#include "cgfx/gfx.h"

#include "cbase/c_integer.h"
#include "cmacos/Metal/Metal.hpp"
#include "cmacos/MetalShaderConverter/IRConverterRuntime/ir_converter_runtime.h"

// RoundUpPow2

namespace ncore
{
    namespace ngfx
    {
        // class MetalConstantBufferAllocator
        // {
        // public:
        //     MetalConstantBufferAllocator(MetalDevice* device, u32 buffer_size, const char* name)
        //     {
        //         m_pDevice = device;

        //         MTL::Device* mtlDevice = (MTL::Device*)m_pDevice->GetHandle();
        //         m_pBuffer              = mtlDevice->newBuffer(buffer_size, MTL::ResourceStorageModeShared | MTL::ResourceCPUCacheModeWriteCombined | MTL::ResourceHazardTrackingModeTracked);
        //         m_pDevice->MakeResident(m_pBuffer);
        //         SetDebugLabel(m_pBuffer, name);

        //         m_pCpuAddress = m_pBuffer->contents();
        //         m_bufferSize  = buffer_size;
        //     }

        //     ~MetalConstantBufferAllocator()
        //     {
        //         m_pDevice->Evict(m_pBuffer);
        //         m_pBuffer->release();
        //     }

        //     void Allocate(u32 size, void** cpu_address, u64* gpu_address)
        //     {
        //         ASSERT(m_allocatedSize + size <= m_bufferSize);

        //         *cpu_address = (char*)m_pCpuAddress + m_allocatedSize;
        //         *gpu_address = m_pBuffer->gpuAddress() + m_allocatedSize;
        //         m_allocatedSize += math::alignUp(size, 8);  // Shader converter requires an alignment of 8-bytes:
        //     }

        //     void Reset() { m_allocatedSize = 0; }

        // private:
        //     MetalDevice* m_pDevice       = nullptr;
        //     MTL::Buffer* m_pBuffer       = nullptr;
        //     void*        m_pCpuAddress   = nullptr;
        //     u32          m_bufferSize    = 0;
        //     u32          m_allocatedSize = 0;
        // };

        // class MetalDescriptorAllocator
        // {
        // public:
        //     MetalDescriptorAllocator(MetalDevice* device, u32 descriptor_count, const char* name)
        //     {
        //         m_pDevice = device;

        //         MTL::Device* mtlDevice = (MTL::Device*)m_pDevice->GetHandle();
        //         m_pBuffer              = mtlDevice->newBuffer(sizeof(IRDescriptorTableEntry) * descriptor_count, MTL::ResourceStorageModeShared | MTL::ResourceCPUCacheModeWriteCombined | MTL::ResourceHazardTrackingModeTracked);
        //         m_pDevice->MakeResident(m_pBuffer);
        //         SetDebugLabel(m_pBuffer, name);

        //         m_pCpuAddress     = m_pBuffer->contents();
        //         m_descirptorCount = descriptor_count;
        //     }

        //     ~MetalDescriptorAllocator()
        //     {
        //         m_pDevice->Evict(m_pBuffer);
        //         m_pBuffer->release();
        //     }

        //     u32 Allocate(IRDescriptorTableEntry** descriptor)
        //     {
        //         u32 index = 0;

        //         if (m_freeDescriptorsCount > 0)
        //         {
        //             m_freeDescriptorsCount -= 1;
        //             index = m_freeDescriptors[m_freeDescriptorsCount];
        //         }
        //         else
        //         {
        //             index = m_allocatedCount;
        //             ++m_allocatedCount;
        //         }

        //         *descriptor = (IRDescriptorTableEntry*)((char*)m_pCpuAddress + sizeof(IRDescriptorTableEntry) * index);
        //         return index;
        //     }

        //     void Free(u32 index) { m_freeDescriptors[m_freeDescriptorsCount++] = index; }

        //     MTL::Buffer* GetBuffer() const { return m_pBuffer; }

        // private:
        //     MetalDevice* m_pDevice              = nullptr;
        //     MTL::Buffer* m_pBuffer              = nullptr;
        //     void*        m_pCpuAddress          = nullptr;
        //     u32          m_descirptorCount      = 0;
        //     u32          m_allocatedCount       = 0;
        //     u32*         m_freeDescriptors      = nullptr;
        //     u32          m_freeDescriptorsCount = 0;
        // };

        // MetalDevice::MetalDevice(const device_desc_t& desc)
        // {
        //     m_desc   = desc;
        //     m_vendor = GfxVendor::Apple;
        // }

        // MetalDevice::~MetalDevice()
        // {
        //     for (u32 i = 0; i < GFX_MAX_INFLIGHT_FRAMES; ++i)
        //     {
        //         m_pConstantBufferAllocators[i] = nullptr;
        //     }

        //     m_pResDescriptorAllocator = nullptr;
        //     m_pSamplerAllocator       = nullptr;

        //     m_pResidencySet->release();
        //     m_pQueue->release();
        //     m_pDevice->release();
        // }

        // bool MetalDevice::Create()
        // {
        //     m_pDevice = MTL::CreateSystemDefaultDevice();

        //     // RE_INFO("GPU : {}", m_pDevice->name()->utf8String());

        //     if (!m_pDevice->supportsFamily(MTL::GPUFamilyApple7))
        //     {
        //         // RE_INFO("RealEngine requires a Metal GPU family Apple7+ device");
        //         return false;
        //     }

        //     MTL::ResidencySetDescriptor* descriptor = MTL::ResidencySetDescriptor::alloc()->init();
        //     descriptor->setInitialCapacity(GFX_MAX_RESOURCE_DESCRIPTOR_COUNT);
        //     m_pResidencySet = m_pDevice->newResidencySet(descriptor, nullptr);
        //     descriptor->release();

        //     m_pQueue = m_pDevice->newCommandQueue();
        //     m_pQueue->addResidencySet(m_pResidencySet);

        //     for (u32 i = 0; i < GFX_MAX_INFLIGHT_FRAMES; ++i)
        //     {
        //         const char* name = "in-flight frame";  // fmt::format("CB Allocator {}", i).c_str();
        //         // TODO: allocate
        //         MetalConstantBufferAllocator* cballocator = nullptr;  // <MetalConstantBufferAllocator>(this, 8 * 1024 * 1024, name);
        //         m_pConstantBufferAllocators[i]            = cballocator;
        //     }

        //     // TODO allocate
        //     // m_pResDescriptorAllocator = eastl::make_unique<MetalDescriptorAllocator>(this, GFX_MAX_RESOURCE_DESCRIPTOR_COUNT, "Resource Heap");
        //     // m_pSamplerAllocator       = eastl::make_unique<MetalDescriptorAllocator>(this, GFX_MAX_SAMPLER_DESCRIPTOR_COUNT, "Sampler Heap");

        //     return true;
        // }

        // void MetalDevice::BeginFrame()
        // {
        //     if (m_bResidencyDirty)
        //     {
        //         m_pResidencySet->commit();
        //         m_bResidencyDirty = false;
        //     }

        //     u32 index = m_frameID % GFX_MAX_INFLIGHT_FRAMES;
        //     m_pConstantBufferAllocators[index]->Reset();

        //     // TODO process and deallocate
        //     // while (!m_resDescriptorDeletionQueue.empty())
        //     // {
        //     //     auto item = m_resDescriptorDeletionQueue.front();
        //     //     if (item.second + GFX_MAX_INFLIGHT_FRAMES > m_frameID)
        //     //     {
        //     //         break;
        //     //     }

        //     //     m_pResDescriptorAllocator->Free(item.first);
        //     //     m_resDescriptorDeletionQueue.pop();
        //     // }

        //     // while (!m_samplerDescriptorDeletionQueue.empty())
        //     // {
        //     //     auto item = m_samplerDescriptorDeletionQueue.front();
        //     //     if (item.second + GFX_MAX_INFLIGHT_FRAMES > m_frameID)
        //     //     {
        //     //         break;
        //     //     }

        //     //     m_pSamplerAllocator->Free(item.first);
        //     //     m_samplerDescriptorDeletionQueue.pop();
        //     // }
        // }

        // void MetalDevice::EndFrame() { ++m_frameID; }

        // swapchain_t* MetalDevice::CreateSwapchain(const swapchain_desc_t& desc, const char* name)
        // {
        //     MetalSwapchain* swapchain = new MetalSwapchain(this, desc, name);
        //     if (!swapchain->Create())
        //     {
        //         delete swapchain;
        //         return nullptr;
        //     }
        //     return swapchain;
        // }

        // command_list_t* MetalDevice::CreateCommandList(GfxCommandQueue queue_type, const char* name)
        // {
        //     MetalCommandList* commandList = new MetalCommandList(this, queue_type, name);
        //     if (!commandList->Create())
        //     {
        //         delete commandList;
        //         return nullptr;
        //     }
        //     return commandList;
        // }

        // fence_t* MetalDevice::CreateFence(const char* name)
        // {
        //     MetalFence* fence = new MetalFence(this, name);
        //     if (!fence->Create())
        //     {
        //         delete fence;
        //         return nullptr;
        //     }
        //     return fence;
        // }

        // heap_t* MetalDevice::CreateHeap(const heap_desc_t& desc, const char* name)
        // {
        //     MetalHeap* heap = new MetalHeap(this, desc, name);
        //     if (!heap->Create())
        //     {
        //         delete heap;
        //         return nullptr;
        //     }
        //     return heap;
        // }

        // buffer_t* MetalDevice::CreateBuffer(const buffer_desc_t& desc, const char* name)
        // {
        //     MetalBuffer* buffer = new MetalBuffer(this, desc, name);
        //     if (!buffer->Create())
        //     {
        //         delete buffer;
        //         return nullptr;
        //     }
        //     return buffer;
        // }

        // texture_t* MetalDevice::CreateTexture(const texture_desc_t& desc, const char* name)
        // {
        //     MetalTexture* texture = new MetalTexture(this, desc, name);
        //     if (!texture->Create())
        //     {
        //         delete texture;
        //         return nullptr;
        //     }
        //     return texture;
        // }

        // shader_t* MetalDevice::CreateShader(const shader_desc_t& desc, byte* data_ptr, u32 data_len, const char* name)
        // {
        //     MetalShader* shader = new MetalShader(this, desc, name);
        //     if (!shader->Create(data_ptr, data_len))
        //     {
        //         delete shader;
        //         return nullptr;
        //     }
        //     return shader;
        // }

        // pipeline_state_t* MetalDevice::CreateGraphicsPipelineState(const graphics_pipeline_desc_t& desc, const char* name)
        // {
        //     MetalGraphicsPipelineState* pso = new MetalGraphicsPipelineState(this, desc, name);
        //     if (!pso->Create())
        //     {
        //         delete pso;
        //         return nullptr;
        //     }
        //     return pso;
        // }

        // pipeline_state_t* MetalDevice::CreateMeshShadingPipelineState(const mesh_shading_pipeline_desc_t& desc, const char* name)
        // {
        //     MetalMeshShadingPipelineState* pso = new MetalMeshShadingPipelineState(this, desc, name);
        //     if (!pso->Create())
        //     {
        //         delete pso;
        //         return nullptr;
        //     }
        //     return pso;
        // }

        // pipeline_state_t* MetalDevice::CreateComputePipelineState(const compute_pipeline_desc_t& desc, const char* name)
        // {
        //     MetalComputePipelineState* pso = new MetalComputePipelineState(this, desc, name);
        //     if (!pso->Create())
        //     {
        //         delete pso;
        //         return nullptr;
        //     }
        //     return pso;
        // }

        // descriptor_t* MetalDevice::CreateShaderResourceView(resource_t* resource, const srv_desc_t& desc, const char* name)
        // {
        //     MetalShaderResourceView* srv = new MetalShaderResourceView(this, resource, desc, name);
        //     if (!srv->Create())
        //     {
        //         delete srv;
        //         return nullptr;
        //     }
        //     return srv;
        // }

        // descriptor_t* MetalDevice::CreateUnorderedAccessView(resource_t* resource, const uav_desc_t& desc, const char* name)
        // {
        //     MetalUnorderedAccessView* uav = new MetalUnorderedAccessView(this, resource, desc, name);
        //     if (!uav->Create())
        //     {
        //         delete uav;
        //         return nullptr;
        //     }
        //     return uav;
        // }

        // descriptor_t* MetalDevice::CreateConstantBufferView(buffer_t* buffer, const cbv_desc_t& desc, const char* name)
        // {
        //     MetalConstantBufferView* cbv = new MetalConstantBufferView(this, buffer, desc, name);
        //     if (!cbv->Create())
        //     {
        //         delete cbv;
        //         return nullptr;
        //     }
        //     return cbv;
        // }

        // descriptor_t* MetalDevice::CreateSampler(const sampler_desc_t& desc, const char* name)
        // {
        //     MetalSampler* sampler = new MetalSampler(this, desc, name);
        //     if (!sampler->Create())
        //     {
        //         delete sampler;
        //         return nullptr;
        //     }
        //     return sampler;
        // }

        // blas_t* MetalDevice::CreateRayTracingBLAS(const GfxRayTracingBLASDesc& desc, const char* name)
        // {
        //     MetalRayTracingBLAS* blas = new MetalRayTracingBLAS(this, desc, name);
        //     if (!blas->Create())
        //     {
        //         delete blas;
        //         return nullptr;
        //     }
        //     return blas;
        // }

        // tlas_t* MetalDevice::CreateRayTracingTLAS(const GfxRayTracingTLASDesc& desc, const char* name)
        // {
        //     MetalRayTracingTLAS* tlas = new MetalRayTracingTLAS(this, desc, name);
        //     if (!tlas->Create())
        //     {
        //         delete tlas;
        //         return nullptr;
        //     }
        //     return tlas;
        // }

        // u32 MetalDevice::GetAllocationSize(const texture_desc_t& desc)
        // {
        //     MTL::TextureDescriptor* descriptor   = ToTextureDescriptor(desc);
        //     MTL::SizeAndAlign       sizeAndAlign = m_pDevice->heapTextureSizeAndAlign(descriptor);
        //     descriptor->release();

        //     return (u32)sizeAndAlign.size;
        // }

        // bool MetalDevice::DumpMemoryStats(const char* file) { return false; }

        // u64 MetalDevice::AllocateConstantBuffer(const void* data, size_t data_size)
        // {
        //     void* cpu_address;
        //     u64   gpu_address;

        //     u32 index = m_frameID % GFX_MAX_INFLIGHT_FRAMES;
        //     m_pConstantBufferAllocators[index]->Allocate((u32)data_size, &cpu_address, &gpu_address);

        //     memcpy(cpu_address, data, data_size);

        //     return gpu_address;
        // }

        // u32 MetalDevice::AllocateResourceDescriptor(IRDescriptorTableEntry** descriptor) { return m_pResDescriptorAllocator->Allocate(descriptor); }

        // u32 MetalDevice::AllocateSamplerDescriptor(IRDescriptorTableEntry** descriptor) { return m_pSamplerAllocator->Allocate(descriptor); }

        // void MetalDevice::FreeResourceDescriptor(u32 index)
        // {
        //     if (index != GFX_INVALID_RESOURCE)
        //     {
        //         // m_resDescriptorDeletionQueue.push(eastl::make_pair(index, m_frameID));
        //         // TODO add to deletion queue
        //     }
        // }

        // void MetalDevice::FreeSamplerDescriptor(u32 index)
        // {
        //     if (index != GFX_INVALID_RESOURCE)
        //     {
        //         // m_samplerDescriptorDeletionQueue.push(eastl::make_pair(index, m_frameID));
        //         // TODO add to deletion queue
        //     }
        // }

        // MTL::Buffer* MetalDevice::GetResourceDescriptorBuffer() const { return m_pResDescriptorAllocator->GetBuffer(); }

        // MTL::Buffer* MetalDevice::GetSamplerDescriptorBuffer() const { return m_pSamplerAllocator->GetBuffer(); }

        // void MetalDevice::MakeResident(const MTL::Allocation* allocation)
        // {
        //     m_pResidencySet->addAllocation(allocation);
        //     m_bResidencyDirty = true;
        // }

        // void MetalDevice::Evict(const MTL::Allocation* allocation)
        // {
        //     m_pResidencySet->removeAllocation(allocation);
        //     m_bResidencyDirty = true;
        // }

    }  // namespace ngfx
}  // namespace ncore
