#include "cgfx/gfx.h"
// #include "microprofile/microprofile.h"

#ifdef TARGET_PC
    #include "cgfx/d3d12/d3d12_device.h"
#endif

#ifdef TARGET_MAC
    #include "cgfx/metal/metal_device.h"
#endif

#include "cgfx/mock/mock_device.h"

namespace ncore
{
    namespace ngfx
    {
        IGfxDevice* CreateGfxDevice(const GfxDeviceDesc& desc)
        {
            IGfxDevice* pDevice = nullptr;

            switch (desc.backend)
            {
#ifdef TARGET_PC
                case GfxRender::D3D12: pDevice = new D3D12Device(desc); break;
#endif
#ifdef TARGET_MAC
                case GfxRender::Metal: pDevice = new MetalDevice(desc); break;
#endif
                case GfxRender::Mock: pDevice = new MockDevice(desc); break;
                default: break;
            }

            if (pDevice && !pDevice->Create())
            {
                delete pDevice;
                pDevice = nullptr;
            }

            return pDevice;
        }

        void BeginMPGpuEvent(IGfxCommandList* pCommandList, const char* event_name)
        {
#if MICROPROFILE_GPU_TIMERS
            static const u32 EVENT_COLOR[] = {
              MP_LIGHTCYAN4, MP_SKYBLUE2, MP_SEAGREEN4, MP_LIGHTGOLDENROD4, MP_BROWN3, MP_MEDIUMPURPLE2, MP_SIENNA, MP_LIMEGREEN, MP_MISTYROSE, MP_LIGHTYELLOW,
            };

            u32 color_count = sizeof(EVENT_COLOR) / sizeof(EVENT_COLOR[0]);
            u32 color       = EVENT_COLOR[XXH32(event_name.c_str(), strlen(event_name.c_str()), 0) % color_count];

            MicroProfileToken token = MicroProfileGetToken("GPU", event_name.c_str(), color, MicroProfileTokenTypeGpu);

            MicroProfileEnterGpu(token, pCommandList->GetProfileLog());
#endif
        }

        void EndMPGpuEvent(IGfxCommandList* pCommandList)
        {
#if MICROPROFILE_GPU_TIMERS
            MicroProfileLeaveGpu(pCommandList->GetProfileLog());
#endif
        }

        u32 GetFormatRowPitch(GfxFormat format, u32 width)
        {
            switch (format)
            {
                case Gfx::RGBA32F:
                case Gfx::RGBA32UI:
                case Gfx::RGBA32SI: return width * 16;
                case Gfx::RGB32F:
                case Gfx::RGB32UI:
                case Gfx::RGB32SI: return width * 12;
                case Gfx::RGBA16F:
                case Gfx::RGBA16UI:
                case Gfx::RGBA16SI:
                case Gfx::RGBA16UNORM:
                case Gfx::RGBA16SNORM: return width * 8;
                case Gfx::RGBA8UI:
                case Gfx::RGBA8SI:
                case Gfx::RGBA8UNORM:
                case Gfx::RGBA8SNORM:
                case Gfx::RGBA8SRGB:
                case Gfx::BGRA8UNORM:
                case Gfx::BGRA8SRGB:
                case Gfx::RGB10A2UNORM:
                case Gfx::R11G11B10F:
                case Gfx::RGB9E5: return width * 4;
                case Gfx::RG32F:
                case Gfx::RG32UI:
                case Gfx::RG32SI: return width * 8;
                case Gfx::RG16F:
                case Gfx::RG16UI:
                case Gfx::RG16SI:
                case Gfx::RG16UNORM:
                case Gfx::RG16SNORM: return width * 4;
                case Gfx::RG8UI:
                case Gfx::RG8SI:
                case Gfx::RG8UNORM:
                case Gfx::RG8SNORM: return width * 2;
                case Gfx::R32F:
                case Gfx::R32UI:
                case Gfx::R32SI: return width * 4;
                case Gfx::R16F:
                case Gfx::R16UI:
                case Gfx::R16SI:
                case Gfx::R16UNORM:
                case Gfx::R16SNORM: return width * 2;
                case Gfx::R8UI:
                case Gfx::R8SI:
                case Gfx::R8UNORM:
                case Gfx::R8SNORM: return width;
                case Gfx::BC1UNORM:
                case Gfx::BC1SRGB:
                case Gfx::BC4UNORM:
                case Gfx::BC4SNORM: return width / 2;
                case Gfx::BC2UNORM:
                case Gfx::BC2SRGB:
                case Gfx::BC3UNORM:
                case Gfx::BC3SRGB:
                case Gfx::BC5UNORM:
                case Gfx::BC5SNORM:
                case Gfx::BC6U16F:
                case Gfx::BC6S16F:
                case Gfx::BC7UNORM:
                case Gfx::BC7SRGB: return width;
                default: ASSERT(false); return 0;
            }
        }

        u32 GetFormatBlockWidth(GfxFormat format)
        {
            if (format >= Gfx::BC1UNORM && format <= Gfx::BC7SRGB)
            {
                return 4;
            }

            return 1;
        }

        u32 GetFormatBlockHeight(GfxFormat format)
        {
            if (format >= Gfx::BC1UNORM && format <= Gfx::BC7SRGB)
            {
                return 4;
            }

            return 1;
        }

        u32 GetFormatComponentNum(GfxFormat format)
        {
            switch (format)
            {
                case Gfx::RGBA32F:
                case Gfx::RGBA32UI:
                case Gfx::RGBA32SI:
                case Gfx::RGBA16F:
                case Gfx::RGBA16UI:
                case Gfx::RGBA16SI:
                case Gfx::RGBA16UNORM:
                case Gfx::RGBA16SNORM:
                case Gfx::RGBA8UI:
                case Gfx::RGBA8SI:
                case Gfx::RGBA8UNORM:
                case Gfx::RGBA8SNORM:
                case Gfx::RGBA8SRGB:
                case Gfx::BGRA8UNORM:
                case Gfx::BGRA8SRGB:
                case Gfx::RGB10A2UI:
                case Gfx::RGB10A2UNORM: return 4;
                case Gfx::RGB32F:
                case Gfx::RGB32UI:
                case Gfx::RGB32SI:
                case Gfx::R11G11B10F:
                case Gfx::RGB9E5: return 3;
                case Gfx::RG32F:
                case Gfx::RG32UI:
                case Gfx::RG32SI:
                case Gfx::RG16F:
                case Gfx::RG16UI:
                case Gfx::RG16SI:
                case Gfx::RG16UNORM:
                case Gfx::RG16SNORM:
                case Gfx::RG8UI:
                case Gfx::RG8SI:
                case Gfx::RG8UNORM:
                case Gfx::RG8SNORM: return 2;
                case Gfx::R32F:
                case Gfx::R32UI:
                case Gfx::R32SI:
                case Gfx::R16F:
                case Gfx::R16UI:
                case Gfx::R16SI:
                case Gfx::R16UNORM:
                case Gfx::R16SNORM:
                case Gfx::R8UI:
                case Gfx::R8SI:
                case Gfx::R8UNORM:
                case Gfx::R8SNORM: return 1;
                default: ASSERT(false); return 0;
            }
        }

        bool IsDepthFormat(GfxFormat format) { return format == Gfx::D32FS8 || format == Gfx::D32F || format == Gfx::D16; }
        bool IsStencilFormat(GfxFormat format) { return format == Gfx::D32FS8; }
        bool IsSRGBFormat(GfxFormat format) { return format == Gfx::RGBA8SRGB || format == Gfx::BGRA8SRGB; }
        u32  CalcSubresource(const GfxTextureDesc& desc, u32 mip, u32 slice) { return mip + desc.mip_levels * slice; }

        void DecomposeSubresource(const GfxTextureDesc& desc, u32 subresource, u32& mip, u32& slice)
        {
            mip   = subresource % desc.mip_levels;
            slice = (subresource / desc.mip_levels) % desc.array_size;
        }

    }  // namespace ngfx
}  // namespace ncore
