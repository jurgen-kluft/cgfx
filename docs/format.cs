namespace gfxformat
{
    class Program
    {
        public enum format
        {
            FORMAT_UNKNOWN,
            FORMAT_RGBA32F,
            FORMAT_RGBA32UI,
            FORMAT_RGBA32SI,
            FORMAT_RGBA16F,
            FORMAT_RGBA16UI,
            FORMAT_RGBA16SI,
            FORMAT_RGBA16UNORM,
            FORMAT_RGBA16SNORM,
            FORMAT_RGBA8UI,
            FORMAT_RGBA8SI,
            FORMAT_RGBA8UNORM,
            FORMAT_RGBA8SNORM,
            FORMAT_RGBA8SRGB,
            FORMAT_BGRA8UNORM,
            FORMAT_BGRA8SRGB,
            FORMAT_RGB10A2UI,
            FORMAT_RGB10A2UNORM,
            FORMAT_RGB32F,
            FORMAT_RGB32UI,
            FORMAT_RGB32SI,
            FORMAT_R11G11B10F,
            FORMAT_RGB9E5,
            FORMAT_RG32F,
            FORMAT_RG32UI,
            FORMAT_RG32SI,
            FORMAT_RG16F,
            FORMAT_RG16UI,
            FORMAT_RG16SI,
            FORMAT_RG16UNORM,
            FORMAT_RG16SNORM,
            FORMAT_RG8UI,
            FORMAT_RG8SI,
            FORMAT_RG8UNORM,
            FORMAT_RG8SNORM,
            FORMAT_R32F,
            FORMAT_R32UI,
            FORMAT_R32SI,
            FORMAT_R16F,
            FORMAT_R16UI,
            FORMAT_R16SI,
            FORMAT_R16UNORM,
            FORMAT_R16SNORM,
            FORMAT_R8UI,
            FORMAT_R8SI,
            FORMAT_R8UNORM,
            FORMAT_R8SNORM,
            FORMAT_D32F,
            FORMAT_D32FS8,
            FORMAT_D16,
            FORMAT_BC1UNORM,
            FORMAT_BC1SRGB,
            FORMAT_BC2UNORM,
            FORMAT_BC2SRGB,
            FORMAT_BC3UNORM,
            FORMAT_BC3SRGB,
            FORMAT_BC4UNORM,
            FORMAT_BC4SNORM,
            FORMAT_BC5UNORM,
            FORMAT_BC5SNORM,
            FORMAT_BC6U16F,
            FORMAT_BC6S16F,
            FORMAT_BC7UNORM,
            FORMAT_BC7SRGB,
        };

        public class GfxFormat
        {
            static int GetFormatRowPitch(format format, int width)
            {
                switch (format)
                {
                    case format.FORMAT_RGBA32F:
                    case format.FORMAT_RGBA32UI:
                    case format.FORMAT_RGBA32SI: return width * 16;
                    case format.FORMAT_RGB32F:
                    case format.FORMAT_RGB32UI:
                    case format.FORMAT_RGB32SI: return width * 12;
                    case format.FORMAT_RGBA16F:
                    case format.FORMAT_RGBA16UI:
                    case format.FORMAT_RGBA16SI:
                    case format.FORMAT_RGBA16UNORM:
                    case format.FORMAT_RGBA16SNORM: return width * 8;
                    case format.FORMAT_RGBA8UI:
                    case format.FORMAT_RGBA8SI:
                    case format.FORMAT_RGBA8UNORM:
                    case format.FORMAT_RGBA8SNORM:
                    case format.FORMAT_RGBA8SRGB:
                    case format.FORMAT_BGRA8UNORM:
                    case format.FORMAT_BGRA8SRGB:
                    case format.FORMAT_RGB10A2UNORM:
                    case format.FORMAT_R11G11B10F:
                    case format.FORMAT_RGB9E5: return width * 4;
                    case format.FORMAT_RG32F:
                    case format.FORMAT_RG32UI:
                    case format.FORMAT_RG32SI: return width * 8;
                    case format.FORMAT_RG16F:
                    case format.FORMAT_RG16UI:
                    case format.FORMAT_RG16SI:
                    case format.FORMAT_RG16UNORM:
                    case format.FORMAT_RG16SNORM: return width * 4;
                    case format.FORMAT_RG8UI:
                    case format.FORMAT_RG8SI:
                    case format.FORMAT_RG8UNORM:
                    case format.FORMAT_RG8SNORM: return width * 2;
                    case format.FORMAT_R32F:
                    case format.FORMAT_R32UI:
                    case format.FORMAT_R32SI: return width * 4;
                    case format.FORMAT_R16F:
                    case format.FORMAT_R16UI:
                    case format.FORMAT_R16SI:
                    case format.FORMAT_R16UNORM:
                    case format.FORMAT_R16SNORM: return width * 2;
                    case format.FORMAT_R8UI:
                    case format.FORMAT_R8SI:
                    case format.FORMAT_R8UNORM:
                    case format.FORMAT_R8SNORM: return width;
                    case format.FORMAT_BC1UNORM:
                    case format.FORMAT_BC1SRGB:
                    case format.FORMAT_BC4UNORM:
                    case format.FORMAT_BC4SNORM: return width / 2;
                    case format.FORMAT_BC2UNORM:
                    case format.FORMAT_BC2SRGB:
                    case format.FORMAT_BC3UNORM:
                    case format.FORMAT_BC3SRGB:
                    case format.FORMAT_BC5UNORM:
                    case format.FORMAT_BC5SNORM:
                    case format.FORMAT_BC6U16F:
                    case format.FORMAT_BC6S16F:
                    case format.FORMAT_BC7UNORM:
                    case format.FORMAT_BC7SRGB: return width;
                    default: return 0;
                }
            }

            public static int GetFormatBlockWidth(format format)
            {
                if (format >= format.FORMAT_BC1UNORM && format <= format.FORMAT_BC7SRGB)
                    return 4;
                return 1;
            }

            public static int GetFormatBlockHeight(format format)
            {
                if (format >= format.FORMAT_BC1UNORM && format <= format.FORMAT_BC7SRGB)
                    return 4;
                return 1;
            }

            public static int GetFormatComponentNum(format format)
            {
                switch (format)
                {
                    case format.FORMAT_RGBA32F:
                    case format.FORMAT_RGBA32UI:
                    case format.FORMAT_RGBA32SI:
                    case format.FORMAT_RGBA16F:
                    case format.FORMAT_RGBA16UI:
                    case format.FORMAT_RGBA16SI:
                    case format.FORMAT_RGBA16UNORM:
                    case format.FORMAT_RGBA16SNORM:
                    case format.FORMAT_RGBA8UI:
                    case format.FORMAT_RGBA8SI:
                    case format.FORMAT_RGBA8UNORM:
                    case format.FORMAT_RGBA8SNORM:
                    case format.FORMAT_RGBA8SRGB:
                    case format.FORMAT_BGRA8UNORM:
                    case format.FORMAT_BGRA8SRGB:
                    case format.FORMAT_RGB10A2UI:
                    case format.FORMAT_RGB10A2UNORM: return 4;
                    case format.FORMAT_RGB32F:
                    case format.FORMAT_RGB32UI:
                    case format.FORMAT_RGB32SI:
                    case format.FORMAT_R11G11B10F:
                    case format.FORMAT_RGB9E5: return 3;
                    case format.FORMAT_RG32F:
                    case format.FORMAT_RG32UI:
                    case format.FORMAT_RG32SI:
                    case format.FORMAT_RG16F:
                    case format.FORMAT_RG16UI:
                    case format.FORMAT_RG16SI:
                    case format.FORMAT_RG16UNORM:
                    case format.FORMAT_RG16SNORM:
                    case format.FORMAT_RG8UI:
                    case format.FORMAT_RG8SI:
                    case format.FORMAT_RG8UNORM:
                    case format.FORMAT_RG8SNORM: return 2;
                    case format.FORMAT_R32F:
                    case format.FORMAT_R32UI:
                    case format.FORMAT_R32SI:
                    case format.FORMAT_R16F:
                    case format.FORMAT_R16UI:
                    case format.FORMAT_R16SI:
                    case format.FORMAT_R16UNORM:
                    case format.FORMAT_R16SNORM:
                    case format.FORMAT_R8UI:
                    case format.FORMAT_R8SI:
                    case format.FORMAT_R8UNORM:
                    case format.FORMAT_R8SNORM: return 1;
                    case format.FORMAT_UNKNOWN: return 0;
                    default: break;
                }

                return 0;
            }

            public static bool IsDepthFormat(format format) { return format == format.FORMAT_D32FS8 || format == format.FORMAT_D32F || format == format.FORMAT_D16; }
            public static bool IsStencilFormat(format format) { return format == format.FORMAT_D32FS8; }
            public static bool IsSRGBFormat(format format) { return format == format.FORMAT_RGBA8SRGB || format == format.FORMAT_BGRA8SRGB; }

            // Output a C/C++ table like:
            //
            // struct format_compressed_t
            // {
            //     u16 RowPitchMul : 5;    // 5 bits for row pitch multiplier (*1, *2, *4, *8, *12, *16)
            //     u16 RowPitchDiv : 1;    // 1 bit for row pitch divisor (0=(/1), 1=(/2))
            //     u16 BlockWidth : 1;     // 1 bit for block width (4 or 1)
            //     u16 BlockHeight : 1;    // 1 bit for block height (4 or 1)
            //     u16 ComponentNum : 3;   // 3 bits for component num (0, 1, 2, 3, or 4)
            //     u16 DepthFormat : 1;    // 1 bit for depth format
            //     u16 StencilFormat : 1;  // 1 bit for stencil format
            //     u16 SRGBFormat : 1;     // 1 bit for SRGB format
            // };
            //
            // static const format_compressed_t s_format_table[] =
            // {
            //     [FORMAT_RGBA32F] = format_compressed_t{16, 1, 1, 4, 0, 0, 0},
            //     [FORMAT_RGBA32UI] = format_compressed_t{16, 1, 1, 4, 0, 0, 0},
            //     ...
            // };
            public static int Main(string[] args)
            {
                Console.WriteLine("struct format_compressed_t");
                Console.WriteLine("{");
                Console.WriteLine("    u16 RowPitchMul : 5;    // 5 bits for row pitch multiplier (*1, *2, *4, *8, *12, *16)");
                Console.WriteLine("    u16 RowPitchDiv : 1;    // 1 bit for row pitch divisor (0=(/1), 1=(/2))");
                Console.WriteLine("    u16 BlockWidth : 1;     // 1 bit for block width (4 or 1)");
                Console.WriteLine("    u16 BlockHeight : 1;    // 1 bit for block height (4 or 1)");
                Console.WriteLine("    u16 ComponentNum : 3;   // 3 bits for component num (0, 1, 2, 3, or 4)");
                Console.WriteLine("    u16 DepthFormat : 1;    // 1 bit for depth format");
                Console.WriteLine("    u16 StencilFormat : 1;  // 1 bit for stencil format");
                Console.WriteLine("    u16 SRGBFormat : 1;     // 1 bit for SRGB format");
                Console.WriteLine("};");

                Console.WriteLine("static const format_compressed_t s_format_table[] =");
                Console.WriteLine("{");

                // Iterate over all format enums
                foreach (var f in Enum.GetValues<format>())
                {
                    var rowPitch = GetFormatRowPitch(f, 2) / 2;
                    var rowDiv = GetFormatRowPitch(f, 2) == 1 ? 1 : 0;
                    var blockWidth = GetFormatBlockWidth(f);
                    var blockHeight = GetFormatBlockHeight(f);
                    var componentNum = GetFormatComponentNum(f);
                    var isDepthFormat = IsDepthFormat(f) ? 0 : 1;
                    var isStencilFormat = IsStencilFormat(f) ? 0 : 1;
                    var isSrgb = IsSRGBFormat(f) ? 0 : 1;
                    Console.WriteLine("    [{0}] = format_compressed_t{{{1}, {2}, {3}, {4}, {5}, {6}, {7}, {8} }},", f.ToString(), rowPitch, rowDiv, blockWidth, blockHeight, componentNum, isDepthFormat, isStencilFormat, isSrgb);
                }

                Console.WriteLine("};");
                return 0;
            }
        }
    }
}
