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
        IGfxDevice* CreateGfxDevice(const GfxDeviceDesc& desc);
        u32         GetFormatRowPitch(GfxFormat format, u32 width);
        u32         GetFormatBlockWidth(GfxFormat format);
        u32         GetFormatBlockHeight(GfxFormat format);
        u32         GetFormatComponentNum(GfxFormat format);
        bool        IsDepthFormat(GfxFormat format);
        bool        IsStencilFormat(GfxFormat format);
        bool        IsSRGBFormat(GfxFormat format);
        u32         CalcSubresource(const GfxTextureDesc& desc, u32 mip, u32 slice);
        void        DecomposeSubresource(const GfxTextureDesc& desc, u32 subresource, u32& mip, u32& slice);

    }  // namespace ngfx
}  // namespace ncore

#endif
