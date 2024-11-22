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

#include "ccore/c_allocator.h"
#include "ccore/c_math.h"
#include "cmacos/Metal/Metal.hpp"
#include "cmacos/MetalShaderConverter/IRConverterRuntime/ir_converter_runtime.h"

namespace ncore
{
    namespace ngfx
    {
        namespace nmetal
        {

            // ---------------------------------------------------------------------------------------
            // ---------------------------------------------------------------------------------------

            struct ConstantBufferAllocator
            {
                nmetal::device_t* m_pDevice       = nullptr;
                MTL::Buffer*      m_pBuffer       = nullptr;
                void*             m_pCpuAddress   = nullptr;
                u32               m_bufferSize    = 0;
                u32               m_allocatedSize = 0;
            };

            void setup(ConstantBufferAllocator* cba, nmetal::device_t* device, u32 buffer_size, const char* name)
            {
                cba->m_pDevice = device;
                cba->m_pBuffer = device->m_pDevice->newBuffer(buffer_size, MTL::ResourceStorageModeShared | MTL::ResourceCPUCacheModeWriteCombined | MTL::ResourceHazardTrackingModeTracked);
                MakeResident(cba->m_pDevice, cba->m_pBuffer);
                SetDebugLabel(cba->m_pBuffer, name);

                cba->m_pCpuAddress = cba->m_pBuffer->contents();
                cba->m_bufferSize  = buffer_size;
            }

            void teardown(ConstantBufferAllocator* cba)
            {
                Evict(cba->m_pDevice, cba->m_pBuffer);
                cba->m_pBuffer->release();
            }

            void allocateConstantBuffer(ConstantBufferAllocator* cba, u32 size, void** cpu_address, u64* gpu_address)
            {
                ASSERT(cba->m_allocatedSize + size <= cba->m_bufferSize);

                *cpu_address = (char*)cba->m_pCpuAddress + cba->m_allocatedSize;
                *gpu_address = cba->m_pBuffer->gpuAddress() + cba->m_allocatedSize;
                cba->m_allocatedSize += math::g_alignUp(size, 8);  // Shader converter requires an alignment of 8-bytes:
            }

            void reset(ConstantBufferAllocator* cba) { cba->m_allocatedSize = 0; }

            // ---------------------------------------------------------------------------------------
            // ---------------------------------------------------------------------------------------

            struct DescriptorAllocator
            {
                nmetal::device_t* m_pDevice              = nullptr;
                MTL::Buffer*      m_pBuffer              = nullptr;
                void*             m_pCpuAddress          = nullptr;
                u32               m_descirptorCount      = 0;
                u32               m_allocatedCount       = 0;
                u32*              m_freeDescriptors      = nullptr;
                u32               m_freeDescriptorsCount = 0;
            };

            void setup(DescriptorAllocator* da, nmetal::device_t* device, u32 descriptor_count, const char* name)
            {
                da->m_pDevice = device;
                da->m_pBuffer = device->m_pDevice->newBuffer(sizeof(IRDescriptorTableEntry) * descriptor_count, MTL::ResourceStorageModeShared | MTL::ResourceCPUCacheModeWriteCombined | MTL::ResourceHazardTrackingModeTracked);
                MakeResident(da->m_pDevice, da->m_pBuffer);
                SetDebugLabel(da->m_pBuffer, name);

                da->m_pCpuAddress     = da->m_pBuffer->contents();
                da->m_descirptorCount = descriptor_count;
            }

            void teardown(DescriptorAllocator* da)
            {
                Evict(da->m_pDevice, da->m_pBuffer);
                da->m_pBuffer->release();
            }

            u32 allocateDescriptor(DescriptorAllocator* da, IRDescriptorTableEntry** descriptor)
            {
                u32 index = 0;

                if (da->m_freeDescriptorsCount > 0)
                {
                    da->m_freeDescriptorsCount -= 1;
                    index = da->m_freeDescriptors[da->m_freeDescriptorsCount];
                }
                else
                {
                    index = da->m_allocatedCount;
                    ++da->m_allocatedCount;
                }

                *descriptor = (IRDescriptorTableEntry*)((char*)da->m_pCpuAddress + sizeof(IRDescriptorTableEntry) * index);
                return index;
            }

            void         freeDescriptor(DescriptorAllocator* da, u32 index) { da->m_freeDescriptors[da->m_freeDescriptorsCount++] = index; }
            MTL::Buffer* getBuffer(DescriptorAllocator* da) { return da->m_pBuffer; }

            // ---------------------------------------------------------------------------------------
            // ---------------------------------------------------------------------------------------

            void CreateDevice(ngfx::device_t* device) { nmetal::device_t* mdevice = CreateComponent<ngfx::device_t, nmetal::device_t>(device, device); }

            bool Create(ngfx::device_t* device)
            {
                nmetal::device_t* mdevice = GetComponent<ngfx::device_t, nmetal::device_t>(device, device);
                mdevice->m_pDevice        = MTL::CreateSystemDefaultDevice();

                // RE_INFO("GPU : {}", m_pDevice->name()->utf8String());
                if (!mdevice->m_pDevice->supportsFamily(MTL::GPUFamilyApple7))
                {
                    // RE_INFO("RealEngine requires a Metal GPU family Apple7+ device");
                    return false;
                }

                MTL::ResidencySetDescriptor* descriptor = MTL::ResidencySetDescriptor::alloc()->init();
                descriptor->setInitialCapacity(GFX_MAX_RESOURCE_DESCRIPTOR_COUNT);
                mdevice->m_pResidencySet = mdevice->m_pDevice->newResidencySet(descriptor, nullptr);
                descriptor->release();

                mdevice->m_pQueue = mdevice->m_pDevice->newCommandQueue();
                mdevice->m_pQueue->addResidencySet(mdevice->m_pResidencySet);

                for (u32 i = 0; i < GFX_MAX_INFLIGHT_FRAMES; ++i)
                {
                    const char*              name        = "in-flight frame";  // fmt::format("CB Allocator {}", i).c_str();
                    ConstantBufferAllocator* cballocator = g_allocate<ConstantBufferAllocator>(device->m_allocator);
                    setup(cballocator, mdevice, 8 * 1024 * 1024, name);
                    mdevice->m_pConstantBufferAllocators[i] = cballocator;
                }

                DescriptorAllocator* mdallocator = g_allocate<DescriptorAllocator>(device->m_allocator);
                setup(mdallocator, mdevice, GFX_MAX_RESOURCE_DESCRIPTOR_COUNT, "Descriptor Heap");
                mdevice->m_pResDescriptorAllocator = mdallocator;

                DescriptorAllocator* msallocator = g_allocate<DescriptorAllocator>(device->m_allocator);
                setup(msallocator, mdevice, GFX_MAX_SAMPLER_DESCRIPTOR_COUNT, "Sampler Heap");
                mdevice->m_pSamplerAllocator = msallocator;

                mdevice->m_resDescriptorDeletionQueueFirst  = g_allocate_array<u32>(device->m_allocator, GFX_MAX_RESOURCE_DESCRIPTOR_COUNT);
                mdevice->m_resDescriptorDeletionQueueSecond = g_allocate_array<u64>(device->m_allocator, GFX_MAX_RESOURCE_DESCRIPTOR_COUNT);

                mdevice->m_samplerDescriptorDeletionQueueFirst  = g_allocate_array<u32>(device->m_allocator, GFX_MAX_SAMPLER_DESCRIPTOR_COUNT);
                mdevice->m_samplerDescriptorDeletionQueueSecond = g_allocate_array<u64>(device->m_allocator, GFX_MAX_SAMPLER_DESCRIPTOR_COUNT);

                return true;
            }

            void Destroy(ngfx::device_t* device)
            {
                nmetal::device_t* mdevice = GetComponent<ngfx::device_t, nmetal::device_t>(device, device);

                for (u32 i = 0; i < GFX_MAX_INFLIGHT_FRAMES; ++i)
                {
                    teardown(mdevice->m_pConstantBufferAllocators[i]);
                    device->m_allocator->deallocate(mdevice->m_pConstantBufferAllocators[i]);
                    mdevice->m_pConstantBufferAllocators[i] = nullptr;
                }

                teardown(mdevice->m_pResDescriptorAllocator);
                device->m_allocator->deallocate(mdevice->m_pResDescriptorAllocator);
                mdevice->m_pResDescriptorAllocator = nullptr;

                teardown(mdevice->m_pSamplerAllocator);
                device->m_allocator->deallocate(mdevice->m_pSamplerAllocator);
                mdevice->m_pSamplerAllocator = nullptr;

                device->m_allocator->deallocate(mdevice->m_resDescriptorDeletionQueueFirst);
                device->m_allocator->deallocate(mdevice->m_resDescriptorDeletionQueueSecond);

                device->m_allocator->deallocate(mdevice->m_samplerDescriptorDeletionQueueFirst);
                device->m_allocator->deallocate(mdevice->m_samplerDescriptorDeletionQueueSecond);

                mdevice->m_pResidencySet->release();
                mdevice->m_pQueue->release();
                mdevice->m_pDevice->release();
                mdevice->m_pResidencySet = nullptr;
                mdevice->m_pQueue        = nullptr;
                mdevice->m_pDevice       = nullptr;
            }

            MTL::Device* GetHandle(ngfx::device_t* device);

            void BeginFrame(ngfx::device_t* device)
            {
                nmetal::device_t* mdevice = GetComponent<ngfx::device_t, nmetal::device_t>(device, device);

                if (mdevice->m_bResidencyDirty)
                {
                    mdevice->m_pResidencySet->commit();
                    mdevice->m_bResidencyDirty = false;
                }

                u32 index = device->m_frameID % GFX_MAX_INFLIGHT_FRAMES;
                reset(mdevice->m_pConstantBufferAllocators[index]);

                while (mdevice->m_resDescriptorDeletionQueueCount > 0)
                {
                    u64 const frameID = mdevice->m_resDescriptorDeletionQueueSecond[mdevice->m_resDescriptorDeletionQueueCount - 1];
                    if (frameID + GFX_MAX_INFLIGHT_FRAMES > device->m_frameID)
                        break;

                    u32 const index = mdevice->m_resDescriptorDeletionQueueFirst[mdevice->m_resDescriptorDeletionQueueCount - 1];
                    freeDescriptor(mdevice->m_pResDescriptorAllocator, index);

                    mdevice->m_resDescriptorDeletionQueueCount -= 1;
                }

                while (mdevice->m_samplerDescriptorDeletionQueueCount > 0)
                {
                    u64 const frameID = mdevice->m_samplerDescriptorDeletionQueueSecond[mdevice->m_samplerDescriptorDeletionQueueCount - 1];
                    if (frameID + GFX_MAX_INFLIGHT_FRAMES > device->m_frameID)
                        break;

                    u32 const index = mdevice->m_samplerDescriptorDeletionQueueFirst[mdevice->m_samplerDescriptorDeletionQueueCount - 1];
                    freeDescriptor(mdevice->m_pSamplerAllocator, index);

                    mdevice->m_samplerDescriptorDeletionQueueCount -= 1;
                }
            }

            void EndFrame(ngfx::device_t* device) { ++device->m_frameID; }

            u32 GetAllocationSize(nmetal::device_t* device, const texture_desc_t& desc)
            {
                MTL::TextureDescriptor* descriptor   = ToTextureDescriptor(desc);
                MTL::SizeAndAlign       sizeAndAlign = device->m_pDevice->heapTextureSizeAndAlign(descriptor);
                descriptor->release();
                return (u32)sizeAndAlign.size;
            }

            u64 AllocateConstantBuffer(ngfx::device_t* device, const void* data, size_t data_size)
            {
                nmetal::device_t* mdevice = GetComponent<ngfx::device_t, nmetal::device_t>(device, device);

                void* cpu_address;
                u64   gpu_address;
                u32   index = device->m_frameID % GFX_MAX_INFLIGHT_FRAMES;
                allocateConstantBuffer(mdevice->m_pConstantBufferAllocators[index], (u32)data_size, &cpu_address, &gpu_address);
                memcpy(cpu_address, data, data_size);
                return gpu_address;
            }

            u32 AllocateResourceDescriptor(ngfx::device_t* device, IRDescriptorTableEntry** descriptor)
            {
                nmetal::device_t* mdevice = GetComponent<ngfx::device_t, nmetal::device_t>(device, device);
                return allocateDescriptor(mdevice->m_pResDescriptorAllocator, descriptor);
            }

            u32 AllocateSamplerDescriptor(ngfx::device_t* device, IRDescriptorTableEntry** descriptor)
            {
                nmetal::device_t* mdevice = GetComponent<ngfx::device_t, nmetal::device_t>(device, device);
                return allocateDescriptor(mdevice->m_pSamplerAllocator, descriptor);
            }

            void FreeResourceDescriptor(ngfx::device_t* device, u32 index)
            {
                if (index != GFX_INVALID_RESOURCE)
                {
                    nmetal::device_t* mdevice                                                               = GetComponent<ngfx::device_t, nmetal::device_t>(device, device);
                    mdevice->m_resDescriptorDeletionQueueFirst[mdevice->m_resDescriptorDeletionQueueCount]  = index;
                    mdevice->m_resDescriptorDeletionQueueSecond[mdevice->m_resDescriptorDeletionQueueCount] = device->m_frameID;
                    mdevice->m_resDescriptorDeletionQueueCount += 1;
                }
            }

            void FreeSamplerDescriptor(ngfx::device_t* device, u32 index)
            {
                if (index != GFX_INVALID_RESOURCE)
                {
                    nmetal::device_t* mdevice                                                                       = GetComponent<ngfx::device_t, nmetal::device_t>(device, device);
                    mdevice->m_samplerDescriptorDeletionQueueFirst[mdevice->m_samplerDescriptorDeletionQueueCount]  = index;
                    mdevice->m_samplerDescriptorDeletionQueueSecond[mdevice->m_samplerDescriptorDeletionQueueCount] = device->m_frameID;
                    mdevice->m_samplerDescriptorDeletionQueueCount += 1;
                }
            }

            MTL::Buffer* GetResourceDescriptorBuffer(nmetal::device_t* device) { return device->m_pResDescriptorAllocator->m_pBuffer; }
            MTL::Buffer* GetSamplerDescriptorBuffer(nmetal::device_t* device) { return device->m_pSamplerAllocator->m_pBuffer; }

            void MakeResident(nmetal::device_t* device, const MTL::Allocation* allocation)
            {
                device->m_pResidencySet->addAllocation(allocation);
                device->m_bResidencyDirty = true;
            }

            void Evict(nmetal::device_t* device, const MTL::Allocation* allocation)
            {
                device->m_pResidencySet->removeAllocation(allocation);
                device->m_bResidencyDirty = true;
            }

            bool DumpMemoryStats(ngfx::device_t* device, const char* file)
            {
                nmetal::device_t* mdevice = GetComponent<ngfx::device_t, nmetal::device_t>(device, device);
                return false;
            }

        }  // namespace nmetal
    }  // namespace ngfx
}  // namespace ncore
