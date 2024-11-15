#include "cgfx/gfx_defines.h"
#include "cgfx/gfx_texture.h"
#include "ccore/c_debug.h"

namespace ncore
{
    namespace ngfx
    {
        u32 GetFormatRowPitch(enums::format format, u32 width)
        {
            switch (format)
            {
                case enums::FORMAT_RGBA32F:
                case enums::FORMAT_RGBA32UI:
                case enums::FORMAT_RGBA32SI: return width * 16;
                case enums::FORMAT_RGB32F:
                case enums::FORMAT_RGB32UI:
                case enums::FORMAT_RGB32SI: return width * 12;
                case enums::FORMAT_RGBA16F:
                case enums::FORMAT_RGBA16UI:
                case enums::FORMAT_RGBA16SI:
                case enums::FORMAT_RGBA16UNORM:
                case enums::FORMAT_RGBA16SNORM: return width * 8;
                case enums::FORMAT_RGBA8UI:
                case enums::FORMAT_RGBA8SI:
                case enums::FORMAT_RGBA8UNORM:
                case enums::FORMAT_RGBA8SNORM:
                case enums::FORMAT_RGBA8SRGB:
                case enums::FORMAT_BGRA8UNORM:
                case enums::FORMAT_BGRA8SRGB:
                case enums::FORMAT_RGB10A2UNORM:
                case enums::FORMAT_R11G11B10F:
                case enums::FORMAT_RGB9E5: return width * 4;
                case enums::FORMAT_RG32F:
                case enums::FORMAT_RG32UI:
                case enums::FORMAT_RG32SI: return width * 8;
                case enums::FORMAT_RG16F:
                case enums::FORMAT_RG16UI:
                case enums::FORMAT_RG16SI:
                case enums::FORMAT_RG16UNORM:
                case enums::FORMAT_RG16SNORM: return width * 4;
                case enums::FORMAT_RG8UI:
                case enums::FORMAT_RG8SI:
                case enums::FORMAT_RG8UNORM:
                case enums::FORMAT_RG8SNORM: return width * 2;
                case enums::FORMAT_R32F:
                case enums::FORMAT_R32UI:
                case enums::FORMAT_R32SI: return width * 4;
                case enums::FORMAT_R16F:
                case enums::FORMAT_R16UI:
                case enums::FORMAT_R16SI:
                case enums::FORMAT_R16UNORM:
                case enums::FORMAT_R16SNORM: return width * 2;
                case enums::FORMAT_R8UI:
                case enums::FORMAT_R8SI:
                case enums::FORMAT_R8UNORM:
                case enums::FORMAT_R8SNORM: return width;
                case enums::FORMAT_BC1UNORM:
                case enums::FORMAT_BC1SRGB:
                case enums::FORMAT_BC4UNORM:
                case enums::FORMAT_BC4SNORM: return width / 2;
                case enums::FORMAT_BC2UNORM:
                case enums::FORMAT_BC2SRGB:
                case enums::FORMAT_BC3UNORM:
                case enums::FORMAT_BC3SRGB:
                case enums::FORMAT_BC5UNORM:
                case enums::FORMAT_BC5SNORM:
                case enums::FORMAT_BC6U16F:
                case enums::FORMAT_BC6S16F:
                case enums::FORMAT_BC7UNORM:
                case enums::FORMAT_BC7SRGB: return width;
                default: ASSERT(false); return 0;
            }
        }

        u32 GetFormatBlockWidth(enums::format format)
        {
            if (format >= enums::FORMAT_BC1UNORM && format <= enums::FORMAT_BC7SRGB)
                return 4;
            return 1;
        }

        u32 GetFormatBlockHeight(enums::format format)
        {
            if (format >= enums::FORMAT_BC1UNORM && format <= enums::FORMAT_BC7SRGB)
            {
                return 4;
            }

            return 1;
        }

        u32 GetFormatComponentNum(enums::format format)
        {
            switch (format)
            {
                case enums::FORMAT_RGBA32F:
                case enums::FORMAT_RGBA32UI:
                case enums::FORMAT_RGBA32SI:
                case enums::FORMAT_RGBA16F:
                case enums::FORMAT_RGBA16UI:
                case enums::FORMAT_RGBA16SI:
                case enums::FORMAT_RGBA16UNORM:
                case enums::FORMAT_RGBA16SNORM:
                case enums::FORMAT_RGBA8UI:
                case enums::FORMAT_RGBA8SI:
                case enums::FORMAT_RGBA8UNORM:
                case enums::FORMAT_RGBA8SNORM:
                case enums::FORMAT_RGBA8SRGB:
                case enums::FORMAT_BGRA8UNORM:
                case enums::FORMAT_BGRA8SRGB:
                case enums::FORMAT_RGB10A2UI:
                case enums::FORMAT_RGB10A2UNORM: return 4;
                case enums::FORMAT_RGB32F:
                case enums::FORMAT_RGB32UI:
                case enums::FORMAT_RGB32SI:
                case enums::FORMAT_R11G11B10F:
                case enums::FORMAT_RGB9E5: return 3;
                case enums::FORMAT_RG32F:
                case enums::FORMAT_RG32UI:
                case enums::FORMAT_RG32SI:
                case enums::FORMAT_RG16F:
                case enums::FORMAT_RG16UI:
                case enums::FORMAT_RG16SI:
                case enums::FORMAT_RG16UNORM:
                case enums::FORMAT_RG16SNORM:
                case enums::FORMAT_RG8UI:
                case enums::FORMAT_RG8SI:
                case enums::FORMAT_RG8UNORM:
                case enums::FORMAT_RG8SNORM: return 2;
                case enums::FORMAT_R32F:
                case enums::FORMAT_R32UI:
                case enums::FORMAT_R32SI:
                case enums::FORMAT_R16F:
                case enums::FORMAT_R16UI:
                case enums::FORMAT_R16SI:
                case enums::FORMAT_R16UNORM:
                case enums::FORMAT_R16SNORM:
                case enums::FORMAT_R8UI:
                case enums::FORMAT_R8SI:
                case enums::FORMAT_R8UNORM:
                case enums::FORMAT_R8SNORM: return 1;
                default: ASSERT(false); return 0;
            }
        }

        bool IsDepthFormat(enums::format format) { return format == enums::FORMAT_D32FS8 || format == enums::FORMAT_D32F || format == enums::FORMAT_D16; }
        bool IsStencilFormat(enums::format format) { return format == enums::FORMAT_D32FS8; }
        bool IsSRGBFormat(enums::format format) { return format == enums::FORMAT_RGBA8SRGB || format == enums::FORMAT_BGRA8SRGB; }
        u32  CalcSubresource(const texture_desc_t& desc, u32 mip, u32 slice) { return mip + desc.mip_levels * slice; }

        void DecomposeSubresource(const texture_desc_t& desc, u32 subresource, u32& mip, u32& slice)
        {
            mip   = subresource % desc.mip_levels;
            slice = (subresource / desc.mip_levels) % desc.array_size;
        }

    }  // namespace ngfx
}  // namespace ncore
