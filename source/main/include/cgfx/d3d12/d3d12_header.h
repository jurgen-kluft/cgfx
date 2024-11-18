#ifndef __CGFX_D3D12_HEADER_H__
#define __CGFX_D3D12_HEADER_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
    #pragma once
#endif

#ifdef TARGET_PC
#include "cd3d12/d3d12.h"
#include "cd3d12/d3dx12.h"
#include "dxgi1_6.h"
#endif

#include "cgfx/gfx_defines.h"
#include "cbase/c_debug.h"

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

        inline D3D12_HEAP_TYPE d3d12_heap_type(GfxMemoryType memory_type)
        {
            switch (memory_type)
            {
                case GfxMemory::GpuOnly: return D3D12_HEAP_TYPE_DEFAULT;
                case GfxMemory::CpuOnly:
                case GfxMemory::CpuToGpu: return D3D12_HEAP_TYPE_UPLOAD;
                case GfxMemory::GpuToCpu: return D3D12_HEAP_TYPE_READBACK;
                default: return D3D12_HEAP_TYPE_DEFAULT;
            }
        }

        inline D3D12_BARRIER_SYNC d3d12_barrier_sync(GfxAccessFlags flags)
        {
            D3D12_BARRIER_SYNC sync    = D3D12_BARRIER_SYNC_NONE;
            bool               discard = flags & GfxAccess::Discard;
            if (!discard)
            {
                // d3d validation : "SyncAfter bits D3D12_BARRIER_SYNC_CLEAR_UNORDERED_ACCESS_VIEW are incompatible with AccessAfter bits D3D12_BARRIER_ACCESS_NO_ACCESS"
                if (flags & GfxAccess::ClearUAV)
                    sync |= D3D12_BARRIER_SYNC_CLEAR_UNORDERED_ACCESS_VIEW;
            }

            if (flags & GfxAccess::Present)
                sync |= D3D12_BARRIER_SYNC_ALL;
            if (flags & GfxAccess::RTV)
                sync |= D3D12_BARRIER_SYNC_RENDER_TARGET;
            if (flags & GfxAccess::MaskDSV)
                sync |= D3D12_BARRIER_SYNC_DEPTH_STENCIL;
            if (flags & GfxAccess::MaskVS)
                sync |= D3D12_BARRIER_SYNC_VERTEX_SHADING;
            if (flags & GfxAccess::MaskPS)
                sync |= D3D12_BARRIER_SYNC_PIXEL_SHADING;
            if (flags & GfxAccess::MaskCS)
                sync |= D3D12_BARRIER_SYNC_COMPUTE_SHADING;
            if (flags & GfxAccess::MaskCopy)
                sync |= D3D12_BARRIER_SYNC_COPY;
            if (flags & GfxAccess::ShadingRate)
                sync |= D3D12_BARRIER_SYNC_PIXEL_SHADING;
            if (flags & GfxAccess::IndexBuffer)
                sync |= D3D12_BARRIER_SYNC_INDEX_INPUT;
            if (flags & GfxAccess::IndirectArgs)
                sync |= D3D12_BARRIER_SYNC_EXECUTE_INDIRECT;
            if (flags & GfxAccess::MaskAS)
                sync |= D3D12_BARRIER_SYNC_BUILD_RAYTRACING_ACCELERATION_STRUCTURE;

            return sync;
        }

        inline D3D12_BARRIER_ACCESS d3d12_barrier_access(GfxAccessFlags flags)
        {
            if (flags & GfxAccess::Discard)
            {
                return D3D12_BARRIER_ACCESS_NO_ACCESS;
            }

            D3D12_BARRIER_ACCESS access = D3D12_BARRIER_ACCESS_COMMON;

            if (flags & GfxAccess::RTV)
                access |= D3D12_BARRIER_ACCESS_RENDER_TARGET;
            if (flags & GfxAccess::DSV)
                access |= D3D12_BARRIER_ACCESS_DEPTH_STENCIL_WRITE;
            if (flags & GfxAccess::DSVReadOnly)
                access |= D3D12_BARRIER_ACCESS_DEPTH_STENCIL_READ;
            if (flags & GfxAccess::MaskSRV)
                access |= D3D12_BARRIER_ACCESS_SHADER_RESOURCE;
            if (flags & GfxAccess::MaskUAV)
                access |= D3D12_BARRIER_ACCESS_UNORDERED_ACCESS;
            if (flags & GfxAccess::ClearUAV)
                access |= D3D12_BARRIER_ACCESS_UNORDERED_ACCESS;
            if (flags & GfxAccess::CopyDst)
                access |= D3D12_BARRIER_ACCESS_COPY_DEST;
            if (flags & GfxAccess::CopySrc)
                access |= D3D12_BARRIER_ACCESS_COPY_SOURCE;
            if (flags & GfxAccess::ShadingRate)
                access |= D3D12_BARRIER_ACCESS_SHADING_RATE_SOURCE;
            if (flags & GfxAccess::IndexBuffer)
                access |= D3D12_BARRIER_ACCESS_INDEX_BUFFER;
            if (flags & GfxAccess::IndirectArgs)
                access |= D3D12_BARRIER_ACCESS_INDIRECT_ARGUMENT;
            if (flags & GfxAccess::ASRead)
                access |= D3D12_BARRIER_ACCESS_RAYTRACING_ACCELERATION_STRUCTURE_READ;
            if (flags & GfxAccess::ASWrite)
                access |= D3D12_BARRIER_ACCESS_RAYTRACING_ACCELERATION_STRUCTURE_WRITE;

            return access;
        }

        inline D3D12_BARRIER_LAYOUT d3d12_barrier_layout(GfxAccessFlags flags)
        {
            if (flags & GfxAccess::Discard)
                return D3D12_BARRIER_LAYOUT_UNDEFINED;
            if (flags & GfxAccess::Present)
                return D3D12_BARRIER_LAYOUT_PRESENT;
            if (flags & GfxAccess::RTV)
                return D3D12_BARRIER_LAYOUT_RENDER_TARGET;
            if (flags & GfxAccess::DSV)
                return D3D12_BARRIER_LAYOUT_DEPTH_STENCIL_WRITE;
            if (flags & GfxAccess::DSVReadOnly)
                return D3D12_BARRIER_LAYOUT_DEPTH_STENCIL_READ;
            if (flags & GfxAccess::MaskSRV)
                return D3D12_BARRIER_LAYOUT_SHADER_RESOURCE;
            if (flags & GfxAccess::MaskUAV)
                return D3D12_BARRIER_LAYOUT_UNORDERED_ACCESS;
            if (flags & GfxAccess::ClearUAV)
                return D3D12_BARRIER_LAYOUT_UNORDERED_ACCESS;
            if (flags & GfxAccess::CopyDst)
                return D3D12_BARRIER_LAYOUT_COPY_DEST;
            if (flags & GfxAccess::CopySrc)
                return D3D12_BARRIER_LAYOUT_COPY_SOURCE;
            if (flags & GfxAccess::ShadingRate)
                return D3D12_BARRIER_LAYOUT_SHADING_RATE_SOURCE;

            ASSERT(false);
            return D3D12_BARRIER_LAYOUT_UNDEFINED;
        }

        inline DXGI_FORMAT dxgi_format(GfxFormat format, bool depth_srv = false, bool uav = false)
        {
            switch (format)
            {
                case Gfx::Unknown: return DXGI_FORMAT_UNKNOWN;
                case Gfx::RGBA32F: return DXGI_FORMAT_R32G32B32A32_FLOAT;
                case Gfx::RGBA32UI: return DXGI_FORMAT_R32G32B32A32_UINT;
                case Gfx::RGBA32SI: return DXGI_FORMAT_R32G32B32A32_SINT;
                case Gfx::RGBA16F: return DXGI_FORMAT_R16G16B16A16_FLOAT;
                case Gfx::RGBA16UI: return DXGI_FORMAT_R16G16B16A16_UINT;
                case Gfx::RGBA16SI: return DXGI_FORMAT_R16G16B16A16_SINT;
                case Gfx::RGBA16UNORM: return DXGI_FORMAT_R16G16B16A16_UNORM;
                case Gfx::RGBA16SNORM: return DXGI_FORMAT_R16G16B16A16_SNORM;
                case Gfx::RGBA8UI: return DXGI_FORMAT_R8G8B8A8_UINT;
                case Gfx::RGBA8SI: return DXGI_FORMAT_R8G8B8A8_SINT;
                case Gfx::RGBA8UNORM: return DXGI_FORMAT_R8G8B8A8_UNORM;
                case Gfx::RGBA8SNORM: return DXGI_FORMAT_R8G8B8A8_SNORM;
                case Gfx::RGBA8SRGB: return uav ? DXGI_FORMAT_R8G8B8A8_UNORM : DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
                case Gfx::BGRA8UNORM: return DXGI_FORMAT_B8G8R8A8_UNORM;
                case Gfx::BGRA8SRGB: return uav ? DXGI_FORMAT_B8G8R8A8_UNORM : DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
                case Gfx::RGB10A2UI: return DXGI_FORMAT_R10G10B10A2_UINT;
                case Gfx::RGB10A2UNORM: return DXGI_FORMAT_R10G10B10A2_UNORM;
                case Gfx::RGB32F: return DXGI_FORMAT_R32G32B32_FLOAT;
                case Gfx::RGB32UI: return DXGI_FORMAT_R32G32B32_UINT;
                case Gfx::RGB32SI: return DXGI_FORMAT_R32G32B32_SINT;
                case Gfx::R11G11B10F: return DXGI_FORMAT_R11G11B10_FLOAT;
                case Gfx::RGB9E5: return DXGI_FORMAT_R9G9B9E5_SHAREDEXP;
                case Gfx::RG32F: return DXGI_FORMAT_R32G32_FLOAT;
                case Gfx::RG32UI: return DXGI_FORMAT_R32G32_UINT;
                case Gfx::RG32SI: return DXGI_FORMAT_R32G32_SINT;
                case Gfx::RG16F: return DXGI_FORMAT_R16G16_FLOAT;
                case Gfx::RG16UI: return DXGI_FORMAT_R16G16_UINT;
                case Gfx::RG16SI: return DXGI_FORMAT_R16G16_SINT;
                case Gfx::RG16UNORM: return DXGI_FORMAT_R16G16_UNORM;
                case Gfx::RG16SNORM: return DXGI_FORMAT_R16G16_SNORM;
                case Gfx::RG8UI: return DXGI_FORMAT_R8G8_UINT;
                case Gfx::RG8SI: return DXGI_FORMAT_R8G8_SINT;
                case Gfx::RG8UNORM: return DXGI_FORMAT_R8G8_UNORM;
                case Gfx::RG8SNORM: return DXGI_FORMAT_R8G8_SNORM;
                case Gfx::R32F: return DXGI_FORMAT_R32_FLOAT;
                case Gfx::R32UI: return DXGI_FORMAT_R32_UINT;
                case Gfx::R32SI: return DXGI_FORMAT_R32_SINT;
                case Gfx::R16F: return DXGI_FORMAT_R16_FLOAT;
                case Gfx::R16UI: return DXGI_FORMAT_R16_UINT;
                case Gfx::R16SI: return DXGI_FORMAT_R16_SINT;
                case Gfx::R16UNORM: return DXGI_FORMAT_R16_UNORM;
                case Gfx::R16SNORM: return DXGI_FORMAT_R16_SNORM;
                case Gfx::R8UI: return DXGI_FORMAT_R8_UINT;
                case Gfx::R8SI: return DXGI_FORMAT_R8_SINT;
                case Gfx::R8UNORM: return DXGI_FORMAT_R8_UNORM;
                case Gfx::R8SNORM: return DXGI_FORMAT_R8_SNORM;
                case Gfx::D32F: return depth_srv ? DXGI_FORMAT_R32_FLOAT : DXGI_FORMAT_D32_FLOAT;
                case Gfx::D32FS8: return depth_srv ? DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS : DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
                case Gfx::D16: return depth_srv ? DXGI_FORMAT_R16_UNORM : DXGI_FORMAT_D16_UNORM;
                case Gfx::BC1UNORM: return DXGI_FORMAT_BC1_UNORM;
                case Gfx::BC1SRGB: return DXGI_FORMAT_BC1_UNORM_SRGB;
                case Gfx::BC2UNORM: return DXGI_FORMAT_BC2_UNORM;
                case Gfx::BC2SRGB: return DXGI_FORMAT_BC2_UNORM_SRGB;
                case Gfx::BC3UNORM: return DXGI_FORMAT_BC3_UNORM;
                case Gfx::BC3SRGB: return DXGI_FORMAT_BC3_UNORM_SRGB;
                case Gfx::BC4UNORM: return DXGI_FORMAT_BC4_UNORM;
                case Gfx::BC4SNORM: return DXGI_FORMAT_BC4_SNORM;
                case Gfx::BC5UNORM: return DXGI_FORMAT_BC5_UNORM;
                case Gfx::BC5SNORM: return DXGI_FORMAT_BC5_SNORM;
                case Gfx::BC6U16F: return DXGI_FORMAT_BC6H_UF16;
                case Gfx::BC6S16F: return DXGI_FORMAT_BC6H_SF16;
                case Gfx::BC7UNORM: return DXGI_FORMAT_BC7_UNORM;
                case Gfx::BC7SRGB: return DXGI_FORMAT_BC7_UNORM_SRGB;
                default: ASSERT(false); return DXGI_FORMAT_UNKNOWN;
            }
        }

        inline D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE d3d12_render_pass_loadop(GfxRenderPassLoadOp loadOp)
        {
            switch (loadOp)
            {
                case GfxRenderPass::LoadLoad: return D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_PRESERVE;
                case GfxRenderPass::LoadClear: return D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_CLEAR;
                case GfxRenderPass::LoadDontCare: return D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_DISCARD;
                default: return D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_PRESERVE;
            }
        }

        inline D3D12_RENDER_PASS_ENDING_ACCESS_TYPE d3d12_render_pass_storeop(GfxRenderPassStoreOp storeOp)
        {
            switch (storeOp)
            {
                case GfxRenderPass::StoreStore: return D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_PRESERVE;
                case GfxRenderPass::StoreDontCare: return D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_DISCARD;
                default: return D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_PRESERVE;
            }
        }

        inline D3D12_BLEND d3d12_blend(GfxBlendFactor blend_factor)
        {
            switch (blend_factor)
            {
                case GfxBlendFactor::Zero: return D3D12_BLEND_ZERO;
                case GfxBlendFactor::One: return D3D12_BLEND_ONE;
                case GfxBlendFactor::SrcColor: return D3D12_BLEND_SRC_COLOR;
                case GfxBlendFactor::InvSrcColor: return D3D12_BLEND_INV_SRC_COLOR;
                case GfxBlendFactor::SrcAlpha: return D3D12_BLEND_SRC_ALPHA;
                case GfxBlendFactor::InvSrcAlpha: return D3D12_BLEND_INV_SRC_ALPHA;
                case GfxBlendFactor::DstAlpha: return D3D12_BLEND_DEST_ALPHA;
                case GfxBlendFactor::InvDstAlpha: return D3D12_BLEND_INV_DEST_ALPHA;
                case GfxBlendFactor::DstColor: return D3D12_BLEND_DEST_COLOR;
                case GfxBlendFactor::InvDstColor: return D3D12_BLEND_INV_DEST_COLOR;
                case GfxBlendFactor::SrcAlphaClamp: return D3D12_BLEND_SRC_ALPHA_SAT;
                case GfxBlendFactor::ConstantFactor: return D3D12_BLEND_BLEND_FACTOR;
                case GfxBlendFactor::InvConstantFactor: return D3D12_BLEND_INV_BLEND_FACTOR;
                default: return D3D12_BLEND_ONE;
            }
        }

        inline D3D12_BLEND_OP d3d12_blend_op(GfxBlendOp blend_op)
        {
            switch (blend_op)
            {
                case GfxBlendOp::Add: return D3D12_BLEND_OP_ADD;
                case GfxBlendOp::Subtract: return D3D12_BLEND_OP_SUBTRACT;
                case GfxBlendOp::ReverseSubtract: return D3D12_BLEND_OP_REV_SUBTRACT;
                case GfxBlendOp::Min: return D3D12_BLEND_OP_MIN;
                case GfxBlendOp::Max: return D3D12_BLEND_OP_MAX;
                default: return D3D12_BLEND_OP_ADD;
            }
        }

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

        inline D3D12_CULL_MODE d3d12_cull_mode(GfxCullMode cull_mode)
        {
            switch (cull_mode)
            {
                case GfxCullMode::None: return D3D12_CULL_MODE_NONE;
                case GfxCullMode::Front: return D3D12_CULL_MODE_FRONT;
                case GfxCullMode::Back: return D3D12_CULL_MODE_BACK;
                default: return D3D12_CULL_MODE_NONE;
            }
        }

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

        inline D3D12_COMPARISON_FUNC d3d12_compare_func(GfxCompareFunc func)
        {
            switch (func)
            {
                case GfxCompareFunc::Never: return D3D12_COMPARISON_FUNC_NEVER;
                case GfxCompareFunc::Less: return D3D12_COMPARISON_FUNC_LESS;
                case GfxCompareFunc::Equal: return D3D12_COMPARISON_FUNC_EQUAL;
                case GfxCompareFunc::LessEqual: return D3D12_COMPARISON_FUNC_LESS_EQUAL;
                case GfxCompareFunc::Greater: return D3D12_COMPARISON_FUNC_GREATER;
                case GfxCompareFunc::NotEqual: return D3D12_COMPARISON_FUNC_NOT_EQUAL;
                case GfxCompareFunc::GreaterEqual: return D3D12_COMPARISON_FUNC_GREATER_EQUAL;
                case GfxCompareFunc::Always: return D3D12_COMPARISON_FUNC_ALWAYS;
                default: return D3D12_COMPARISON_FUNC_NONE;
            }
        }

        inline D3D12_STENCIL_OP d3d12_stencil_op(GfxStencilOp stencil_op)
        {
            switch (stencil_op)
            {
                case GfxStencilOp::Keep: return D3D12_STENCIL_OP_KEEP;
                case GfxStencilOp::Zero: return D3D12_STENCIL_OP_ZERO;
                case GfxStencilOp::Replace: return D3D12_STENCIL_OP_REPLACE;
                case GfxStencilOp::IncreaseClamp: return D3D12_STENCIL_OP_INCR_SAT;
                case GfxStencilOp::DecreaseClamp: return D3D12_STENCIL_OP_DECR_SAT;
                case GfxStencilOp::Invert: return D3D12_STENCIL_OP_INVERT;
                case GfxStencilOp::IncreaseWrap: return D3D12_STENCIL_OP_INCR;
                case GfxStencilOp::DecreaseWrap: return D3D12_STENCIL_OP_DECR;
                default: return D3D12_STENCIL_OP_KEEP;
            }
        }

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

        inline D3D12_PRIMITIVE_TOPOLOGY_TYPE d3d12_topology_type(GfxPrimitiveType primitive_type)
        {
            switch (primitive_type)
            {
                case GfxPrimitiveType::PointList: return D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
                case GfxPrimitiveType::LineList:
                case GfxPrimitiveType::LineStrip: return D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
                case GfxPrimitiveType::TriangleList:
                case GfxPrimitiveType::TriangleTrip: return D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
                default: return D3D12_PRIMITIVE_TOPOLOGY_TYPE_UNDEFINED;
            }
        }

        inline D3D_PRIMITIVE_TOPOLOGY d3d12_primitive_topology(GfxPrimitiveType primitive_type)
        {
            switch (primitive_type)
            {
                case GfxPrimitiveType::PointList: return D3D_PRIMITIVE_TOPOLOGY_POINTLIST;
                case GfxPrimitiveType::LineList: return D3D_PRIMITIVE_TOPOLOGY_LINELIST;
                case GfxPrimitiveType::LineStrip: return D3D_PRIMITIVE_TOPOLOGY_LINESTRIP;
                case GfxPrimitiveType::TriangleList: return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
                case GfxPrimitiveType::TriangleTrip: return D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
                default: return D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
            }
        }

        inline D3D12_FILTER_REDUCTION_TYPE d3d12_filter_reduction_type(GfxSamplerReductionMode mode)
        {
            switch (mode)
            {
                case GfxSamplerReductionMode::Standard: return D3D12_FILTER_REDUCTION_TYPE_STANDARD;
                case GfxSamplerReductionMode::Compare: return D3D12_FILTER_REDUCTION_TYPE_COMPARISON;
                case GfxSamplerReductionMode::Min: return D3D12_FILTER_REDUCTION_TYPE_MINIMUM;
                case GfxSamplerReductionMode::Max: return D3D12_FILTER_REDUCTION_TYPE_MAXIMUM;
                default: return D3D12_FILTER_REDUCTION_TYPE_STANDARD;
            }
        }

        inline D3D12_FILTER_TYPE d3d12_filter_type(GfxFilter filter)
        {
            switch (filter)
            {
                case GfxFilter::Point: return D3D12_FILTER_TYPE_POINT;
                case GfxFilter::Linear: return D3D12_FILTER_TYPE_LINEAR;
                default: return D3D12_FILTER_TYPE_POINT;
            }
        }

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

        inline D3D12_TEXTURE_ADDRESS_MODE d3d12_address_mode(GfxSamplerAddressMode mode)
        {
            switch (mode)
            {
                case GfxSamplerAddressMode::Repeat: return D3D12_TEXTURE_ADDRESS_MODE_WRAP;
                case GfxSamplerAddressMode::MirroredRepeat: return D3D12_TEXTURE_ADDRESS_MODE_MIRROR;
                case GfxSamplerAddressMode::ClampToEdge: return D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
                case GfxSamplerAddressMode::ClampToBorder: return D3D12_TEXTURE_ADDRESS_MODE_BORDER;
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

            if (desc.alloc_type == GfxAllocation::Sparse)
            {
                resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_64KB_UNDEFINED_SWIZZLE;
            }

            if (desc.usage & GfxTextureUsage::RenderTarget)
            {
                resourceDesc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
            }

            if (desc.usage & GfxTextureUsage::DepthStencil)
            {
                resourceDesc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
            }

            if (desc.usage & GfxTextureUsage::UnorderedAccess)
            {
                resourceDesc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
            }

            switch (desc.type)
            {
                case GfxTextureType::Texture2D:
                    resourceDesc.Dimension        = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
                    resourceDesc.DepthOrArraySize = 1;
                    break;
                case GfxTextureType::Texture2DArray:
                    resourceDesc.Dimension        = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
                    resourceDesc.DepthOrArraySize = desc.array_size;
                    break;
                case GfxTextureType::Texture3D:
                    resourceDesc.Dimension        = D3D12_RESOURCE_DIMENSION_TEXTURE3D;
                    resourceDesc.DepthOrArraySize = desc.depth;
                    break;
                case GfxTextureType::TextureCube:
                    ASSERT(desc.array_size == 6);
                    resourceDesc.Dimension        = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
                    resourceDesc.DepthOrArraySize = 6;
                    break;
                case GfxTextureType::TextureCubeArray:
                    ASSERT(desc.array_size % 6 == 0);
                    resourceDesc.Dimension        = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
                    resourceDesc.DepthOrArraySize = desc.array_size;
                    break;
                default: break;
            }

            return resourceDesc;
        }

        inline D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS d3d12_rt_as_flags(GfxRayTracing::AccStructureFlag flags)
        {
            D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS d3d12_flags = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_NONE;

            if (flags & GfxRayTracing::AccStructure::FlagAllowUpdate)
            {
                d3d12_flags |= D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_ALLOW_UPDATE;
            }

            if (flags & GfxRayTracing::AccStructure::FlagAllowCompaction)
            {
                d3d12_flags |= D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_ALLOW_COMPACTION;
            }

            if (flags & GfxRayTracing::AccStructure::FlagPreferFastTrace)
            {
                d3d12_flags |= D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PREFER_FAST_TRACE;
            }

            if (flags & GfxRayTracing::AccStructure::FlagPreferFastBuild)
            {
                d3d12_flags |= D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PREFER_FAST_BUILD;
            }

            if (flags & GfxRayTracing::AccStructure::FlagLowMemory)
            {
                d3d12_flags |= D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_MINIMIZE_MEMORY;
            }

            return d3d12_flags;
        }

        inline D3D12_RAYTRACING_INSTANCE_FLAGS d3d12_rt_instance_flags(GfxRayTracing::InstanceFlag flags)
        {
            D3D12_RAYTRACING_INSTANCE_FLAGS d3d12_flags = D3D12_RAYTRACING_INSTANCE_FLAG_NONE;

            if (flags & GfxRayTracing::Instance::DisableCull)
            {
                d3d12_flags |= D3D12_RAYTRACING_INSTANCE_FLAG_TRIANGLE_CULL_DISABLE;
            }

            if (flags & GfxRayTracing::Instance::FrontFaceCCW)
            {
                d3d12_flags |= D3D12_RAYTRACING_INSTANCE_FLAG_TRIANGLE_FRONT_COUNTERCLOCKWISE;
            }

            if (flags & GfxRayTracing::Instance::ForceOpaque)
            {
                d3d12_flags |= D3D12_RAYTRACING_INSTANCE_FLAG_FORCE_OPAQUE;
            }

            if (flags & GfxRayTracing::Instance::ForceNoOpaque)
            {
                d3d12_flags |= D3D12_RAYTRACING_INSTANCE_FLAG_FORCE_NON_OPAQUE;
            }

            return d3d12_flags;
        }
#endif
    }  // namespace ngfx
}  // namespace ncore
#endif
