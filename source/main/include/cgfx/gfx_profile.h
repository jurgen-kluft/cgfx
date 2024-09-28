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
        void BeginMPGpuEvent(IGfxCommandList* pCommandList, const eastl::string& event_name);
        void EndMPGpuEvent(IGfxCommandList* pCommandList);

        class RenderEvent
        {
        public:
            RenderEvent(IGfxCommandList* pCommandList, const eastl::string& event_name)
                : m_pCommandList(pCommandList)
            {
                pCommandList->BeginEvent(event_name);
            }

            ~RenderEvent() { m_pCommandList->EndEvent(); }

        private:
            IGfxCommandList* m_pCommandList;
        };

        class MPRenderEvent
        {
        public:
            MPRenderEvent(IGfxCommandList* pCommandList, const eastl::string& event_name)
                : m_pCommandList(pCommandList)
            {
                BeginMPGpuEvent(pCommandList, event_name);
            }

            ~MPRenderEvent() { EndMPGpuEvent(m_pCommandList); }

        private:
            IGfxCommandList* m_pCommandList;
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
