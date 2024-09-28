#ifndef __CGFX_GFX_PIPELINE_STATE_H__
#define __CGFX_GFX_PIPELINE_STATE_H__

#include "cgfx/gfx_resource.h"

namespace ncore
{
    namespace ngfx
    {

        class IGfxPipelineState : public IGfxResource
        {
        public:
            GfxPipelineType GetType() const { return m_type; }

            virtual bool Create() = 0;

        protected:
            GfxPipelineType m_type;
        };

    }  // namespace ngfx
}  // namespace ncore

#endif
