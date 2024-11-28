namespace gfxformat
{
    class Program
    {
        public enum format
        {
            FORMAT_UNKNOWN = 0,
            FORMAT_RGBA32F = 1,
            FORMAT_RGBA32UI = 2,
            FORMAT_RGBA32SI = 3,
            FORMAT_RGBA16F = 4,
            FORMAT_RGBA16UI = 5,
            FORMAT_RGBA16SI = 6,
            FORMAT_RGBA16UNORM = 7,
            FORMAT_RGBA16SNORM = 8,
            FORMAT_RGBA8UI = 9,
            FORMAT_RGBA8SI = 10,
            FORMAT_RGBA8UNORM = 11,
            FORMAT_RGBA8SNORM = 12,
            FORMAT_RGBA8SRGB = 13,
            FORMAT_BGRA8UNORM = 14,
            FORMAT_BGRA8SRGB = 15,
            FORMAT_RGB10A2UI = 16,
            FORMAT_RGB10A2UNORM = 17,
            FORMAT_RGB32F = 18,
            FORMAT_RGB32UI = 19,
            FORMAT_RGB32SI = 20,
            FORMAT_R11G11B10F = 21,
            FORMAT_RGB9E5 = 22,
            FORMAT_RG32F = 23,
            FORMAT_RG32UI = 24,
            FORMAT_RG32SI = 25,
            FORMAT_RG16F = 26,
            FORMAT_RG16UI = 27,
            FORMAT_RG16SI = 28,
            FORMAT_RG16UNORM = 29,
            FORMAT_RG16SNORM = 30,
            FORMAT_RG8UI = 31,
            FORMAT_RG8SI = 32,
            FORMAT_RG8UNORM = 33,
            FORMAT_RG8SNORM = 34,
            FORMAT_R32F = 35,
            FORMAT_R32UI = 36,
            FORMAT_R32SI = 37,
            FORMAT_R16F = 38,
            FORMAT_R16UI = 39,
            FORMAT_R16SI = 40,
            FORMAT_R16UNORM = 41,
            FORMAT_R16SNORM = 42,
            FORMAT_R8UI = 43,
            FORMAT_R8SI = 44,
            FORMAT_R8UNORM = 45,
            FORMAT_R8SNORM = 46,
            FORMAT_D32F = 47,
            FORMAT_D32FS8 = 48,
            FORMAT_D16 = 49,
            FORMAT_BC1UNORM = 50,
            FORMAT_BC1SRGB = 51,
            FORMAT_BC2UNORM = 52,
            FORMAT_BC2SRGB = 53,
            FORMAT_BC3UNORM = 54,
            FORMAT_BC3SRGB = 55,
            FORMAT_BC4UNORM = 56,
            FORMAT_BC4SNORM = 57,
            FORMAT_BC5UNORM = 58,
            FORMAT_BC5SNORM = 59,
            FORMAT_BC6U16F = 60,
            FORMAT_BC6S16F = 61,
            FORMAT_BC7UNORM = 62,
            FORMAT_BC7SRGB = 63,
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
            //     format_compressed_t{16, 1, 1, 4, 0, 0, 0},
            //     format_compressed_t{16, 1, 1, 4, 0, 0, 0},
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
                    Console.WriteLine("    format_compressed_t{{{1}, {2}, {3}, {4}, {5}, {6}, {7}, {8} }},", f.ToString(), rowPitch, rowDiv, blockWidth, blockHeight, componentNum, isDepthFormat, isStencilFormat, isSrgb);
                }

                Console.WriteLine("};");
                return 0;
            }
        }
    }
}
