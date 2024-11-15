#include "cgfx/gfx.h"
// #include "microprofile/microprofile.h"

namespace ncore
{
    namespace ngfx
    {
        void BeginMPGpuEvent(command_list_t* pCommandList, const char* event_name)
        {
#if MICROPROFILE_GPU_TIMERS
            static const u32 EVENT_COLOR[] = {
              MP_LIGHTCYAN4, MP_SKYBLUE2, MP_SEAGREEN4, MP_LIGHTGOLDENROD4, MP_BROWN3, MP_MEDIUMPURPLE2, MP_SIENNA, MP_LIMEGREEN, MP_MISTYROSE, MP_LIGHTYELLOW,
            };

            u32 color_count = sizeof(EVENT_COLOR) / sizeof(EVENT_COLOR[0]);
            u32 color       = EVENT_COLOR[XXH32(event_name.c_str(), strlen(event_name.c_str()), 0) % color_count];

            MicroProfileToken token = MicroProfileGetToken("GPU", event_name.c_str(), color, MicroProfileTokenTypeGpu);

            MicroProfileEnterGpu(token, pCommandList->GetProfileLog());
#endif
        }

        void EndMPGpuEvent(command_list_t* pCommandList)
        {
#if MICROPROFILE_GPU_TIMERS
            MicroProfileLeaveGpu(pCommandList->GetProfileLog());
#endif
        }
    }  // namespace ngfx
}  // namespace ncore
