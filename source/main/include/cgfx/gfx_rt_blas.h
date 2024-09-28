#ifndef __CGFX_GFX_RT_BLAS_H__
#define __CGFX_GFX_RT_BLAS_H__

#include "cgfx/gfx_resource.h"

namespace ncore
{
    namespace ngfx
    {

        class IGfxRayTracingBLAS : public IGfxResource
        {
        public:
            const GfxRayTracingBLASDesc& GetDesc() const { return m_desc; }

        protected:
            GfxRayTracingBLASDesc m_desc;
        };

    }  // namespace ngfx
}  // namespace ncore

#endif
