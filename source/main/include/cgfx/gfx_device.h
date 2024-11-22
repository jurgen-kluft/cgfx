#ifndef __CGFX_GFX_DEVICE_H__
#define __CGFX_GFX_DEVICE_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
    #pragma once
#endif

#include "cgfx/gfx_defines.h"
#include "callocator/c_allocator_cs.h"

namespace ncore
{
    class alloc_t;

    namespace ngfx
    {
        struct device_t
        {
            D_GFX_OCS_COMPONENT_SET(enums::ComponentDevice);
            u64               m_frameID;
            device_desc_t     m_desc;
            enums::vendor_t   m_vendor;
            alloc_t*          m_allocator;
            ncs::allocator_t* m_allocatorCS;
        };

        device_t* CreateDevice(alloc_t* allocator, const device_desc_t& desc);
        bool      Create(device_t* device);
        void*     GetHandle(device_t* device);
        void      BeginFrame(device_t* device);
        void      EndFrame(device_t* device);

        template <typename C>
        void AttachName(device_t* device, C* pComponent, const char* name)
        {
            name_t* n = device->m_allocatorCS->add_component<name_t, C>(pComponent);
            SetName(n, name);
        }

        template <typename C>
        C* CreateInstance(device_t* device, const char* name)
        {
            C* instance = device->m_allocatorCS->create_instance<C>();
            AttachName<C>(device, instance, name);
            return instance;
        }

        template <typename C>
        void DestroyInstance(device_t* device, C* pInstance)
        {
            device->m_allocatorCS->destroy_instance<C>(pInstance);
        }

        template <typename C1, typename C2>
        C2* CreateComponent(device_t* device, C1* pComponent)
        {
            return device->m_allocatorCS->create_component<C1, C2>(pComponent);
        }

        template <typename C1, typename C2>
        void DestroyComponent(device_t* device, C1* pComponent1)
        {
            device->m_allocatorCS->rem_component<C1, C2>(pComponent1);
        }

        template <typename C1, typename C2>
        bool HasComponent(device_t* device, C1* pComponent)
        {
            return device->m_allocatorCS->has_component<C1, C2>(pComponent);
        }

        template <typename C1, typename C2>
        C2* GetComponent(device_t* device, C1 const* pComponent)
        {
            return device->m_allocatorCS->get_component<C1, C2>(pComponent);
        }

        template <typename C1, typename C2>
        C2 const* GetComponent(device_t const* device, C1 const* pComponent)
        {
            return device->m_allocatorCS->get_component<C1, C2>(pComponent);
        }

        template <typename C>
        void SetTag(device_t* device, C* pComponent, u16 tag)
        {
            device->m_allocatorCS->set_tag<C>(pComponent, tag);
        }

        template <typename C>
        bool HasTag(device_t* device, C* pComponent, u16 tag)
        {
            return device->m_allocatorCS->has_tag<C>(pComponent, tag);
        }

        // Debug
        bool DumpMemoryStats(device_t* device, const char* file);

    }  // namespace ngfx
}  // namespace ncore

#endif
