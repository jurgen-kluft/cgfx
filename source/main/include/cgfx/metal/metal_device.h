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
            struct ConstantBufferAllocator;
            struct DescriptorAllocator;
            struct DescriptorAllocator;

            struct device_t
            {
                D_GFX_OCS_COMPONENT_SET(enums::ComponentMetalDevice);
                MTL::Device*       m_pDevice         = nullptr;
                MTL::CommandQueue* m_pQueue          = nullptr;
                MTL::ResidencySet* m_pResidencySet   = nullptr;
                bool               m_bResidencyDirty = false;

                ConstantBufferAllocator* m_pConstantBufferAllocators[GFX_MAX_INFLIGHT_FRAMES];
                DescriptorAllocator*     m_pResDescriptorAllocator;
                DescriptorAllocator*     m_pSamplerAllocator;

                // We may need a ring buffer or something of the sort
                s32  m_resDescriptorDeletionQueueCount;
                s32  m_resDescriptorDeletionQueueMax;
                u32* m_resDescriptorDeletionQueueFirst;
                u64* m_resDescriptorDeletionQueueSecond;

                s32  m_samplerDescriptorDeletionQueueCount;
                s32  m_samplerDescriptorDeletionQueueMax;
                u32* m_samplerDescriptorDeletionQueueFirst;
                u64* m_samplerDescriptorDeletionQueueSecond;
                DCORE_CLASS_PLACEMENT_NEW_DELETE
            };

            void CreateDevice(ngfx::device_t* device, u32 max_instances);
            bool Create(ngfx::device_t* device);
            void Destroy(ngfx::device_t* device);
            void BeginFrame(ngfx::device_t* device);
            void EndFrame(ngfx::device_t* device);

            // TODO: investigate if these functions can just accept 'nmetal::device_t* mdevice' instead of 'ngfx::device_t* device'
            MTL::Device* GetHandle(ngfx::device_t* device);
            u64          AllocateConstantBuffer(ngfx::device_t* device, const void* data, size_t data_size);
            u32          AllocateResourceDescriptor(ngfx::device_t* device, IRDescriptorTableEntry** descriptor);
            u32          AllocateSamplerDescriptor(ngfx::device_t* device, IRDescriptorTableEntry** descriptor);
            void         FreeResourceDescriptor(ngfx::device_t* device, u32 index);
            void         FreeSamplerDescriptor(ngfx::device_t* device, u32 index);
            MTL::Buffer* GetResourceDescriptorBuffer(nmetal::device_t* device);
            MTL::Buffer* GetSamplerDescriptorBuffer(nmetal::device_t* device);
            u32          GetAllocationSize(nmetal::device_t* device, const texture_desc_t& desc);
            void         MakeResident(nmetal::device_t* device, const MTL::Allocation* allocation);
            void         Evict(nmetal::device_t* device, const MTL::Allocation* allocation);
            bool         DumpMemoryStats(ngfx::device_t* device, const char* file);
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
