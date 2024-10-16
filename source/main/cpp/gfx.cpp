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
                case GfxRenderBackend::D3D12: pDevice = new D3D12Device(desc); break;
#endif
#ifdef TARGET_MAC
                case GfxRenderBackend::Metal: pDevice = new MetalDevice(desc); break;
#endif
                case GfxRenderBackend::Mock: pDevice = new MockDevice(desc); break;
                default: break;
            }

            if (pDevice && !pDevice->Create())
            {
                delete pDevice;
                pDevice = nullptr;
            }

            return pDevice;
        }

        void BeginMPGpuEvent(IGfxCommandList* pCommandList, const eastl::string& event_name)
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
                case GfxFormat::RGBA32F:
                case GfxFormat::RGBA32UI:
                case GfxFormat::RGBA32SI: return width * 16;
                case GfxFormat::RGB32F:
                case GfxFormat::RGB32UI:
                case GfxFormat::RGB32SI: return width * 12;
                case GfxFormat::RGBA16F:
                case GfxFormat::RGBA16UI:
                case GfxFormat::RGBA16SI:
                case GfxFormat::RGBA16UNORM:
                case GfxFormat::RGBA16SNORM: return width * 8;
                case GfxFormat::RGBA8UI:
                case GfxFormat::RGBA8SI:
                case GfxFormat::RGBA8UNORM:
                case GfxFormat::RGBA8SNORM:
                case GfxFormat::RGBA8SRGB:
                case GfxFormat::BGRA8UNORM:
                case GfxFormat::BGRA8SRGB:
                case GfxFormat::RGB10A2UNORM:
                case GfxFormat::R11G11B10F:
                case GfxFormat::RGB9E5: return width * 4;
                case GfxFormat::RG32F:
                case GfxFormat::RG32UI:
                case GfxFormat::RG32SI: return width * 8;
                case GfxFormat::RG16F:
                case GfxFormat::RG16UI:
                case GfxFormat::RG16SI:
                case GfxFormat::RG16UNORM:
                case GfxFormat::RG16SNORM: return width * 4;
                case GfxFormat::RG8UI:
                case GfxFormat::RG8SI:
                case GfxFormat::RG8UNORM:
                case GfxFormat::RG8SNORM: return width * 2;
                case GfxFormat::R32F:
                case GfxFormat::R32UI:
                case GfxFormat::R32SI: return width * 4;
                case GfxFormat::R16F:
                case GfxFormat::R16UI:
                case GfxFormat::R16SI:
                case GfxFormat::R16UNORM:
                case GfxFormat::R16SNORM: return width * 2;
                case GfxFormat::R8UI:
                case GfxFormat::R8SI:
                case GfxFormat::R8UNORM:
                case GfxFormat::R8SNORM: return width;
                case GfxFormat::BC1UNORM:
                case GfxFormat::BC1SRGB:
                case GfxFormat::BC4UNORM:
                case GfxFormat::BC4SNORM: return width / 2;
                case GfxFormat::BC2UNORM:
                case GfxFormat::BC2SRGB:
                case GfxFormat::BC3UNORM:
                case GfxFormat::BC3SRGB:
                case GfxFormat::BC5UNORM:
                case GfxFormat::BC5SNORM:
                case GfxFormat::BC6U16F:
                case GfxFormat::BC6S16F:
                case GfxFormat::BC7UNORM:
                case GfxFormat::BC7SRGB: return width;
                default: ASSERT(false); return 0;
            }
        }

        u32 GetFormatBlockWidth(GfxFormat format)
        {
            if (format >= GfxFormat::BC1UNORM && format <= GfxFormat::BC7SRGB)
            {
                return 4;
            }

            return 1;
        }

        u32 GetFormatBlockHeight(GfxFormat format)
        {
            if (format >= GfxFormat::BC1UNORM && format <= GfxFormat::BC7SRGB)
            {
                return 4;
            }

            return 1;
        }

        u32 GetFormatComponentNum(GfxFormat format)
        {
            switch (format)
            {
                case GfxFormat::RGBA32F:
                case GfxFormat::RGBA32UI:
                case GfxFormat::RGBA32SI:
                case GfxFormat::RGBA16F:
                case GfxFormat::RGBA16UI:
                case GfxFormat::RGBA16SI:
                case GfxFormat::RGBA16UNORM:
                case GfxFormat::RGBA16SNORM:
                case GfxFormat::RGBA8UI:
                case GfxFormat::RGBA8SI:
                case GfxFormat::RGBA8UNORM:
                case GfxFormat::RGBA8SNORM:
                case GfxFormat::RGBA8SRGB:
                case GfxFormat::BGRA8UNORM:
                case GfxFormat::BGRA8SRGB:
                case GfxFormat::RGB10A2UI:
                case GfxFormat::RGB10A2UNORM: return 4;
                case GfxFormat::RGB32F:
                case GfxFormat::RGB32UI:
                case GfxFormat::RGB32SI:
                case GfxFormat::R11G11B10F:
                case GfxFormat::RGB9E5: return 3;
                case GfxFormat::RG32F:
                case GfxFormat::RG32UI:
                case GfxFormat::RG32SI:
                case GfxFormat::RG16F:
                case GfxFormat::RG16UI:
                case GfxFormat::RG16SI:
                case GfxFormat::RG16UNORM:
                case GfxFormat::RG16SNORM:
                case GfxFormat::RG8UI:
                case GfxFormat::RG8SI:
                case GfxFormat::RG8UNORM:
                case GfxFormat::RG8SNORM: return 2;
                case GfxFormat::R32F:
                case GfxFormat::R32UI:
                case GfxFormat::R32SI:
                case GfxFormat::R16F:
                case GfxFormat::R16UI:
                case GfxFormat::R16SI:
                case GfxFormat::R16UNORM:
                case GfxFormat::R16SNORM:
                case GfxFormat::R8UI:
                case GfxFormat::R8SI:
                case GfxFormat::R8UNORM:
                case GfxFormat::R8SNORM: return 1;
                default: ASSERT(false); return 0;
            }
        }

        bool IsDepthFormat(GfxFormat format) { return format == GfxFormat::D32FS8 || format == GfxFormat::D32F || format == GfxFormat::D16; }
        bool IsStencilFormat(GfxFormat format) { return format == GfxFormat::D32FS8; }
        bool IsSRGBFormat(GfxFormat format) { return format == GfxFormat::RGBA8SRGB || format == GfxFormat::BGRA8SRGB; }
        u32  CalcSubresource(const GfxTextureDesc& desc, u32 mip, u32 slice) { return mip + desc.mip_levels * slice; }

        void DecomposeSubresource(const GfxTextureDesc& desc, u32 subresource, u32& mip, u32& slice)
        {
            mip   = subresource % desc.mip_levels;
            slice = (subresource / desc.mip_levels) % desc.array_size;
        }

    }  // namespace ngfx
}  // namespace ncore
