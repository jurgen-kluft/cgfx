#ifndef __CGFX_GFX_PROFILE_H__
#define __CGFX_GFX_PROFILE_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
    #pragma once
#endif

#include "cgfx/gfx_command_list.h"
#include "cgfx/gfx_defines.h"

namespace ncore
{
    namespace ngfx
    {
        void BeginMPGpuEvent(command_list_t* pCommandList, const char* event_name);
        void EndMPGpuEvent(command_list_t* pCommandList);

        class RenderEvent
        {
        public:
            RenderEvent(command_list_t* pCommandList, const char* event_name)
                : m_pCommandList(pCommandList)
            {
                BeginEvent(pCommandList, event_name);
            }

            ~RenderEvent() { EndEvent(m_pCommandList); }

        private:
            command_list_t* m_pCommandList;
        };

        class MPRenderEvent
        {
        public:
            MPRenderEvent(command_list_t* pCommandList, const char* event_name)
                : m_pCommandList(pCommandList)
            {
                BeginMPGpuEvent(pCommandList, event_name);
            }

            ~MPRenderEvent() { EndMPGpuEvent(m_pCommandList); }

        private:
            command_list_t* m_pCommandList;
        };

#if 1
    #define GPU_EVENT_DEBUG(pCommandList, event_name)    RenderEvent __render_event__(pCommandList, event_name)
    #define GPU_EVENT_PROFILER(pCommandList, event_name) MPRenderEvent __mp_event__(pCommandList, event_name)
    #define GPU_EVENT(pCommandList, event_name)    \
        GPU_EVENT_DEBUG(pCommandList, event_name); \
        GPU_EVENT_PROFILER(pCommandList, event_name)
#else
    #define GPU_EVENT(pCommandList, event_name)
    #define GPU_EVENT_DEBUG(pCommandList, event_name)
    #define GPU_EVENT_PROFILER(pCommandList, event_name)
#endif

    }  // namespace ngfx
}  // namespace ncore

#endif
