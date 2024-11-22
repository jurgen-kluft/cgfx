#ifndef __CGFX_D3D12_HEADER_H__
#define __CGFX_D3D12_HEADER_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
    #pragma once
#endif

#ifdef TARGET_PC
    #include "cd3d12/d3d12.h"
    #include "cd3d12/d3dx12.h"
    #include "cd3d12/ma/D3D12MemAlloc.h"
    #include "dxgi1_6.h"
#endif

#include "cbase/c_debug.h"
#include "cgfx/gfx_defines.h"
#include "cgfx/gfx_texture.h"

namespace ncore
{
    namespace ngfx
    {

#ifndef SAFE_RELEASE
    #define SAFE_RELEASE(p) \
        if (p)              \
        {                   \
            p->Release();   \
            p = nullptr;    \
        }
#endif

#ifdef TARGET_PC
        struct D3D12Descriptor
        {
            D3D12_CPU_DESCRIPTOR_HANDLE cpu_handle = {};
            D3D12_GPU_DESCRIPTOR_HANDLE gpu_handle = {};
            u32                         index      = GFX_INVALID_RESOURCE;
        };

        inline bool IsNullDescriptor(const D3D12Descriptor& descriptor) { return descriptor.cpu_handle.ptr == 0 && descriptor.gpu_handle.ptr == 0; }

        inline D3D12_HEAP_TYPE d3d12_heap_type(enums::memory memory_type)
        {
            switch (memory_type)
            {
                case enums::MemoryGpuOnly: return D3D12_HEAP_TYPE_DEFAULT;
                case enums::MemoryCpuOnly:
                case enums::MemoryCpuToGpu: return D3D12_HEAP_TYPE_UPLOAD;
                case enums::MemoryGpuToCpu: return D3D12_HEAP_TYPE_READBACK;
                default: return D3D12_HEAP_TYPE_DEFAULT;
            }
        }

        inline D3D12_BARRIER_SYNC d3d12_barrier_sync(enums::access_flags flags)
        {
            D3D12_BARRIER_SYNC sync    = D3D12_BARRIER_SYNC_NONE;
            bool               discard = flags & enums::AccessDiscard;
            if (!discard)
            {
                // d3d validation : "SyncAfter bits D3D12_BARRIER_SYNC_CLEAR_UNORDERED_ACCESS_VIEW are incompatible with AccessAfter bits D3D12_BARRIER_ACCESS_NO_ACCESS"
                if (flags & enums::AccessClearUAV)
                    sync |= D3D12_BARRIER_SYNC_CLEAR_UNORDERED_ACCESS_VIEW;
            }

            if (flags & enums::AccessPresent)
                sync |= D3D12_BARRIER_SYNC_ALL;
            if (flags & enums::AccessRTV)
                sync |= D3D12_BARRIER_SYNC_RENDER_TARGET;
            if (flags & enums::AccessMaskDSV)
                sync |= D3D12_BARRIER_SYNC_DEPTH_STENCIL;
            if (flags & enums::AccessMaskVS)
                sync |= D3D12_BARRIER_SYNC_VERTEX_SHADING;
            if (flags & enums::AccessMaskPS)
                sync |= D3D12_BARRIER_SYNC_PIXEL_SHADING;
            if (flags & enums::AccessMaskCS)
                sync |= D3D12_BARRIER_SYNC_COMPUTE_SHADING;
            if (flags & enums::AccessMaskCopy)
                sync |= D3D12_BARRIER_SYNC_COPY;
            if (flags & enums::AccessShadingRate)
                sync |= D3D12_BARRIER_SYNC_PIXEL_SHADING;
            if (flags & enums::AccessIndexBuffer)
                sync |= D3D12_BARRIER_SYNC_INDEX_INPUT;
            if (flags & enums::AccessIndirectArgs)
                sync |= D3D12_BARRIER_SYNC_EXECUTE_INDIRECT;
            if (flags & enums::AccessMaskAS)
                sync |= D3D12_BARRIER_SYNC_BUILD_RAYTRACING_ACCELERATION_STRUCTURE;

            return sync;
        }

        inline D3D12_BARRIER_ACCESS d3d12_barrier_access(enums::access_flags flags)
        {
            if (flags & enums::AccessDiscard)
            {
                return D3D12_BARRIER_ACCESS_NO_ACCESS;
            }

            D3D12_BARRIER_ACCESS access = D3D12_BARRIER_ACCESS_COMMON;

            if (flags & enums::AccessRTV)
                access |= D3D12_BARRIER_ACCESS_RENDER_TARGET;
            if (flags & enums::AccessDSV)
                access |= D3D12_BARRIER_ACCESS_DEPTH_STENCIL_WRITE;
            if (flags & enums::AccessDSVReadOnly)
                access |= D3D12_BARRIER_ACCESS_DEPTH_STENCIL_READ;
            if (flags & enums::AccessMaskSRV)
                access |= D3D12_BARRIER_ACCESS_SHADER_RESOURCE;
            if (flags & enums::AccessMaskUAV)
                access |= D3D12_BARRIER_ACCESS_UNORDERED_ACCESS;
            if (flags & enums::AccessClearUAV)
                access |= D3D12_BARRIER_ACCESS_UNORDERED_ACCESS;
            if (flags & enums::AccessCopyDst)
                access |= D3D12_BARRIER_ACCESS_COPY_DEST;
            if (flags & enums::AccessCopySrc)
                access |= D3D12_BARRIER_ACCESS_COPY_SOURCE;
            if (flags & enums::AccessShadingRate)
                access |= D3D12_BARRIER_ACCESS_SHADING_RATE_SOURCE;
            if (flags & enums::AccessIndexBuffer)
                access |= D3D12_BARRIER_ACCESS_INDEX_BUFFER;
            if (flags & enums::AccessIndirectArgs)
                access |= D3D12_BARRIER_ACCESS_INDIRECT_ARGUMENT;
            if (flags & enums::AccessASRead)
                access |= D3D12_BARRIER_ACCESS_RAYTRACING_ACCELERATION_STRUCTURE_READ;
            if (flags & enums::AccessASWrite)
                access |= D3D12_BARRIER_ACCESS_RAYTRACING_ACCELERATION_STRUCTURE_WRITE;

            return access;
        }

        inline D3D12_BARRIER_LAYOUT d3d12_barrier_layout(enums::access_flags flags)
        {
            if (flags & enums::AccessDiscard)
                return D3D12_BARRIER_LAYOUT_UNDEFINED;
            if (flags & enums::AccessPresent)
                return D3D12_BARRIER_LAYOUT_PRESENT;
            if (flags & enums::AccessRTV)
                return D3D12_BARRIER_LAYOUT_RENDER_TARGET;
            if (flags & enums::AccessDSV)
                return D3D12_BARRIER_LAYOUT_DEPTH_STENCIL_WRITE;
            if (flags & enums::AccessDSVReadOnly)
                return D3D12_BARRIER_LAYOUT_DEPTH_STENCIL_READ;
            if (flags & enums::AccessMaskSRV)
                return D3D12_BARRIER_LAYOUT_SHADER_RESOURCE;
            if (flags & enums::AccessMaskUAV)
                return D3D12_BARRIER_LAYOUT_UNORDERED_ACCESS;
            if (flags & enums::AccessClearUAV)
                return D3D12_BARRIER_LAYOUT_UNORDERED_ACCESS;
            if (flags & enums::AccessCopyDst)
                return D3D12_BARRIER_LAYOUT_COPY_DEST;
            if (flags & enums::AccessCopySrc)
                return D3D12_BARRIER_LAYOUT_COPY_SOURCE;
            if (flags & enums::AccessShadingRate)
                return D3D12_BARRIER_LAYOUT_SHADING_RATE_SOURCE;

            ASSERT(false);
            return D3D12_BARRIER_LAYOUT_UNDEFINED;
        }

        inline DXGI_FORMAT dxgi_format(enums::format format, bool depth_srv = false, bool uav = false)
        {
            switch (format)
            {
                case enums::FORMAT_UNKNOWN: return DXGI_FORMAT_UNKNOWN;
                case enums::FORMAT_RGBA32F: return DXGI_FORMAT_R32G32B32A32_FLOAT;
                case enums::FORMAT_RGBA32UI: return DXGI_FORMAT_R32G32B32A32_UINT;
                case enums::FORMAT_RGBA32SI: return DXGI_FORMAT_R32G32B32A32_SINT;
                case enums::FORMAT_RGBA16F: return DXGI_FORMAT_R16G16B16A16_FLOAT;
                case enums::FORMAT_RGBA16UI: return DXGI_FORMAT_R16G16B16A16_UINT;
                case enums::FORMAT_RGBA16SI: return DXGI_FORMAT_R16G16B16A16_SINT;
                case enums::FORMAT_RGBA16UNORM: return DXGI_FORMAT_R16G16B16A16_UNORM;
                case enums::FORMAT_RGBA16SNORM: return DXGI_FORMAT_R16G16B16A16_SNORM;
                case enums::FORMAT_RGBA8UI: return DXGI_FORMAT_R8G8B8A8_UINT;
                case enums::FORMAT_RGBA8SI: return DXGI_FORMAT_R8G8B8A8_SINT;
                case enums::FORMAT_RGBA8UNORM: return DXGI_FORMAT_R8G8B8A8_UNORM;
                case enums::FORMAT_RGBA8SNORM: return DXGI_FORMAT_R8G8B8A8_SNORM;
                case enums::FORMAT_RGBA8SRGB: return uav ? DXGI_FORMAT_R8G8B8A8_UNORM : DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
                case enums::FORMAT_BGRA8UNORM: return DXGI_FORMAT_B8G8R8A8_UNORM;
                case enums::FORMAT_BGRA8SRGB: return uav ? DXGI_FORMAT_B8G8R8A8_UNORM : DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
                case enums::FORMAT_RGB10A2UI: return DXGI_FORMAT_R10G10B10A2_UINT;
                case enums::FORMAT_RGB10A2UNORM: return DXGI_FORMAT_R10G10B10A2_UNORM;
                case enums::FORMAT_RGB32F: return DXGI_FORMAT_R32G32B32_FLOAT;
                case enums::FORMAT_RGB32UI: return DXGI_FORMAT_R32G32B32_UINT;
                case enums::FORMAT_RGB32SI: return DXGI_FORMAT_R32G32B32_SINT;
                case enums::FORMAT_R11G11B10F: return DXGI_FORMAT_R11G11B10_FLOAT;
                case enums::FORMAT_RGB9E5: return DXGI_FORMAT_R9G9B9E5_SHAREDEXP;
                case enums::FORMAT_RG32F: return DXGI_FORMAT_R32G32_FLOAT;
                case enums::FORMAT_RG32UI: return DXGI_FORMAT_R32G32_UINT;
                case enums::FORMAT_RG32SI: return DXGI_FORMAT_R32G32_SINT;
                case enums::FORMAT_RG16F: return DXGI_FORMAT_R16G16_FLOAT;
                case enums::FORMAT_RG16UI: return DXGI_FORMAT_R16G16_UINT;
                case enums::FORMAT_RG16SI: return DXGI_FORMAT_R16G16_SINT;
                case enums::FORMAT_RG16UNORM: return DXGI_FORMAT_R16G16_UNORM;
                case enums::FORMAT_RG16SNORM: return DXGI_FORMAT_R16G16_SNORM;
                case enums::FORMAT_RG8UI: return DXGI_FORMAT_R8G8_UINT;
                case enums::FORMAT_RG8SI: return DXGI_FORMAT_R8G8_SINT;
                case enums::FORMAT_RG8UNORM: return DXGI_FORMAT_R8G8_UNORM;
                case enums::FORMAT_RG8SNORM: return DXGI_FORMAT_R8G8_SNORM;
                case enums::FORMAT_R32F: return DXGI_FORMAT_R32_FLOAT;
                case enums::FORMAT_R32UI: return DXGI_FORMAT_R32_UINT;
                case enums::FORMAT_R32SI: return DXGI_FORMAT_R32_SINT;
                case enums::FORMAT_R16F: return DXGI_FORMAT_R16_FLOAT;
                case enums::FORMAT_R16UI: return DXGI_FORMAT_R16_UINT;
                case enums::FORMAT_R16SI: return DXGI_FORMAT_R16_SINT;
                case enums::FORMAT_R16UNORM: return DXGI_FORMAT_R16_UNORM;
                case enums::FORMAT_R16SNORM: return DXGI_FORMAT_R16_SNORM;
                case enums::FORMAT_R8UI: return DXGI_FORMAT_R8_UINT;
                case enums::FORMAT_R8SI: return DXGI_FORMAT_R8_SINT;
                case enums::FORMAT_R8UNORM: return DXGI_FORMAT_R8_UNORM;
                case enums::FORMAT_R8SNORM: return DXGI_FORMAT_R8_SNORM;
                case enums::FORMAT_D32F: return depth_srv ? DXGI_FORMAT_R32_FLOAT : DXGI_FORMAT_D32_FLOAT;
                case enums::FORMAT_D32FS8: return depth_srv ? DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS : DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
                case enums::FORMAT_D16: return depth_srv ? DXGI_FORMAT_R16_UNORM : DXGI_FORMAT_D16_UNORM;
                case enums::FORMAT_BC1UNORM: return DXGI_FORMAT_BC1_UNORM;
                case enums::FORMAT_BC1SRGB: return DXGI_FORMAT_BC1_UNORM_SRGB;
                case enums::FORMAT_BC2UNORM: return DXGI_FORMAT_BC2_UNORM;
                case enums::FORMAT_BC2SRGB: return DXGI_FORMAT_BC2_UNORM_SRGB;
                case enums::FORMAT_BC3UNORM: return DXGI_FORMAT_BC3_UNORM;
                case enums::FORMAT_BC3SRGB: return DXGI_FORMAT_BC3_UNORM_SRGB;
                case enums::FORMAT_BC4UNORM: return DXGI_FORMAT_BC4_UNORM;
                case enums::FORMAT_BC4SNORM: return DXGI_FORMAT_BC4_SNORM;
                case enums::FORMAT_BC5UNORM: return DXGI_FORMAT_BC5_UNORM;
                case enums::FORMAT_BC5SNORM: return DXGI_FORMAT_BC5_SNORM;
                case enums::FORMAT_BC6U16F: return DXGI_FORMAT_BC6H_UF16;
                case enums::FORMAT_BC6S16F: return DXGI_FORMAT_BC6H_SF16;
                case enums::FORMAT_BC7UNORM: return DXGI_FORMAT_BC7_UNORM;
                case enums::FORMAT_BC7SRGB: return DXGI_FORMAT_BC7_UNORM_SRGB;
                default: ASSERT(false); return DXGI_FORMAT_UNKNOWN;
            }
        }
        inline DXGI_FORMAT dxgi_format(enums::format_t format, bool depth_srv = false, bool uav = false) { return dxgi_format(enums::cast<enums::format>(format), depth_srv, uav); }

        inline D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE d3d12_render_pass_loadop(enums::load_op loadOp)
        {
            switch (loadOp)
            {
                case enums::LoadOpLoad: return D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_PRESERVE;
                case enums::LoadOpClear: return D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_CLEAR;
                case enums::LoadOpDontCare: return D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_DISCARD;
                default: return D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_PRESERVE;
            }
        }

        inline D3D12_RENDER_PASS_ENDING_ACCESS_TYPE d3d12_render_pass_storeop(enums::store_op storeOp)
        {
            switch (storeOp)
            {
                case enums::StoreOpStore: return D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_PRESERVE;
                case enums::StoreOpDontCare: return D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_DISCARD;
                default: return D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_PRESERVE;
            }
        }

        inline D3D12_BLEND d3d12_blend(enums::blendfactor blend_factor)
        {
            switch (blend_factor)
            {
                case enums::BlendFactorZero: return D3D12_BLEND_ZERO;
                case enums::BlendFactorOne: return D3D12_BLEND_ONE;
                case enums::BlendFactorSrcColor: return D3D12_BLEND_SRC_COLOR;
                case enums::BlendFactorInvSrcColor: return D3D12_BLEND_INV_SRC_COLOR;
                case enums::BlendFactorSrcAlpha: return D3D12_BLEND_SRC_ALPHA;
                case enums::BlendFactorInvSrcAlpha: return D3D12_BLEND_INV_SRC_ALPHA;
                case enums::BlendFactorDstAlpha: return D3D12_BLEND_DEST_ALPHA;
                case enums::BlendFactorInvDstAlpha: return D3D12_BLEND_INV_DEST_ALPHA;
                case enums::BlendFactorDstColor: return D3D12_BLEND_DEST_COLOR;
                case enums::BlendFactorInvDstColor: return D3D12_BLEND_INV_DEST_COLOR;
                case enums::BlendFactorSrcAlphaClamp: return D3D12_BLEND_SRC_ALPHA_SAT;
                case enums::BlendFactorConstantFactor: return D3D12_BLEND_BLEND_FACTOR;
                case enums::BlendFactorInvConstantFactor: return D3D12_BLEND_INV_BLEND_FACTOR;
                default: return D3D12_BLEND_ONE;
            }
        }
        inline D3D12_BLEND d3d12_blend(enums::blendfactor_t blend_factor) { return d3d12_blend(enums::cast<enums::blendfactor>(blend_factor)); }

        inline D3D12_BLEND_OP d3d12_blend_op(enums::blendop blend_op)
        {
            switch (blend_op)
            {
                case enums::BlendOpAdd: return D3D12_BLEND_OP_ADD;
                case enums::BlendOpSubtract: return D3D12_BLEND_OP_SUBTRACT;
                case enums::BlendOpReverseSubtract: return D3D12_BLEND_OP_REV_SUBTRACT;
                case enums::BlendOpMin: return D3D12_BLEND_OP_MIN;
                case enums::BlendOpMax: return D3D12_BLEND_OP_MAX;
                default: return D3D12_BLEND_OP_ADD;
            }
        }
        inline D3D12_BLEND_OP d3d12_blend_op(enums::blendop_t blend_op) { return d3d12_blend_op(enums::cast<enums::blendop>(blend_op)); }

        inline D3D12_RENDER_TARGET_BLEND_DESC d3d12_rt_blend_desc(const blendstate_t& blendState)
        {
            D3D12_RENDER_TARGET_BLEND_DESC desc = {};
            desc.BlendEnable                    = blendState.blend_enable;
            desc.SrcBlend                       = d3d12_blend(blendState.color_src);
            desc.DestBlend                      = d3d12_blend(blendState.color_dst);
            desc.BlendOp                        = d3d12_blend_op(blendState.color_op);
            desc.SrcBlendAlpha                  = d3d12_blend(blendState.alpha_src);
            desc.DestBlendAlpha                 = d3d12_blend(blendState.alpha_dst);
            desc.BlendOpAlpha                   = d3d12_blend_op(blendState.alpha_op);
            desc.RenderTargetWriteMask          = blendState.write_mask;

            return desc;
        }

        inline D3D12_BLEND_DESC d3d12_blend_desc(const blendstate_t* blendStates)
        {
            D3D12_BLEND_DESC desc       = {};
            desc.AlphaToCoverageEnable  = false;
            desc.IndependentBlendEnable = true;

            for (int i = 0; i < 8; ++i)
            {
                desc.RenderTarget[i] = d3d12_rt_blend_desc(blendStates[i]);
            }

            return desc;
        }

        inline D3D12_CULL_MODE d3d12_cull_mode(enums::cullmode cull_mode)
        {
            switch (cull_mode)
            {
                case enums::CullNone: return D3D12_CULL_MODE_NONE;
                case enums::CullFront: return D3D12_CULL_MODE_FRONT;
                case enums::CullBack: return D3D12_CULL_MODE_BACK;
                default: return D3D12_CULL_MODE_NONE;
            }
        }
        inline D3D12_CULL_MODE d3d12_cull_mode(enums::cullmode_t cull_mode) { return d3d12_cull_mode(enums::cast<enums::cullmode>(cull_mode)); }

        inline D3D12_RASTERIZER_DESC d3d12_rasterizer_desc(const rasterizer_state_t& rasterizerState)
        {
            D3D12_RASTERIZER_DESC desc = {};
            desc.FillMode              = rasterizerState.wireframe ? D3D12_FILL_MODE_WIREFRAME : D3D12_FILL_MODE_SOLID;
            desc.CullMode              = d3d12_cull_mode(rasterizerState.cull_mode);
            desc.FrontCounterClockwise = rasterizerState.front_ccw;
            desc.DepthBias             = (INT)rasterizerState.depth_bias;
            desc.DepthBiasClamp        = rasterizerState.depth_bias_clamp;
            desc.SlopeScaledDepthBias  = rasterizerState.depth_slope_scale;
            desc.DepthClipEnable       = rasterizerState.depth_clip;
            desc.AntialiasedLineEnable = rasterizerState.line_aa;
            desc.ConservativeRaster    = rasterizerState.conservative_raster ? D3D12_CONSERVATIVE_RASTERIZATION_MODE_ON : D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

            return desc;
        }

        inline D3D12_COMPARISON_FUNC d3d12_compare_func(enums::comparefunc func)
        {
            switch (func)
            {
                case enums::CompareFuncNever: return D3D12_COMPARISON_FUNC_NEVER;
                case enums::CompareFuncLess: return D3D12_COMPARISON_FUNC_LESS;
                case enums::CompareFuncEqual: return D3D12_COMPARISON_FUNC_EQUAL;
                case enums::CompareFuncLessEqual: return D3D12_COMPARISON_FUNC_LESS_EQUAL;
                case enums::CompareFuncGreater: return D3D12_COMPARISON_FUNC_GREATER;
                case enums::CompareFuncNotEqual: return D3D12_COMPARISON_FUNC_NOT_EQUAL;
                case enums::CompareFuncGreaterEqual: return D3D12_COMPARISON_FUNC_GREATER_EQUAL;
                case enums::CompareFuncAlways: return D3D12_COMPARISON_FUNC_ALWAYS;
                default: return D3D12_COMPARISON_FUNC_NONE;
            }
        }
        inline D3D12_COMPARISON_FUNC d3d12_compare_func(enums::comparefunc_t func) { return d3d12_compare_func(enums::cast<enums::comparefunc>(func)); }

        inline D3D12_STENCIL_OP d3d12_stencil_op(enums::stencil stencil_op)
        {
            switch (stencil_op)
            {
                case enums::StencilKeep: return D3D12_STENCIL_OP_KEEP;
                case enums::StencilZero: return D3D12_STENCIL_OP_ZERO;
                case enums::StencilReplace: return D3D12_STENCIL_OP_REPLACE;
                case enums::StencilIncreaseClamp: return D3D12_STENCIL_OP_INCR_SAT;
                case enums::StencilDecreaseClamp: return D3D12_STENCIL_OP_DECR_SAT;
                case enums::StencilInvert: return D3D12_STENCIL_OP_INVERT;
                case enums::StencilIncreaseWrap: return D3D12_STENCIL_OP_INCR;
                case enums::StencilDecreaseWrap: return D3D12_STENCIL_OP_DECR;
                default: return D3D12_STENCIL_OP_KEEP;
            }
        }
        inline D3D12_STENCIL_OP d3d12_stencil_op(enums::stencil_t stencil_op) { return d3d12_stencil_op(enums::cast<enums::stencil>(stencil_op)); }

        inline D3D12_DEPTH_STENCILOP_DESC d3d12_depth_stencil_op(const depth_stencilop_t& depthStencilOp)
        {
            D3D12_DEPTH_STENCILOP_DESC desc = {};
            desc.StencilFailOp              = d3d12_stencil_op(depthStencilOp.stencil_fail);
            desc.StencilDepthFailOp         = d3d12_stencil_op(depthStencilOp.depth_fail);
            desc.StencilPassOp              = d3d12_stencil_op(depthStencilOp.pass);
            desc.StencilFunc                = d3d12_compare_func(depthStencilOp.stencil_func);

            return desc;
        }

        inline D3D12_DEPTH_STENCIL_DESC d3d12_depth_stencil_desc(const depth_stencilstate_t& depthStencilState)
        {
            D3D12_DEPTH_STENCIL_DESC desc = {};
            desc.DepthEnable              = depthStencilState.depth_test;
            desc.DepthWriteMask           = depthStencilState.depth_write ? D3D12_DEPTH_WRITE_MASK_ALL : D3D12_DEPTH_WRITE_MASK_ZERO;
            desc.DepthFunc                = d3d12_compare_func(depthStencilState.depth_func);
            desc.StencilEnable            = depthStencilState.stencil_test;
            desc.StencilReadMask          = depthStencilState.stencil_read_mask;
            desc.StencilWriteMask         = depthStencilState.stencil_write_mask;
            desc.FrontFace                = d3d12_depth_stencil_op(depthStencilState.front);
            desc.BackFace                 = d3d12_depth_stencil_op(depthStencilState.back);

            return desc;
        }

        inline D3D12_PRIMITIVE_TOPOLOGY_TYPE d3d12_topology_type(enums::primitive primitive_type)
        {
            switch (primitive_type)
            {
                case enums::PrimitivePointList: return D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
                case enums::PrimitiveLineList:
                case enums::PrimitiveLineStrip: return D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
                case enums::PrimitiveTriangleList:
                case enums::PrimitiveTriangleTrip: return D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
                default: return D3D12_PRIMITIVE_TOPOLOGY_TYPE_UNDEFINED;
            }
        }

        inline D3D_PRIMITIVE_TOPOLOGY d3d12_primitive_topology(enums::primitive primitive_type)
        {
            switch (primitive_type)
            {
                case enums::PrimitivePointList: return D3D_PRIMITIVE_TOPOLOGY_POINTLIST;
                case enums::PrimitiveLineList: return D3D_PRIMITIVE_TOPOLOGY_LINELIST;
                case enums::PrimitiveLineStrip: return D3D_PRIMITIVE_TOPOLOGY_LINESTRIP;
                case enums::PrimitiveTriangleList: return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
                case enums::PrimitiveTriangleTrip: return D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
                default: return D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
            }
        }

        inline D3D12_FILTER_REDUCTION_TYPE d3d12_filter_reduction_type(enums::sampler_reduction_mode mode)
        {
            switch (mode)
            {
                case enums::SamplerReductionModeStandard: return D3D12_FILTER_REDUCTION_TYPE_STANDARD;
                case enums::SamplerReductionModeCompare: return D3D12_FILTER_REDUCTION_TYPE_COMPARISON;
                case enums::SamplerReductionModeMin: return D3D12_FILTER_REDUCTION_TYPE_MINIMUM;
                case enums::SamplerReductionModeMax: return D3D12_FILTER_REDUCTION_TYPE_MAXIMUM;
                default: return D3D12_FILTER_REDUCTION_TYPE_STANDARD;
            }
        }
        inline D3D12_FILTER_REDUCTION_TYPE d3d12_filter_reduction_type(enums::sampler_reduction_mode_t mode) { return d3d12_filter_reduction_type(enums::cast<enums::sampler_reduction_mode>(mode)); }

        inline D3D12_FILTER_TYPE d3d12_filter_type(enums::filter filter)
        {
            switch (filter)
            {
                case enums::FilterPoint: return D3D12_FILTER_TYPE_POINT;
                case enums::FilterLinear: return D3D12_FILTER_TYPE_LINEAR;
                default: return D3D12_FILTER_TYPE_POINT;
            }
        }
        inline D3D12_FILTER_TYPE d3d12_filter_type(enums::filter_t filter) { return d3d12_filter_type(enums::cast<enums::filter>(filter)); }

        inline D3D12_FILTER d3d12_filter(const sampler_desc_t& desc)
        {
            D3D12_FILTER_REDUCTION_TYPE reduction = d3d12_filter_reduction_type(desc.reduction_mode);

            if (desc.enable_anisotropy)
            {
                return D3D12_ENCODE_ANISOTROPIC_FILTER(reduction);
            }
            else
            {
                D3D12_FILTER_TYPE min_filter = d3d12_filter_type(desc.min_filter);
                D3D12_FILTER_TYPE mag_filter = d3d12_filter_type(desc.mag_filter);
                D3D12_FILTER_TYPE mip_filter = d3d12_filter_type(desc.mip_filter);

                return D3D12_ENCODE_BASIC_FILTER(min_filter, mag_filter, mip_filter, reduction);
            }
        }

        inline D3D12_TEXTURE_ADDRESS_MODE d3d12_address_mode(enums::sampler_address_mode mode)
        {
            switch (mode)
            {
                case enums::SamplerAddressModeRepeat: return D3D12_TEXTURE_ADDRESS_MODE_WRAP;
                case enums::SamplerAddressModeMirroredRepeat: return D3D12_TEXTURE_ADDRESS_MODE_MIRROR;
                case enums::SamplerAddressModeClampToEdge: return D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
                case enums::SamplerAddressModeClampToBorder: return D3D12_TEXTURE_ADDRESS_MODE_BORDER;
                default: return D3D12_TEXTURE_ADDRESS_MODE_WRAP;
            }
        }

        inline D3D12_RESOURCE_DESC d3d12_resource_desc(const texture_desc_t& desc)
        {
            D3D12_RESOURCE_DESC resourceDesc = {};
            resourceDesc.Width               = desc.width;
            resourceDesc.Height              = desc.height;
            resourceDesc.MipLevels           = desc.mip_levels;
            resourceDesc.Format              = dxgi_format(desc.format);
            resourceDesc.SampleDesc.Count    = 1;

            if (desc.alloc_type == enums::AllocationSparse)
            {
                resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_64KB_UNDEFINED_SWIZZLE;
            }

            if (desc.usage & enums::TextureUsageRenderTarget)
            {
                resourceDesc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
            }

            if (desc.usage & enums::TextureUsageDepthStencil)
            {
                resourceDesc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
            }

            if (desc.usage & enums::TextureUsageUnorderedAccess)
            {
                resourceDesc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
            }

            switch (desc.type)
            {
                case enums::TextureType2D:
                    resourceDesc.Dimension        = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
                    resourceDesc.DepthOrArraySize = 1;
                    break;
                case enums::TextureType2DArray:
                    resourceDesc.Dimension        = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
                    resourceDesc.DepthOrArraySize = desc.array_size;
                    break;
                case enums::TextureType3D:
                    resourceDesc.Dimension        = D3D12_RESOURCE_DIMENSION_TEXTURE3D;
                    resourceDesc.DepthOrArraySize = desc.depth;
                    break;
                case enums::TextureTypeCube:
                    ASSERT(desc.array_size == 6);
                    resourceDesc.Dimension        = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
                    resourceDesc.DepthOrArraySize = 6;
                    break;
                case enums::TextureTypeCubeArray:
                    ASSERT(desc.array_size % 6 == 0);
                    resourceDesc.Dimension        = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
                    resourceDesc.DepthOrArraySize = desc.array_size;
                    break;
                default: break;
            }

            return resourceDesc;
        }

        inline D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS d3d12_rt_as_flags(enums::rt::accstruct_flag flags)
        {
            D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS d3d12_flags = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_NONE;

            if (flags & enums::rt::AsFlagAllowUpdate)
            {
                d3d12_flags |= D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_ALLOW_UPDATE;
            }

            if (flags & enums::rt::AsFlagAllowCompaction)
            {
                d3d12_flags |= D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_ALLOW_COMPACTION;
            }

            if (flags & enums::rt::AsFlagPreferFastTrace)
            {
                d3d12_flags |= D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PREFER_FAST_TRACE;
            }

            if (flags & enums::rt::AsFlagPreferFastBuild)
            {
                d3d12_flags |= D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PREFER_FAST_BUILD;
            }

            if (flags & enums::rt::AsFlagLowMemory)
            {
                d3d12_flags |= D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_MINIMIZE_MEMORY;
            }

            return d3d12_flags;
        }

        inline D3D12_RAYTRACING_INSTANCE_FLAGS d3d12_rt_instance_flags(enums::rt::instance_flag flags)
        {
            D3D12_RAYTRACING_INSTANCE_FLAGS d3d12_flags = D3D12_RAYTRACING_INSTANCE_FLAG_NONE;

            if (flags & enums::rt::DisableCull)
            {
                d3d12_flags |= D3D12_RAYTRACING_INSTANCE_FLAG_TRIANGLE_CULL_DISABLE;
            }

            if (flags & enums::rt::FrontFaceCCW)
            {
                d3d12_flags |= D3D12_RAYTRACING_INSTANCE_FLAG_TRIANGLE_FRONT_COUNTERCLOCKWISE;
            }

            if (flags & enums::rt::ForceOpaque)
            {
                d3d12_flags |= D3D12_RAYTRACING_INSTANCE_FLAG_FORCE_OPAQUE;
            }

            if (flags & enums::rt::ForceNoOpaque)
            {
                d3d12_flags |= D3D12_RAYTRACING_INSTANCE_FLAG_FORCE_NON_OPAQUE;
            }

            return d3d12_flags;
        }
#endif
    }  // namespace ngfx
}  // namespace ncore

#endif
