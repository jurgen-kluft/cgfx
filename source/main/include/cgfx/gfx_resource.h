#ifndef __CGFX_GFX_RESOURCE_H__
#define __CGFX_GFX_RESOURCE_H__

#include "cgfx/gfx_defines.h"

namespace ncore
{
    namespace ngfx
    {
        class IGfxDevice;

        class IGfxResource
        {
        public:
            virtual ~IGfxResource() {}

            virtual void* GetHandle() const = 0;
            virtual bool  IsTexture() const { return false; }
            virtual bool  IsBuffer() const { return false; }

            IGfxDevice*          GetDevice() const { return m_pDevice; }
            const eastl::string& GetName() const { return m_name; }

        protected:
            IGfxDevice*   m_pDevice = nullptr;
            eastl::string m_name;
        };

    }  // namespace ngfx
}  // namespace ncore

#endif
