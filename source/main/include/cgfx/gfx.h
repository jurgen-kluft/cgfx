#ifndef __CGFX_GFX_H__
#define __CGFX_GFX_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
    #pragma once
#endif

#include "cgfx/gfx_defines.h"
#include "cgfx/gfx_device.h"
#include "cgfx/gfx_buffer.h"
#include "cgfx/gfx_texture.h"
#include "cgfx/gfx_command_list.h"
#include "cgfx/gfx_fence.h"
#include "cgfx/gfx_shader.h"
#include "cgfx/gfx_pipeline_state.h"
#include "cgfx/gfx_swapchain.h"
#include "cgfx/gfx_descriptor.h"
#include "cgfx/gfx_heap.h"
#include "cgfx/gfx_rt_blas.h"
#include "cgfx/gfx_rt_tlas.h"

namespace ncore
{
    namespace ngfx
    {
        device_t* CreateDevice(alloc_t* allocator, const device_desc_t& desc);
        u32       GetFormatRowPitch(enums::format format, u32 width);
        u32       GetFormatBlockWidth(enums::format format);
        u32       GetFormatBlockHeight(enums::format format);
        u32       GetFormatComponentNum(enums::format format);
        bool      IsDepthFormat(enums::format format);
        bool      IsStencilFormat(enums::format format);
        bool      IsSRGBFormat(enums::format format);
        u32       CalcSubresource(const texture_desc_t& desc, u32 mip, u32 slice);
        void      DecomposeSubresource(const texture_desc_t& desc, u32 subresource, u32& mip, u32& slice);

    }  // namespace ngfx
}  // namespace ncore

#endif
