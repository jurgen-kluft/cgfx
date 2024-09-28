#ifndef __CGFX_GFX_BUFFER_H__
#define __CGFX_GFX_BUFFER_H__

#include "cgfx/gfx_resource.h"

namespace ncore
{
    namespace ngfx
    {
        class IGfxBuffer : public IGfxResource
        {
        public:
            const GfxBufferDesc& GetDesc() const { return m_desc; }

            virtual bool  IsBuffer() const { return true; }
            virtual void* GetCpuAddress()                      = 0;
            virtual u64   GetGpuAddress()                      = 0;
            virtual u32   GetRequiredStagingBufferSize() const = 0;

        protected:
            GfxBufferDesc m_desc = {};
        };

    }  // namespace ngfx
}  // namespace ncore

#endif
