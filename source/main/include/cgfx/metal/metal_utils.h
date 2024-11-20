#ifndef __CGFX_METAL_UTILS_H__
#define __CGFX_METAL_UTILS_H__

#include "cbase/c_debug.h"
#include "cgfx/gfx_defines.h"
#include "cgfx/gfx_texture.h"

#ifdef TARGET_MAC

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

        inline MTL::ResourceOptions ToResourceOptions(enums::memory type)
        {
            MTL::ResourceOptions options = MTL::ResourceHazardTrackingModeTracked;

            switch (type)
            {
                case enums::MemoryGpuOnly: options |= MTL::ResourceStorageModePrivate; break;
                case enums::MemoryCpuOnly:
                case enums::MemoryCpuToGpu: options |= MTL::ResourceStorageModeShared | MTL::ResourceCPUCacheModeWriteCombined; break;
                case enums::MemoryGpuToCpu: options |= MTL::ResourceStorageModeShared; break;
                default: break;
            }

            return options;
        }
        inline MTL::ResourceOptions ToResourceOptions(enums::memory_t type) { return ToResourceOptions(enums::cast<enums::memory>(type)); }

        inline MTL::TextureType ToTextureType(enums::texture_type type)
        {
            switch (type)
            {
                case enums::TextureType2D: return MTL::TextureType2D;
                case enums::TextureType2DArray: return MTL::TextureType2DArray;
                case enums::TextureType3D: return MTL::TextureType3D;
                case enums::TextureTypeCube: return MTL::TextureTypeCube;
                case enums::TextureTypeCubeArray: return MTL::TextureTypeCubeArray;
                default: return MTL::TextureType2D;
            }
        }
        inline MTL::TextureType ToTextureType(enums::texture_type_t type) { return ToTextureType(enums::cast<enums::texture_type>(type)); }

        inline MTL::PixelFormat ToPixelFormat(enums::format format)
        {
            switch (format)
            {
                case enums::FORMAT_UNKNOWN: return MTL::PixelFormatInvalid;
                case enums::FORMAT_RGBA32F: return MTL::PixelFormatRGBA32Float;
                case enums::FORMAT_RGBA32UI: return MTL::PixelFormatRGBA32Uint;
                case enums::FORMAT_RGBA32SI: return MTL::PixelFormatRGBA32Sint;
                case enums::FORMAT_RGBA16F: return MTL::PixelFormatRGBA16Float;
                case enums::FORMAT_RGBA16UI: return MTL::PixelFormatRGBA16Uint;
                case enums::FORMAT_RGBA16SI: return MTL::PixelFormatRGBA16Sint;
                case enums::FORMAT_RGBA16UNORM: return MTL::PixelFormatRGBA16Unorm;
                case enums::FORMAT_RGBA16SNORM: return MTL::PixelFormatRGBA16Snorm;
                case enums::FORMAT_RGBA8UI: return MTL::PixelFormatRGBA8Uint;
                case enums::FORMAT_RGBA8SI: return MTL::PixelFormatRGBA8Sint;
                case enums::FORMAT_RGBA8UNORM: return MTL::PixelFormatRGBA8Unorm;
                case enums::FORMAT_RGBA8SNORM: return MTL::PixelFormatRGBA8Snorm;
                case enums::FORMAT_RGBA8SRGB: return MTL::PixelFormatRGBA8Unorm_sRGB;
                case enums::FORMAT_BGRA8UNORM: return MTL::PixelFormatBGRA8Unorm;
                case enums::FORMAT_BGRA8SRGB: return MTL::PixelFormatBGRA8Unorm_sRGB;
                case enums::FORMAT_RGB10A2UI: return MTL::PixelFormatRGB10A2Uint;
                case enums::FORMAT_RGB10A2UNORM: return MTL::PixelFormatRGB10A2Unorm;
                case enums::FORMAT_RGB32F: return MTL::PixelFormatInvalid;
                case enums::FORMAT_RGB32UI: return MTL::PixelFormatInvalid;
                case enums::FORMAT_RGB32SI: return MTL::PixelFormatInvalid;
                case enums::FORMAT_R11G11B10F: return MTL::PixelFormatRG11B10Float;
                case enums::FORMAT_RGB9E5: return MTL::PixelFormatRGB9E5Float;
                case enums::FORMAT_RG32F: return MTL::PixelFormatRG32Float;
                case enums::FORMAT_RG32UI: return MTL::PixelFormatRG32Uint;
                case enums::FORMAT_RG32SI: return MTL::PixelFormatRG32Sint;
                case enums::FORMAT_RG16F: return MTL::PixelFormatRG16Float;
                case enums::FORMAT_RG16UI: return MTL::PixelFormatRG16Uint;
                case enums::FORMAT_RG16SI: return MTL::PixelFormatRG16Sint;
                case enums::FORMAT_RG16UNORM: return MTL::PixelFormatRG16Unorm;
                case enums::FORMAT_RG16SNORM: return MTL::PixelFormatRG16Snorm;
                case enums::FORMAT_RG8UI: return MTL::PixelFormatRG8Uint;
                case enums::FORMAT_RG8SI: return MTL::PixelFormatRG8Sint;
                case enums::FORMAT_RG8UNORM: return MTL::PixelFormatRG8Unorm;
                case enums::FORMAT_RG8SNORM: return MTL::PixelFormatRG8Snorm;
                case enums::FORMAT_R32F: return MTL::PixelFormatR32Float;
                case enums::FORMAT_R32UI: return MTL::PixelFormatR32Uint;
                case enums::FORMAT_R32SI: return MTL::PixelFormatR32Sint;
                case enums::FORMAT_R16F: return MTL::PixelFormatR16Float;
                case enums::FORMAT_R16UI: return MTL::PixelFormatR16Uint;
                case enums::FORMAT_R16SI: return MTL::PixelFormatR16Sint;
                case enums::FORMAT_R16UNORM: return MTL::PixelFormatR16Unorm;
                case enums::FORMAT_R16SNORM: return MTL::PixelFormatR16Snorm;
                case enums::FORMAT_R8UI: return MTL::PixelFormatR8Uint;
                case enums::FORMAT_R8SI: return MTL::PixelFormatR8Sint;
                case enums::FORMAT_R8UNORM: return MTL::PixelFormatR8Unorm;
                case enums::FORMAT_R8SNORM: return MTL::PixelFormatR8Snorm;
                case enums::FORMAT_D32F: return MTL::PixelFormatDepth32Float;
                case enums::FORMAT_D32FS8: return MTL::PixelFormatDepth32Float_Stencil8;
                case enums::FORMAT_D16: return MTL::PixelFormatDepth16Unorm;
                case enums::FORMAT_BC1UNORM: return MTL::PixelFormatBC1_RGBA;
                case enums::FORMAT_BC1SRGB: return MTL::PixelFormatBC1_RGBA_sRGB;
                case enums::FORMAT_BC2UNORM: return MTL::PixelFormatBC2_RGBA;
                case enums::FORMAT_BC2SRGB: return MTL::PixelFormatBC2_RGBA_sRGB;
                case enums::FORMAT_BC3UNORM: return MTL::PixelFormatBC3_RGBA;
                case enums::FORMAT_BC3SRGB: return MTL::PixelFormatBC3_RGBA_sRGB;
                case enums::FORMAT_BC4UNORM: return MTL::PixelFormatBC4_RUnorm;
                case enums::FORMAT_BC4SNORM: return MTL::PixelFormatBC4_RSnorm;
                case enums::FORMAT_BC5UNORM: return MTL::PixelFormatBC5_RGUnorm;
                case enums::FORMAT_BC5SNORM: return MTL::PixelFormatBC5_RGSnorm;
                case enums::FORMAT_BC6U16F: return MTL::PixelFormatBC6H_RGBUfloat;
                case enums::FORMAT_BC6S16F: return MTL::PixelFormatBC6H_RGBFloat;
                case enums::FORMAT_BC7UNORM: return MTL::PixelFormatBC7_RGBAUnorm;
                case enums::FORMAT_BC7SRGB: return MTL::PixelFormatBC7_RGBAUnorm_sRGB;
                default: return MTL::PixelFormatInvalid;
            }
        }
        inline MTL::PixelFormat ToPixelFormat(enums::format_t format) { return ToPixelFormat(enums::cast<enums::format>(format)); }

        inline MTL::TextureUsage ToTextureUsage(enums::texture_usage flags)
        {
            MTL::TextureUsage usage = MTL::TextureUsageShaderRead | MTL::TextureUsagePixelFormatView;

            if (flags & (enums::TextureUsageRenderTarget | enums::TextureUsageDepthStencil))
            {
                usage |= MTL::TextureUsageRenderTarget;
            }

            if (flags & enums::TextureUsageUnorderedAccess)
            {
                usage |= MTL::TextureUsageShaderWrite;  // todo TextureUsageShaderAtomic
            }

            return usage;
        }
        inline MTL::TextureUsage ToTextureUsage(enums::texture_usage_t flags) { return ToTextureUsage(enums::cast<enums::texture_usage>(flags)); }

        inline MTL::TextureDescriptor* ToTextureDescriptor(const texture_desc_t& desc)
        {
            MTL::TextureDescriptor* descriptor = MTL::TextureDescriptor::alloc()->init();
            descriptor->setWidth(desc.width);
            descriptor->setHeight(desc.height);
            descriptor->setDepth(desc.depth);
            descriptor->setMipmapLevelCount(desc.mip_levels);
            if (desc.type == enums::TextureTypeCube || desc.type == enums::TextureTypeCubeArray)
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

        inline MTL::LoadAction ToLoadAction(enums::load_op loadOp)
        {
            switch (loadOp)
            {
                case enums::LoadOpLoad: return MTL::LoadActionLoad;
                case enums::LoadOpClear: return MTL::LoadActionClear;
                case enums::LoadOpDontCare: return MTL::LoadActionDontCare;
                default: return MTL::LoadActionLoad;
            }
        }

        inline MTL::StoreAction ToStoreAction(enums::store_op storeOp)
        {
            switch (storeOp)
            {
                case enums::StoreOpStore: return MTL::StoreActionStore;
                case enums::StoreOpDontCare: return MTL::StoreActionDontCare;
                default: return MTL::StoreActionStore;
            }
        }

        inline MTL::PrimitiveTopologyClass ToTopologyClass(enums::primitive type)
        {
            switch (type)
            {
                case enums::PrimitivePointList: return MTL::PrimitiveTopologyClassPoint;
                case enums::PrimitiveLineList:
                case enums::PrimitiveLineStrip: return MTL::PrimitiveTopologyClassLine;
                case enums::PrimitiveTriangleList:
                case enums::PrimitiveTriangleTrip: return MTL::PrimitiveTopologyClassTriangle;
                default: return MTL::PrimitiveTopologyClassUnspecified;
            }
        }
        inline MTL::PrimitiveTopologyClass ToTopologyClass(enums::primitive_t type) { return ToTopologyClass(enums::cast<enums::primitive>(type)); }

        inline MTL::PrimitiveType ToPrimitiveType(enums::primitive type)
        {
            switch (type)
            {
                case enums::PrimitivePointList: return MTL::PrimitiveTypePoint;
                case enums::PrimitiveLineList: return MTL::PrimitiveTypeLine;
                case enums::PrimitiveLineStrip: return MTL::PrimitiveTypeLineStrip;
                case enums::PrimitiveTriangleList: return MTL::PrimitiveTypeTriangle;
                case enums::PrimitiveTriangleTrip: return MTL::PrimitiveTypeTriangleStrip;
                default: return MTL::PrimitiveTypeTriangle;
            }
        }
        inline MTL::PrimitiveType ToPrimitiveType(enums::primitive_t type) { return ToPrimitiveType(enums::cast<enums::primitive>(type)); }

        inline MTL::BlendFactor ToBlendFactor(enums::blendfactor blend_factor)
        {
            switch (blend_factor)
            {
                case enums::BlendFactorZero: return MTL::BlendFactorZero;
                case enums::BlendFactorOne: return MTL::BlendFactorOne;
                case enums::BlendFactorSrcColor: return MTL::BlendFactorSourceColor;
                case enums::BlendFactorInvSrcColor: return MTL::BlendFactorOneMinusSourceColor;
                case enums::BlendFactorSrcAlpha: return MTL::BlendFactorSourceAlpha;
                case enums::BlendFactorInvSrcAlpha: return MTL::BlendFactorOneMinusSourceAlpha;
                case enums::BlendFactorDstAlpha: return MTL::BlendFactorDestinationAlpha;
                case enums::BlendFactorInvDstAlpha: return MTL::BlendFactorOneMinusDestinationAlpha;
                case enums::BlendFactorDstColor: return MTL::BlendFactorDestinationColor;
                case enums::BlendFactorInvDstColor: return MTL::BlendFactorOneMinusDestinationColor;
                case enums::BlendFactorSrcAlphaClamp: return MTL::BlendFactorSourceAlphaSaturated;
                case enums::BlendFactorConstantFactor: return MTL::BlendFactorBlendColor;
                case enums::BlendFactorInvConstantFactor: return MTL::BlendFactorOneMinusBlendColor;
                default: return MTL::BlendFactorZero;
            }
        }
        inline MTL::BlendFactor ToBlendFactor(enums::blendfactor_t blend_factor) { return ToBlendFactor(enums::cast<enums::blendfactor>(blend_factor)); }

        inline MTL::BlendOperation ToBlendOperation(enums::blendop blend_op)
        {
            switch (blend_op)
            {
                case enums::BlendOpAdd: return MTL::BlendOperationAdd;
                case enums::BlendOpSubtract: return MTL::BlendOperationSubtract;
                case enums::BlendOpReverseSubtract: return MTL::BlendOperationReverseSubtract;
                case enums::BlendOpMin: return MTL::BlendOperationMin;
                case enums::BlendOpMax: return MTL::BlendOperationMax;
                default: return MTL::BlendOperationAdd;
            }
        }
        inline MTL::BlendOperation ToBlendOperation(enums::blendop_t blend_op) { return ToBlendOperation(enums::cast<enums::blendop>(blend_op)); }

        inline MTL::ColorWriteMask ToColorWriteMask(enums::colorwritemask mask)
        {
            MTL::ColorWriteMask mtlMask = MTL::ColorWriteMaskNone;

            if (mask & enums::ColorWriteMaskR)
                mtlMask |= MTL::ColorWriteMaskRed;
            if (mask & enums::ColorWriteMaskG)
                mtlMask |= MTL::ColorWriteMaskGreen;
            if (mask & enums::ColorWriteMaskB)
                mtlMask |= MTL::ColorWriteMaskBlue;
            if (mask & enums::ColorWriteMaskA)
                mtlMask |= MTL::ColorWriteMaskAlpha;

            return mtlMask;
        }
        inline MTL::ColorWriteMask ToColorWriteMask(enums::colorwritemask_t mask) { return ToColorWriteMask(enums::cast<enums::colorwritemask>(mask)); }

        inline MTL::CompareFunction ToCompareFunction(enums::comparefunc func)
        {
            switch (func)
            {
                case enums::CompareFuncNever: return MTL::CompareFunctionNever;
                case enums::CompareFuncLess: return MTL::CompareFunctionLess;
                case enums::CompareFuncEqual: return MTL::CompareFunctionEqual;
                case enums::CompareFuncLessEqual: return MTL::CompareFunctionLessEqual;
                case enums::CompareFuncGreater: return MTL::CompareFunctionGreater;
                case enums::CompareFuncNotEqual: return MTL::CompareFunctionNotEqual;
                case enums::CompareFuncGreaterEqual: return MTL::CompareFunctionGreaterEqual;
                case enums::CompareFuncAlways: return MTL::CompareFunctionAlways;
                default: return MTL::CompareFunctionAlways;
            }
        }
        inline MTL::CompareFunction ToCompareFunction(enums::comparefunc_t func) { return ToCompareFunction(enums::cast<enums::comparefunc>(func)); }

        inline MTL::StencilOperation ToStencilOperation(enums::stencil stencil_op)
        {
            switch (stencil_op)
            {
                case enums::StencilKeep: return MTL::StencilOperationKeep;
                case enums::StencilZero: return MTL::StencilOperationZero;
                case enums::StencilReplace: return MTL::StencilOperationReplace;
                case enums::StencilIncreaseClamp: return MTL::StencilOperationIncrementClamp;
                case enums::StencilDecreaseClamp: return MTL::StencilOperationDecrementClamp;
                case enums::StencilInvert: return MTL::StencilOperationInvert;
                case enums::StencilIncreaseWrap: return MTL::StencilOperationIncrementWrap;
                case enums::StencilDecreaseWrap: return MTL::StencilOperationDecrementWrap;
                default: return MTL::StencilOperationKeep;
            }
        }
        inline MTL::StencilOperation ToStencilOperation(enums::stencil_t stencil_op) { return ToStencilOperation(enums::cast<enums::stencil>(stencil_op)); }

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

        inline MTL::CullMode ToCullMode(enums::cullmode mode)
        {
            switch (mode)
            {
                case enums::CullFront: return MTL::CullModeFront;
                case enums::CullBack: return MTL::CullModeBack;
                case enums::CullNone:
                default: return MTL::CullModeNone;
            }
        }
        inline MTL::CullMode ToCullMode(enums::cullmode_t mode) { return enums::cast<enums::cullmode>(mode); }

        inline MTL::SamplerMinMagFilter ToSamplerMinMagFilter(enums::filter filter)
        {
            switch (filter)
            {
                case enums::FilterPoint: return MTL::SamplerMinMagFilterNearest;
                case enums::FilterLinear: return MTL::SamplerMinMagFilterLinear;
                default: return MTL::SamplerMinMagFilterNearest;
            }
        }

        inline MTL::SamplerMipFilter ToSamplerMipFilter(enums::filter filter)
        {
            switch (filter)
            {
                case enums::FilterPoint: return MTL::SamplerMipFilterNearest;
                case enums::FilterLinear: return MTL::SamplerMipFilterLinear;
                default: return MTL::SamplerMipFilterNotMipmapped;
            }
        }

        inline MTL::SamplerAddressMode ToSamplerAddressMode(enums::sampler_address_mode mode)
        {
            switch (mode)
            {
                case enums::SamplerAddressModeRepeat: return MTL::SamplerAddressModeRepeat;
                case enums::SamplerAddressModeMirroredRepeat: return MTL::SamplerAddressModeMirrorRepeat;
                case enums::SamplerAddressModeClampToEdge: return MTL::SamplerAddressModeClampToEdge;
                case enums::SamplerAddressModeClampToBorder: return MTL::SamplerAddressModeClampToBorderColor;
                default: return MTL::SamplerAddressModeRepeat;
            }
        }

        inline MTL::AccelerationStructureUsage ToAccelerationStructureUsage(enums::rt::accstruct_flag flags)
        {
            MTL::AccelerationStructureUsage usage = MTL::AccelerationStructureUsageNone;

            if (flags & enums::rt::AsFlagAllowUpdate)
            {
                usage |= MTL::AccelerationStructureUsageRefit;
            }

            if (flags & enums::rt::AsFlagPreferFastBuild)
            {
                usage |= MTL::AccelerationStructureUsagePreferFastBuild;
            }

            return usage;
        }
        inline MTL::AccelerationStructureUsage ToAccelerationStructureUsage(enums::rt::accstruct_flag_t flags) { return ToAccelerationStructureUsage(enums::cast<enums::rt::accstruct_flag>(flags)); }

        inline MTL::AttributeFormat ToAttributeFormat(enums::format format)
        {
            switch (format)
            {
                case enums::FORMAT_RGB32F: return MTL::AttributeFormatFloat3;
                case enums::FORMAT_RGBA32F: return MTL::AttributeFormatFloat4;
                case enums::FORMAT_RGBA16F: return MTL::AttributeFormatHalf4;
                default: ASSERT(false); return MTL::AttributeFormatInvalid;
            }
        }

        inline MTL::AccelerationStructureInstanceOptions ToAccelerationStructureInstanceOptions(enums::rt::instance_flag flags)
        {
            MTL::AccelerationStructureInstanceOptions options = MTL::AccelerationStructureInstanceOptionNone;

            if (flags & enums::rt::DisableCull)
            {
                options |= MTL::AccelerationStructureInstanceOptionDisableTriangleCulling;
            }

            if (flags & enums::rt::FrontFaceCCW)
            {
                options |= MTL::AccelerationStructureInstanceOptionTriangleFrontFacingWindingCounterClockwise;
            }

            if (flags & enums::rt::ForceOpaque)
            {
                options |= MTL::AccelerationStructureInstanceOptionOpaque;
            }

            if (flags & enums::rt::ForceNoOpaque)
            {
                options |= MTL::AccelerationStructureInstanceOptionNonOpaque;
            }

            return options;
        }
        inline MTL::AccelerationStructureInstanceOptions ToAccelerationStructureInstanceOptions(enums::rt::instance_flag_t flags) { return ToAccelerationStructureInstanceOptions(enums::cast<enums::rt::instance_flag>(flags)); }

    }  // namespace ngfx
}  // namespace ncore

#endif

#endif
