#ifndef __CGFX_METAL_UTILS_H__
#define __CGFX_METAL_UTILS_H__

#include "cbase/c_debug.h"
#include "cgfx/gfx_defines.h"

#include "cmacos/Foundation/Foundation.hpp"
#include "cmacos/Metal/Metal.hpp"
#include "cmacos/MetalKit/MetalKit.hpp"

#define IR_RUNTIME_METALCPP
// #include "metal_irconverter_runtime/metal_irconverter_runtime.h"

namespace ncore
{
    namespace ngfx
    {
        template <typename T>
        inline void SetDebugLabel(T* resource, const char* name)
        {
            NS::String* label = NS::String::alloc()->init(name, NS::StringEncoding::UTF8StringEncoding);
            resource->setLabel(label);
            label->release();
        }

        inline MTL::ResourceOptions ToResourceOptions(GfxMemoryType type)
        {
            MTL::ResourceOptions options = MTL::ResourceHazardTrackingModeTracked;

            switch (type)
            {
                case GfxMemory::GpuOnly: options |= MTL::ResourceStorageModePrivate; break;
                case GfxMemory::CpuOnly:
                case GfxMemory::CpuToGpu: options |= MTL::ResourceStorageModeShared | MTL::ResourceCPUCacheModeWriteCombined; break;
                case GfxMemory::GpuToCpu: options |= MTL::ResourceStorageModeShared; break;
                default: break;
            }

            return options;
        }

        inline MTL::TextureType ToTextureType(GfxTextureType type)
        {
            switch (type)
            {
                case GfxTextureType::Texture2D: return MTL::TextureType2D;
                case GfxTextureType::Texture2DArray: return MTL::TextureType2DArray;
                case GfxTextureType::Texture3D: return MTL::TextureType3D;
                case GfxTextureType::TextureCube: return MTL::TextureTypeCube;
                case GfxTextureType::TextureCubeArray: return MTL::TextureTypeCubeArray;
                default: return MTL::TextureType2D;
            }
        }

        inline MTL::PixelFormat ToPixelFormat(GfxFormat format)
        {
            switch (format)
            {
                case Gfx::Unknown: return MTL::PixelFormatInvalid;
                case Gfx::RGBA32F: return MTL::PixelFormatRGBA32Float;
                case Gfx::RGBA32UI: return MTL::PixelFormatRGBA32Uint;
                case Gfx::RGBA32SI: return MTL::PixelFormatRGBA32Sint;
                case Gfx::RGBA16F: return MTL::PixelFormatRGBA16Float;
                case Gfx::RGBA16UI: return MTL::PixelFormatRGBA16Uint;
                case Gfx::RGBA16SI: return MTL::PixelFormatRGBA16Sint;
                case Gfx::RGBA16UNORM: return MTL::PixelFormatRGBA16Unorm;
                case Gfx::RGBA16SNORM: return MTL::PixelFormatRGBA16Snorm;
                case Gfx::RGBA8UI: return MTL::PixelFormatRGBA8Uint;
                case Gfx::RGBA8SI: return MTL::PixelFormatRGBA8Sint;
                case Gfx::RGBA8UNORM: return MTL::PixelFormatRGBA8Unorm;
                case Gfx::RGBA8SNORM: return MTL::PixelFormatRGBA8Snorm;
                case Gfx::RGBA8SRGB: return MTL::PixelFormatRGBA8Unorm_sRGB;
                case Gfx::BGRA8UNORM: return MTL::PixelFormatBGRA8Unorm;
                case Gfx::BGRA8SRGB: return MTL::PixelFormatBGRA8Unorm_sRGB;
                case Gfx::RGB10A2UI: return MTL::PixelFormatRGB10A2Uint;
                case Gfx::RGB10A2UNORM: return MTL::PixelFormatRGB10A2Unorm;
                case Gfx::RGB32F: return MTL::PixelFormatInvalid;
                case Gfx::RGB32UI: return MTL::PixelFormatInvalid;
                case Gfx::RGB32SI: return MTL::PixelFormatInvalid;
                case Gfx::R11G11B10F: return MTL::PixelFormatRG11B10Float;
                case Gfx::RGB9E5: return MTL::PixelFormatRGB9E5Float;
                case Gfx::RG32F: return MTL::PixelFormatRG32Float;
                case Gfx::RG32UI: return MTL::PixelFormatRG32Uint;
                case Gfx::RG32SI: return MTL::PixelFormatRG32Sint;
                case Gfx::RG16F: return MTL::PixelFormatRG16Float;
                case Gfx::RG16UI: return MTL::PixelFormatRG16Uint;
                case Gfx::RG16SI: return MTL::PixelFormatRG16Sint;
                case Gfx::RG16UNORM: return MTL::PixelFormatRG16Unorm;
                case Gfx::RG16SNORM: return MTL::PixelFormatRG16Snorm;
                case Gfx::RG8UI: return MTL::PixelFormatRG8Uint;
                case Gfx::RG8SI: return MTL::PixelFormatRG8Sint;
                case Gfx::RG8UNORM: return MTL::PixelFormatRG8Unorm;
                case Gfx::RG8SNORM: return MTL::PixelFormatRG8Snorm;
                case Gfx::R32F: return MTL::PixelFormatR32Float;
                case Gfx::R32UI: return MTL::PixelFormatR32Uint;
                case Gfx::R32SI: return MTL::PixelFormatR32Sint;
                case Gfx::R16F: return MTL::PixelFormatR16Float;
                case Gfx::R16UI: return MTL::PixelFormatR16Uint;
                case Gfx::R16SI: return MTL::PixelFormatR16Sint;
                case Gfx::R16UNORM: return MTL::PixelFormatR16Unorm;
                case Gfx::R16SNORM: return MTL::PixelFormatR16Snorm;
                case Gfx::R8UI: return MTL::PixelFormatR8Uint;
                case Gfx::R8SI: return MTL::PixelFormatR8Sint;
                case Gfx::R8UNORM: return MTL::PixelFormatR8Unorm;
                case Gfx::R8SNORM: return MTL::PixelFormatR8Snorm;
                case Gfx::D32F: return MTL::PixelFormatDepth32Float;
                case Gfx::D32FS8: return MTL::PixelFormatDepth32Float_Stencil8;
                case Gfx::D16: return MTL::PixelFormatDepth16Unorm;
                case Gfx::BC1UNORM: return MTL::PixelFormatBC1_RGBA;
                case Gfx::BC1SRGB: return MTL::PixelFormatBC1_RGBA_sRGB;
                case Gfx::BC2UNORM: return MTL::PixelFormatBC2_RGBA;
                case Gfx::BC2SRGB: return MTL::PixelFormatBC2_RGBA_sRGB;
                case Gfx::BC3UNORM: return MTL::PixelFormatBC3_RGBA;
                case Gfx::BC3SRGB: return MTL::PixelFormatBC3_RGBA_sRGB;
                case Gfx::BC4UNORM: return MTL::PixelFormatBC4_RUnorm;
                case Gfx::BC4SNORM: return MTL::PixelFormatBC4_RSnorm;
                case Gfx::BC5UNORM: return MTL::PixelFormatBC5_RGUnorm;
                case Gfx::BC5SNORM: return MTL::PixelFormatBC5_RGSnorm;
                case Gfx::BC6U16F: return MTL::PixelFormatBC6H_RGBUfloat;
                case Gfx::BC6S16F: return MTL::PixelFormatBC6H_RGBFloat;
                case Gfx::BC7UNORM: return MTL::PixelFormatBC7_RGBAUnorm;
                case Gfx::BC7SRGB: return MTL::PixelFormatBC7_RGBAUnorm_sRGB;
                default: return MTL::PixelFormatInvalid;
            }
        }

        inline MTL::TextureUsage ToTextureUsage(GfxTextureUsageFlags flags)
        {
            MTL::TextureUsage usage = MTL::TextureUsageShaderRead | MTL::TextureUsagePixelFormatView;

            if (flags & (GfxTextureUsage::RenderTarget | GfxTextureUsage::DepthStencil))
            {
                usage |= MTL::TextureUsageRenderTarget;
            }

            if (flags & GfxTextureUsage::UnorderedAccess)
            {
                usage |= MTL::TextureUsageShaderWrite;  // todo TextureUsageShaderAtomic
            }

            return usage;
        }

        inline MTL::TextureDescriptor* ToTextureDescriptor(const texture_desc_t& desc)
        {
            MTL::TextureDescriptor* descriptor = MTL::TextureDescriptor::alloc()->init();
            descriptor->setWidth(desc.width);
            descriptor->setHeight(desc.height);
            descriptor->setDepth(desc.depth);
            descriptor->setMipmapLevelCount(desc.mip_levels);
            if (desc.type == GfxTextureType::TextureCube || desc.type == GfxTextureType::TextureCubeArray)
            {
                ASSERT(desc.array_size % 6 == 0);
                descriptor->setArrayLength(desc.array_size / 6);
            }
            else
            {
                descriptor->setArrayLength(desc.array_size);
            }
            descriptor->setTextureType(ToTextureType(desc.type));
            descriptor->setPixelFormat(ToPixelFormat(desc.format));
            descriptor->setResourceOptions(ToResourceOptions(desc.memory_type));
            descriptor->setUsage(ToTextureUsage(desc.usage));

            return descriptor;
        }

        inline MTL::LoadAction ToLoadAction(GfxRenderPassLoadOp loadOp)
        {
            switch (loadOp)
            {
                case GfxRenderPass::LoadLoad: return MTL::LoadActionLoad;
                case GfxRenderPass::LoadClear: return MTL::LoadActionClear;
                case GfxRenderPass::LoadDontCare: return MTL::LoadActionDontCare;
                default: return MTL::LoadActionLoad;
            }
        }

        inline MTL::StoreAction ToStoreAction(GfxRenderPassStoreOp storeOp)
        {
            switch (storeOp)
            {
                case GfxRenderPass::StoreStore: return MTL::StoreActionStore;
                case GfxRenderPass::StoreDontCare: return MTL::StoreActionDontCare;
                default: return MTL::StoreActionStore;
            }
        }

        inline MTL::PrimitiveTopologyClass ToTopologyClass(GfxPrimitiveType type)
        {
            switch (type)
            {
                case GfxPrimitiveType::PointList: return MTL::PrimitiveTopologyClassPoint;
                case GfxPrimitiveType::LineList:
                case GfxPrimitiveType::LineStrip: return MTL::PrimitiveTopologyClassLine;
                case GfxPrimitiveType::TriangleList:
                case GfxPrimitiveType::TriangleTrip: return MTL::PrimitiveTopologyClassTriangle;
                default: return MTL::PrimitiveTopologyClassUnspecified;
            }
        }

        inline MTL::PrimitiveType ToPrimitiveType(GfxPrimitiveType type)
        {
            switch (type)
            {
                case GfxPrimitiveType::PointList: return MTL::PrimitiveTypePoint;
                case GfxPrimitiveType::LineList: return MTL::PrimitiveTypeLine;
                case GfxPrimitiveType::LineStrip: return MTL::PrimitiveTypeLineStrip;
                case GfxPrimitiveType::TriangleList: return MTL::PrimitiveTypeTriangle;
                case GfxPrimitiveType::TriangleTrip: return MTL::PrimitiveTypeTriangleStrip;
                default: return MTL::PrimitiveTypeTriangle;
            }
        }

        inline MTL::BlendFactor ToBlendFactor(GfxBlendFactor blend_factor)
        {
            switch (blend_factor)
            {
                case GfxBlendFactor::Zero: return MTL::BlendFactorZero;
                case GfxBlendFactor::One: return MTL::BlendFactorOne;
                case GfxBlendFactor::SrcColor: return MTL::BlendFactorSourceColor;
                case GfxBlendFactor::InvSrcColor: return MTL::BlendFactorOneMinusSourceColor;
                case GfxBlendFactor::SrcAlpha: return MTL::BlendFactorSourceAlpha;
                case GfxBlendFactor::InvSrcAlpha: return MTL::BlendFactorOneMinusSourceAlpha;
                case GfxBlendFactor::DstAlpha: return MTL::BlendFactorDestinationAlpha;
                case GfxBlendFactor::InvDstAlpha: return MTL::BlendFactorOneMinusDestinationAlpha;
                case GfxBlendFactor::DstColor: return MTL::BlendFactorDestinationColor;
                case GfxBlendFactor::InvDstColor: return MTL::BlendFactorOneMinusDestinationColor;
                case GfxBlendFactor::SrcAlphaClamp: return MTL::BlendFactorSourceAlphaSaturated;
                case GfxBlendFactor::ConstantFactor: return MTL::BlendFactorBlendColor;
                case GfxBlendFactor::InvConstantFactor: return MTL::BlendFactorOneMinusBlendColor;
                default: return MTL::BlendFactorZero;
            }
        }

        inline MTL::BlendOperation ToBlendOperation(GfxBlendOp blend_op)
        {
            switch (blend_op)
            {
                case GfxBlendOp::Add: return MTL::BlendOperationAdd;
                case GfxBlendOp::Subtract: return MTL::BlendOperationSubtract;
                case GfxBlendOp::ReverseSubtract: return MTL::BlendOperationReverseSubtract;
                case GfxBlendOp::Min: return MTL::BlendOperationMin;
                case GfxBlendOp::Max: return MTL::BlendOperationMax;
                default: return MTL::BlendOperationAdd;
            }
        }

        inline MTL::ColorWriteMask ToColorWriteMask(GfxColorWriteMask mask)
        {
            MTL::ColorWriteMask mtlMask = MTL::ColorWriteMaskNone;

            if (mask & GfxColorWriteMaskR)
                mtlMask |= MTL::ColorWriteMaskRed;
            if (mask & GfxColorWriteMaskG)
                mtlMask |= MTL::ColorWriteMaskGreen;
            if (mask & GfxColorWriteMaskB)
                mtlMask |= MTL::ColorWriteMaskBlue;
            if (mask & GfxColorWriteMaskA)
                mtlMask |= MTL::ColorWriteMaskAlpha;

            return mtlMask;
        }

        inline MTL::CompareFunction ToCompareFunction(GfxCompareFunc func)
        {
            switch (func)
            {
                case GfxCompareFunc::Never: return MTL::CompareFunctionNever;
                case GfxCompareFunc::Less: return MTL::CompareFunctionLess;
                case GfxCompareFunc::Equal: return MTL::CompareFunctionEqual;
                case GfxCompareFunc::LessEqual: return MTL::CompareFunctionLessEqual;
                case GfxCompareFunc::Greater: return MTL::CompareFunctionGreater;
                case GfxCompareFunc::NotEqual: return MTL::CompareFunctionNotEqual;
                case GfxCompareFunc::GreaterEqual: return MTL::CompareFunctionGreaterEqual;
                case GfxCompareFunc::Always: return MTL::CompareFunctionAlways;
                default: return MTL::CompareFunctionAlways;
            }
        }

        inline MTL::StencilOperation ToStencilOperation(GfxStencilOp stencil_op)
        {
            switch (stencil_op)
            {
                case GfxStencilOp::Keep: return MTL::StencilOperationKeep;
                case GfxStencilOp::Zero: return MTL::StencilOperationZero;
                case GfxStencilOp::Replace: return MTL::StencilOperationReplace;
                case GfxStencilOp::IncreaseClamp: return MTL::StencilOperationIncrementClamp;
                case GfxStencilOp::DecreaseClamp: return MTL::StencilOperationDecrementClamp;
                case GfxStencilOp::Invert: return MTL::StencilOperationInvert;
                case GfxStencilOp::IncreaseWrap: return MTL::StencilOperationIncrementWrap;
                case GfxStencilOp::DecreaseWrap: return MTL::StencilOperationDecrementWrap;
                default: return MTL::StencilOperationKeep;
            }
        }

        inline MTL::DepthStencilDescriptor* ToDepthStencilDescriptor(depth_stencilstate_t state)
        {
            MTL::DepthStencilDescriptor* descriptor = MTL::DepthStencilDescriptor::alloc()->init();
            descriptor->setDepthCompareFunction(state.depth_test ? ToCompareFunction(state.depth_func) : MTL::CompareFunctionAlways);
            descriptor->setDepthWriteEnabled(state.depth_write);

            if (state.stencil_test)
            {
                MTL::StencilDescriptor* front = descriptor->frontFaceStencil();
                front->setStencilCompareFunction(ToCompareFunction(state.front.stencil_func));
                front->setStencilFailureOperation(ToStencilOperation(state.front.stencil_fail));
                front->setDepthFailureOperation(ToStencilOperation(state.front.depth_fail));
                front->setDepthStencilPassOperation(ToStencilOperation(state.front.pass));
                front->setReadMask(state.stencil_read_mask);
                front->setWriteMask(state.stencil_write_mask);

                MTL::StencilDescriptor* back = descriptor->backFaceStencil();
                back->setStencilCompareFunction(ToCompareFunction(state.back.stencil_func));
                back->setStencilFailureOperation(ToStencilOperation(state.back.stencil_fail));
                back->setDepthFailureOperation(ToStencilOperation(state.back.depth_fail));
                back->setDepthStencilPassOperation(ToStencilOperation(state.back.pass));
                back->setReadMask(state.stencil_read_mask);
                back->setWriteMask(state.stencil_write_mask);
            }

            return descriptor;
        }

        inline MTL::CullMode ToCullMode(GfxCullMode mode)
        {
            switch (mode)
            {
                case GfxCullMode::Front: return MTL::CullModeFront;
                case GfxCullMode::Back: return MTL::CullModeBack;
                case GfxCullMode::None:
                default: return MTL::CullModeNone;
            }
        }

        inline MTL::SamplerMinMagFilter ToSamplerMinMagFilter(GfxFilter filter)
        {
            switch (filter)
            {
                case GfxFilter::Point: return MTL::SamplerMinMagFilterNearest;
                case GfxFilter::Linear: return MTL::SamplerMinMagFilterLinear;
                default: return MTL::SamplerMinMagFilterNearest;
            }
        }

        inline MTL::SamplerMipFilter ToSamplerMipFilter(GfxFilter filter)
        {
            switch (filter)
            {
                case GfxFilter::Point: return MTL::SamplerMipFilterNearest;
                case GfxFilter::Linear: return MTL::SamplerMipFilterLinear;
                default: return MTL::SamplerMipFilterNotMipmapped;
            }
        }

        inline MTL::SamplerAddressMode ToSamplerAddressMode(GfxSamplerAddressMode mode)
        {
            switch (mode)
            {
                case GfxSamplerAddressMode::Repeat: return MTL::SamplerAddressModeRepeat;
                case GfxSamplerAddressMode::MirroredRepeat: return MTL::SamplerAddressModeMirrorRepeat;
                case GfxSamplerAddressMode::ClampToEdge: return MTL::SamplerAddressModeClampToEdge;
                case GfxSamplerAddressMode::ClampToBorder: return MTL::SamplerAddressModeClampToBorderColor;
                default: return MTL::SamplerAddressModeRepeat;
            }
        }

        inline MTL::AccelerationStructureUsage ToAccelerationStructureUsage(GfxRayTracingASFlag flags)
        {
            MTL::AccelerationStructureUsage usage = MTL::AccelerationStructureUsageNone;

            if (flags & GfxRayTracingASFlagAllowUpdate)
            {
                usage |= MTL::AccelerationStructureUsageRefit;
            }

            if (flags & GfxRayTracingASFlagPreferFastBuild)
            {
                usage |= MTL::AccelerationStructureUsagePreferFastBuild;
            }

            return usage;
        }

        inline MTL::AttributeFormat ToAttributeFormat(GfxFormat format)
        {
            switch (format)
            {
                case Gfx::RGB32F: return MTL::AttributeFormatFloat3;
                case Gfx::RGBA32F: return MTL::AttributeFormatFloat4;
                case Gfx::RGBA16F: return MTL::AttributeFormatHalf4;
                default: ASSERT(false); return MTL::AttributeFormatInvalid;
            }
        }

        inline MTL::AccelerationStructureInstanceOptions ToAccelerationStructureInstanceOptions(GfxRayTracingInstanceFlag flags)
        {
            MTL::AccelerationStructureInstanceOptions options = MTL::AccelerationStructureInstanceOptionNone;

            if (flags & GfxRayTracingInstanceFlagDisableCull)
            {
                options |= MTL::AccelerationStructureInstanceOptionDisableTriangleCulling;
            }

            if (flags & GfxRayTracingInstanceFlagFrontFaceCCW)
            {
                options |= MTL::AccelerationStructureInstanceOptionTriangleFrontFacingWindingCounterClockwise;
            }

            if (flags & GfxRayTracingInstanceFlagForceOpaque)
            {
                options |= MTL::AccelerationStructureInstanceOptionOpaque;
            }

            if (flags & GfxRayTracingInstanceFlagForceNoOpaque)
            {
                options |= MTL::AccelerationStructureInstanceOptionNonOpaque;
            }

            return options;
        }

    }  // namespace ngfx
}  // namespace ncore
#endif
