#ifndef __CGFX_METAL_SHADER_REFLECTION_H__
#define __CGFX_METAL_SHADER_REFLECTION_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

namespace ncore
{
    namespace ngfx
    {
#pragma pack(push, 1)
        struct MetalShaderReflection
        {
            u32 threadsPerThreadgroup[3];
        };
#pragma pack(pop)

    }  // namespace ngfx
}  // namespace ncore
#endif
