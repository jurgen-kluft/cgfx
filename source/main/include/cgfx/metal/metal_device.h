#ifndef __CGFX_METAL_DEVICE_H__
#define __CGFX_METAL_DEVICE_H__

#include "cgfx/metal/metal_utils.h"
#include "cgfx/gfx_device.h"

struct IRDescriptorTableEntry;

namespace ncore
{
    namespace ngfx
    {
#ifdef TARGET_MAC
        namespace nmetal
        {
            class MetalConstantBufferAllocator;
            class MetalDescriptorAllocator;
            class MetalDescriptorAllocator;

            struct device_t
            {
                D_GFX_OCS_COMPONENT_SET(enums::ComponentMetalDevice);
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
                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            bool  Create(ngfx::device_t* device);
            void  Destroy(ngfx::device_t* device);
            void* GetHandle(ngfx::device_t* device);
            void  BeginFrame(ngfx::device_t* device);
            void  EndFrame(ngfx::device_t* device);

            u32          AllocateResourceDescriptor(ngfx::device_t* device, IRDescriptorTableEntry** descriptor);
            u32          AllocateSamplerDescriptor(ngfx::device_t* device, IRDescriptorTableEntry** descriptor);
            void         FreeResourceDescriptor(ngfx::device_t* device, u32 index);
            void         FreeSamplerDescriptor(ngfx::device_t* device, u32 index);
            MTL::Buffer* GetResourceDescriptorBuffer(ngfx::device_t* device);
            MTL::Buffer* GetSamplerDescriptorBuffer(ngfx::device_t* device);

            void MakeResident(ngfx::device_t* device, const MTL::Allocation* allocation);
            void Evict(ngfx::device_t* device, const MTL::Allocation* allocation);

            bool DumpMemoryStats(ngfx::device_t* device, const char* file);
        }  // namespace nmetal
#else
        namespace nmetal
        {
            bool  Create(ngfx::device_t* device) { return false; }
            void  Destroy(ngfx::device_t* device) {}
            void* GetHandle(ngfx::device_t* device) { return nullptr; }
            void  BeginFrame(ngfx::device_t* device) {}
            void  EndFrame(ngfx::device_t* device) {}

            u32  AllocateResourceDescriptor(ngfx::device_t* device, IRDescriptorTableEntry** descriptor) { return 0; }
            u32  AllocateSamplerDescriptor(ngfx::device_t* device, IRDescriptorTableEntry** descriptor) { return 0; }
            void FreeResourceDescriptor(ngfx::device_t* device, u32 index) {}
            void FreeSamplerDescriptor(ngfx::device_t* device, u32 index) {}

            bool DumpMemoryStats(ngfx::device_t* device, const char* file) { return false; }
        }  // namespace nmetal
#endif
    }  // namespace ngfx
}  // namespace ncore
#endif
