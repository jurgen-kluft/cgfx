#ifndef __CGFX_GFX_DEVICE_H__
#define __CGFX_GFX_DEVICE_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
    #pragma once
#endif

#include "cgfx/gfx_defines.h"
#include "cgfx/gfx_name.h"

#include "callocator/c_allocator_cs.h"
#include "callocator/c_allocator_stack.h"
#include "callocator/c_allocator_frame.h"

namespace ncore
{
    class alloc_t;

    namespace ngfx
    {
        struct device_t
        {
            D_GFX_CS_COMPONENT_SET(enums::ComponentDevice);
            u64             m_frameID;
            device_desc_t   m_desc;
            enums::vendor_t m_vendor;
            alloc_t*        m_main_alloc;
            stack_alloc_t*  m_stack_alloc;
            frame_alloc_t*  m_frame_alloc;
            cs_alloc_t*     m_cs_alloc;
            DCORE_CLASS_PLACEMENT_NEW_DELETE
        };

        device_t* CreateDevice(alloc_t* main_alloc, stack_alloc_t* stack_alloc, frame_alloc_t* frame_alloc, const device_desc_t& desc);
        bool      Create(device_t* device);
        void      BeginFrame(device_t* device);
        void      EndFrame(device_t* device);

        template <typename C>
        void AttachName(device_t* device, C* pComponent, const char* name)
        {
            name_t* n = device->m_cs_alloc->create_component<C, name_t>(pComponent);
            SetName(n, name);
        }

        template <typename C>
        C* CreateInstance(device_t* device, const char* name)
        {
            C* instance = device->m_cs_alloc->new_instance<C>();
            AttachName<C>(device, instance, name);
            return instance;
        }

        template <typename C>
        void DestroyInstance(device_t* device, C* pInstance)
        {
            device->m_cs_alloc->destroy_instance<C>(pInstance);
        }

        template <typename C1, typename C2>
        C2* CreateComponent(device_t* device, C1* pComponent)
        {
            return device->m_cs_alloc->create_component<C1, C2>(pComponent);
        }

        template <typename C1, typename C2>
        void DestroyComponent(device_t* device, C1* pComponent1)
        {
            device->m_cs_alloc->destroy_component<C1, C2>(pComponent1);
        }

        template <typename C1, typename C2>
        bool HasComponent(device_t* device, C1* pComponent)
        {
            return device->m_cs_alloc->has_component<C1, C2>(pComponent);
        }

        template <typename C1, typename C2>
        C2* GetComponent(device_t* device, C1 const* pComponent)
        {
            return device->m_cs_alloc->get_component<C1, C2>(pComponent);
        }

        template <typename C1, typename C2>
        C2 const* GetComponent(device_t const* device, C1 const* pComponent)
        {
            return device->m_cs_alloc->get_component<C1, C2>(pComponent);
        }

        template <typename C>
        void SetTag(device_t* device, C* pComponent, u16 tag)
        {
            device->m_cs_alloc->add_tag<C>(pComponent, tag);
        }

        template <typename C>
        bool HasTag(device_t* device, C* pComponent, u16 tag)
        {
            return device->m_cs_alloc->has_tag<C>(pComponent, tag);
        }

        // Debug
        bool DumpMemoryStats(device_t* device, const char* file);

    }  // namespace ngfx
}  // namespace ncore

#endif
