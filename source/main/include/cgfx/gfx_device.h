#ifndef __CGFX_GFX_DEVICE_H__
#define __CGFX_GFX_DEVICE_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
    #pragma once
#endif

#include "cgfx/gfx_defines.h"
#include "callocator/c_allocator_ocs.h"

namespace ncore
{
    class alloc_t;

    namespace ngfx
    {
        struct device_t
        {
            D_GFX_OCS_COMPONENT;
            u64                m_frameID;
            device_desc_t      m_desc;
            enums::vendor_t    m_vendor;
            alloc_t*           m_allocator;
            nocs::allocator_t* m_allocatorOCS;
        };

        device_t* CreateDevice(alloc_t* allocator, const device_desc_t& desc);
        bool      Create(device_t* device);
        void*     GetHandle(device_t* device);
        void      BeginFrame(device_t* device);
        void      EndFrame(device_t* device);
        
        // NOTE: Gfx doesn't really need to have an 'object', everything is a component, we just need:
        // - CreateInstance<C>, return C*
        // - DestroyInstance<C>
        // - AddComponent<C1, C2>(C1 const* cp)
        // - GetComponent<C1, C2>(C1 const* cp), return C2*
        // - HasComponent<C1, C2>(C1 const* cp)
        // - SetTag<C>(C const* cp, u16 tag)
        // - RemTag<C>(C const* cp, u16 tag)
        // - HasTag<C>(C const* cp, u16 tag)

        template <typename T, typename C>
        T* GetObject(device_t* device, C* pComponent)
        {
            return device->m_allocatorOCS->get_object<T, C>(pComponent);
        }

        template <typename T>
        T* CreateObject(device_t* device, const char* name)
        {
            T* resource = device->m_allocatorOCS->create_object<T>();
            AttachName(device, resource, name);
            return resource;
        }

        template <typename T>
        void RemoveObject(device_t* device, T* pResource)
        {
            device->m_allocatorOCS->destroy_object<T>(pResource);
        }

        template <typename T, typename C>
        C* AddComponent(device_t* device, T* pResource)
        {
            return device->m_allocatorOCS->add_component<C, T>(pResource);
        }

        template <typename C1, typename C2>
        C2* AddAnotherComponent(device_t* device, C1* pCp1)
        {
            return device->m_allocatorOCS->add_component<resource_t, C1, C2>(pCp1);
        }

        template <typename T, typename C>
        C* GetComponent(device_t* device, T* pResource)
        {
            return device->m_allocatorOCS->get_component<C, T>(pResource);
        }

        template <typename C1, typename C2>
        C2* GetOtherComponent(device_t* device, C1 const* pComponent1)
        {
            return device->m_allocatorOCS->get_component<resource_t, C1, C2>(pComponent1);
        }

        template <typename C1, typename C2>
        C2 const* GetOtherComponent(device_t const* device, C1 const* pComponent1)
        {
            return device->m_allocatorOCS->get_component<resource_t, C1, C2>(pComponent1);
        }

        template <typename C1, typename C2>
        void RemoveOtherComponent(device_t* device, C1* pComponent1)
        {
            device->m_allocatorOCS->rem_component<resource_t, C1, C2>(pComponent1);
        }


        template <typename T, typename C>
        bool HasComponent(device_t* device, T* pResource)
        {
            return device->m_allocatorOCS->has_component<C, T>(pResource);
        }

        template <typename T>
        void SetTag(device_t* device, T* pResource, u16 tag)
        {
            device->m_allocatorOCS->set_tag<T>(pResource, tag);
        }

        template <typename T>
        bool HasTag(device_t* device, T* pResource, u16 tag)
        {
            return device->m_allocatorOCS->has_tag<T>(pResource, tag);
        }

        template <typename T>
        void AttachName(device_t* device, T* pResource, const char* name)
        {
            name_t* n = device->m_allocatorOCS->add_component<name_t, T>(pResource);
            SetName(n, name);
        }

        // Debug
        bool DumpMemoryStats(device_t* device, const char* file);

    }  // namespace ngfx
}  // namespace ncore

#endif
