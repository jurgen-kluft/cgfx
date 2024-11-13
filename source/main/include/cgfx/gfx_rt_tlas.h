#ifndef __CGFX_GFX_RT_TLAS_H__
#define __CGFX_GFX_RT_TLAS_H__

#include "cgfx/gfx_resource.h"

namespace ncore
{
    namespace ngfx
    {
        class IGfxRayTracingTLAS : public IGfxResource
        {
        public:
            const GfxRayTracing::TLASDesc& GetDesc() const { return m_desc; }

        protected:
            GfxRayTracing::TLASDesc m_desc;
        };

    }  // namespace ngfx
}  // namespace ncore

#endif
