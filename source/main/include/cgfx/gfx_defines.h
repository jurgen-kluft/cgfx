#ifndef __CGFX_GFX_DEFINES_H__
#define __CGFX_GFX_DEFINES_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
    #pragma once
#endif

namespace ncore
{
    namespace ngfx
    {
        class IGfxBuffer;
        class IGfxTexture;
        class IGfxShader;
        class IGfxRayTracingBLAS;
        class IGfxHeap;

        static const u32 GFX_MAX_INFLIGHT_FRAMES           = 3;
        static const u32 GFX_MAX_ROOT_CONSTANTS            = 8;
        static const u32 GFX_MAX_CBV_BINDINGS              = 3;  // root constants in slot 0
        static const u32 GFX_MAX_RESOURCE_DESCRIPTOR_COUNT = 65536;
        static const u32 GFX_MAX_SAMPLER_DESCRIPTOR_COUNT  = 128;

        static const f32 FLT_MAX = 3.402823466e+38F;

        namespace eastl
        {
            struct string
            {
                u32         dummy;
                const char* c_str() const { return ""; }
            };

            template <typename T>
            struct span
            {
                T*       data() { return m_data; }
                const T* data() const { return m_data; }
                u32      size() const { return m_size; }

                void resize(u32 size)
                {
                    m_size = size;
                    // TODO
                    // m_data = new T[size];
                }

                T*  m_data;
                u32 m_size;
            };

            template <typename T>
            inline T max(const T& a, const T& b)
            {
                return a > b ? a : b;
            }
        }  // namespace eastl

        enum class GfxRenderBackend
        {
            D3D12,
            Metal,
            Mock,
        };

        enum class GfxFormat
        {
            Unknown,

            RGBA32F,
            RGBA32UI,
            RGBA32SI,
            RGBA16F,
            RGBA16UI,
            RGBA16SI,
            RGBA16UNORM,
            RGBA16SNORM,
            RGBA8UI,
            RGBA8SI,
            RGBA8UNORM,
            RGBA8SNORM,
            RGBA8SRGB,
            BGRA8UNORM,
            BGRA8SRGB,
            RGB10A2UI,
            RGB10A2UNORM,

            RGB32F,
            RGB32UI,
            RGB32SI,
            R11G11B10F,
            RGB9E5,

            RG32F,
            RG32UI,
            RG32SI,
            RG16F,
            RG16UI,
            RG16SI,
            RG16UNORM,
            RG16SNORM,
            RG8UI,
            RG8SI,
            RG8UNORM,
            RG8SNORM,

            R32F,
            R32UI,
            R32SI,
            R16F,
            R16UI,
            R16SI,
            R16UNORM,
            R16SNORM,
            R8UI,
            R8SI,
            R8UNORM,
            R8SNORM,

            D32F,
            D32FS8,
            D16,

            BC1UNORM,
            BC1SRGB,
            BC2UNORM,
            BC2SRGB,
            BC3UNORM,
            BC3SRGB,
            BC4UNORM,
            BC4SNORM,
            BC5UNORM,
            BC5SNORM,
            BC6U16F,
            BC6S16F,
            BC7UNORM,
            BC7SRGB,
        };

        enum class GfxMemoryType
        {
            GpuOnly,
            CpuOnly,   // staging buffers
            CpuToGpu,  // frequently updated buffers
            GpuToCpu,  // readback
        };

        enum class GfxAllocationType
        {
            Committed,
            Placed,
            Sparse,
        };

        enum GfxBufferUsageBit
        {
            GfxBufferUsageConstantBuffer   = 1 << 0,
            GfxBufferUsageStructuredBuffer = 1 << 1,
            GfxBufferUsageTypedBuffer      = 1 << 2,
            GfxBufferUsageRawBuffer        = 1 << 3,
            GfxBufferUsageUnorderedAccess  = 1 << 4,
        };
        using GfxBufferUsageFlags = u32;

        enum GfxTextureUsageBit
        {
            GfxTextureUsageRenderTarget    = 1 << 0,
            GfxTextureUsageDepthStencil    = 1 << 1,
            GfxTextureUsageUnorderedAccess = 1 << 2,
            GfxTextureUsageShared          = 1 << 3,
        };
        using GfxTextureUsageFlags = u32;

        enum class GfxTextureType
        {
            Texture2D,
            Texture2DArray,
            Texture3D,
            TextureCube,
            TextureCubeArray,
        };

        enum class GfxCommandQueue
        {
            Graphics,
            Compute,
            Copy,
        };

        enum GfxAccessBit
        {
            GfxAccessPresent         = 1 << 0,
            GfxAccessRTV             = 1 << 1,
            GfxAccessDSV             = 1 << 2,
            GfxAccessDSVReadOnly     = 1 << 3,
            GfxAccessVertexShaderSRV = 1 << 4,
            GfxAccessPixelShaderSRV  = 1 << 5,
            GfxAccessComputeSRV      = 1 << 6,
            GfxAccessVertexShaderUAV = 1 << 7,
            GfxAccessPixelShaderUAV  = 1 << 8,
            GfxAccessComputeUAV      = 1 << 9,
            GfxAccessClearUAV        = 1 << 10,
            GfxAccessCopyDst         = 1 << 11,
            GfxAccessCopySrc         = 1 << 12,
            GfxAccessShadingRate     = 1 << 13,
            GfxAccessIndexBuffer     = 1 << 14,
            GfxAccessIndirectArgs    = 1 << 15,
            GfxAccessASRead          = 1 << 16,
            GfxAccessASWrite         = 1 << 17,
            GfxAccessDiscard         = 1 << 18,  // aliasing barrier

            GfxAccessMaskVS   = GfxAccessVertexShaderSRV | GfxAccessVertexShaderUAV,
            GfxAccessMaskPS   = GfxAccessPixelShaderSRV | GfxAccessPixelShaderUAV,
            GfxAccessMaskCS   = GfxAccessComputeSRV | GfxAccessComputeUAV,
            GfxAccessMaskSRV  = GfxAccessVertexShaderSRV | GfxAccessPixelShaderSRV | GfxAccessComputeSRV,
            GfxAccessMaskUAV  = GfxAccessVertexShaderUAV | GfxAccessPixelShaderUAV | GfxAccessComputeUAV,
            GfxAccessMaskDSV  = GfxAccessDSV | GfxAccessDSVReadOnly,
            GfxAccessMaskCopy = GfxAccessCopyDst | GfxAccessCopySrc,
            GfxAccessMaskAS   = GfxAccessASRead | GfxAccessASWrite,
        };
        using GfxAccessFlags = u32;

        enum class GfxRenderPassLoadOp
        {
            Load,
            Clear,
            DontCare,
        };

        enum class GfxRenderPassStoreOp
        {
            Store,
            DontCare,
        };

        enum class GfxShaderResourceViewType
        {
            Texture2D,
            Texture2DArray,
            Texture3D,
            TextureCube,
            TextureCubeArray,
            StructuredBuffer,
            TypedBuffer,
            RawBuffer,
            RayTracingTLAS,
        };

        enum class GfxUnorderedAccessViewType
        {
            Texture2D,
            Texture2DArray,
            Texture3D,
            StructuredBuffer,
            TypedBuffer,
            RawBuffer,
        };

        static const u32 GFX_ALL_SUB_RESOURCE = 0xFFFFFFFF;
        static const u32 GFX_INVALID_RESOURCE = 0xFFFFFFFF;

        struct GfxDeviceDesc
        {
            GfxRenderBackend backend = GfxRenderBackend::D3D12;
        };

        struct GfxSwapchainDesc
        {
            void*     window_handle     = nullptr;
            u32       width             = 1;
            u32       height            = 1;
            u32       backbuffer_count  = 3;
            GfxFormat backbuffer_format = GfxFormat::RGBA8SRGB;
        };

        struct GfxHeapDesc
        {
            u32           size        = 1;
            GfxMemoryType memory_type = GfxMemoryType::GpuOnly;
        };

        struct GfxBufferDesc
        {
            u32                 stride      = 1;
            u32                 size        = 1;
            GfxFormat           format      = GfxFormat::Unknown;
            GfxMemoryType       memory_type = GfxMemoryType::GpuOnly;
            GfxAllocationType   alloc_type  = GfxAllocationType::Placed;
            GfxBufferUsageFlags usage       = 0;
            IGfxHeap*           heap        = nullptr;
            u32                 heap_offset = 0;
        };

        inline bool operator==(const GfxBufferDesc& lhs, const GfxBufferDesc& rhs)
        {
            return lhs.stride == rhs.stride && lhs.size == rhs.size && lhs.format == rhs.format && lhs.memory_type == rhs.memory_type && lhs.alloc_type == rhs.alloc_type && lhs.usage == rhs.usage;
        }

        struct GfxTextureDesc
        {
            u32                  width       = 1;
            u32                  height      = 1;
            u32                  depth       = 1;
            u32                  mip_levels  = 1;
            u32                  array_size  = 1;
            GfxTextureType       type        = GfxTextureType::Texture2D;
            GfxFormat            format      = GfxFormat::Unknown;
            GfxMemoryType        memory_type = GfxMemoryType::GpuOnly;
            GfxAllocationType    alloc_type  = GfxAllocationType::Placed;
            GfxTextureUsageFlags usage       = 0;
            IGfxHeap*            heap        = nullptr;
            u32                  heap_offset = 0;
        };

        inline bool operator==(const GfxTextureDesc& lhs, const GfxTextureDesc& rhs)
        {
            return lhs.width == rhs.width && lhs.height == rhs.height && lhs.depth == rhs.depth && lhs.mip_levels == rhs.mip_levels && lhs.array_size == rhs.array_size && lhs.type == rhs.type && lhs.format == rhs.format &&
                   lhs.memory_type == rhs.memory_type && lhs.alloc_type == rhs.alloc_type && lhs.usage == rhs.usage;
        }

        struct GfxConstantBufferViewDesc
        {
            u32 size   = 0;
            u32 offset = 0;
        };

        struct GfxShaderResourceViewDesc
        {
            GfxShaderResourceViewType type   = GfxShaderResourceViewType::Texture2D;
            GfxFormat                 format = GfxFormat::Unknown;

            union
            {
                struct
                {
                    u32 mip_slice   = 0;
                    u32 array_slice = 0;
                    u32 mip_levels  = u32(-1);
                    u32 array_size  = 1;
                    u32 plane_slice = 0;
                } texture;

                struct
                {
                    u32 size   = 0;
                    u32 offset = 0;
                } buffer;
            };

            GfxShaderResourceViewDesc()
                : texture()
            {
            }
        };

        inline bool operator==(const GfxShaderResourceViewDesc& lhs, const GfxShaderResourceViewDesc& rhs)
        {
            return lhs.type == rhs.type && lhs.texture.mip_slice == rhs.texture.mip_slice && lhs.texture.mip_levels == rhs.texture.mip_levels && lhs.texture.array_slice == rhs.texture.array_slice && lhs.texture.array_size == rhs.texture.array_size &&
                   lhs.texture.plane_slice == rhs.texture.plane_slice;
        }

        struct GfxUnorderedAccessViewDesc
        {
            GfxUnorderedAccessViewType type   = GfxUnorderedAccessViewType::Texture2D;
            GfxFormat                  format = GfxFormat::Unknown;

            union
            {
                struct
                {
                    u32 mip_slice   = 0;
                    u32 array_slice = 0;
                    u32 array_size  = 1;
                    u32 plane_slice = 0;
                } texture;

                struct
                {
                    u32 size   = 0;
                    u32 offset = 0;
                } buffer;
            };

            GfxUnorderedAccessViewDesc()
                : texture()
            {
            }
        };

        inline bool operator==(const GfxUnorderedAccessViewDesc& lhs, const GfxUnorderedAccessViewDesc& rhs)
        {
            return lhs.type == rhs.type && lhs.texture.mip_slice == rhs.texture.mip_slice && lhs.texture.array_slice == rhs.texture.array_slice && lhs.texture.array_size == rhs.texture.array_size && lhs.texture.plane_slice == rhs.texture.plane_slice;
        }

        struct GfxRenderPassColorAttachment
        {
            IGfxTexture*         texture        = nullptr;
            u32                  mip_slice      = 0;
            u32                  array_slice    = 0;
            GfxRenderPassLoadOp  load_op        = GfxRenderPassLoadOp::Load;
            GfxRenderPassStoreOp store_op       = GfxRenderPassStoreOp::Store;
            float                clear_color[4] = {};
        };

        struct GfxRenderPassDepthAttachment
        {
            IGfxTexture*         texture          = nullptr;
            u32                  mip_slice        = 0;
            u32                  array_slice      = 0;
            GfxRenderPassLoadOp  load_op          = GfxRenderPassLoadOp::Load;
            GfxRenderPassStoreOp store_op         = GfxRenderPassStoreOp::Store;
            GfxRenderPassLoadOp  stencil_load_op  = GfxRenderPassLoadOp::Load;
            GfxRenderPassStoreOp stencil_store_op = GfxRenderPassStoreOp::Store;
            float                clear_depth      = 0.0f;
            u32                  clear_stencil    = 0;
            bool                 read_only        = false;
        };

        struct GfxRenderPassDesc
        {
            GfxRenderPassColorAttachment color[8];
            GfxRenderPassDepthAttachment depth;
        };

        enum class GfxShaderType
        {
            AS,
            MS,
            VS,
            PS,
            CS,
        };

        enum GfxShaderCompilerFlagBit
        {
            GfxShaderCompilerFlagO0 = 1 << 0,
            GfxShaderCompilerFlagO1 = 1 << 1,
            GfxShaderCompilerFlagO2 = 1 << 2,
            GfxShaderCompilerFlagO3 = 1 << 3
        };
        using GfxShaderCompilerFlags = u32;

        struct GfxShaderDesc
        {
            GfxShaderType          type;
            eastl::string          file;
            eastl::string          entry_point;
            eastl::string*         defines;
            u32                    define_count;
            GfxShaderCompilerFlags flags = 0;
        };

        enum class GfxCullMode
        {
            None,
            Front,
            Back,
        };

        enum class GfxCompareFunc
        {
            Never,
            Less,
            Equal,
            LessEqual,
            Greater,
            NotEqual,
            GreaterEqual,
            Always,
        };

        enum class GfxStencilOp
        {
            Keep,
            Zero,
            Replace,
            IncreaseClamp,
            DecreaseClamp,
            Invert,
            IncreaseWrap,
            DecreaseWrap,
        };

        enum class GfxBlendFactor
        {
            Zero,
            One,
            SrcColor,
            InvSrcColor,
            SrcAlpha,
            InvSrcAlpha,
            DstAlpha,
            InvDstAlpha,
            DstColor,
            InvDstColor,
            SrcAlphaClamp,
            ConstantFactor,
            InvConstantFactor,
        };

        enum class GfxBlendOp
        {
            Add,
            Subtract,
            ReverseSubtract,
            Min,
            Max,
        };

        enum GfxColorWriteMaskBit
        {
            GfxColorWriteMaskR   = 1,
            GfxColorWriteMaskG   = 2,
            GfxColorWriteMaskB   = 4,
            GfxColorWriteMaskA   = 8,
            GfxColorWriteMaskAll = (GfxColorWriteMaskR | GfxColorWriteMaskG | GfxColorWriteMaskB | GfxColorWriteMaskA),
        };

        using GfxColorWriteMask = u8;

        enum class GfxPrimitiveType
        {
            PointList,
            LineList,
            LineStrip,
            TriangleList,
            TriangleTrip,
        };

        enum class GfxPipelineType
        {
            Graphics,
            MeshShading,
            Compute,
        };

#pragma pack(push, 1)
        struct GfxRasterizerState
        {
            GfxCullMode cull_mode           = GfxCullMode::None;
            float       depth_bias          = 0.0f;
            float       depth_bias_clamp    = 0.0f;
            float       depth_slope_scale   = 0.0f;
            bool        wireframe           = false;
            bool        front_ccw           = false;
            bool        depth_clip          = true;
            bool        line_aa             = false;
            bool        conservative_raster = false;
        };

        struct GfxDepthStencilOp
        {
            GfxStencilOp   stencil_fail = GfxStencilOp::Keep;
            GfxStencilOp   depth_fail   = GfxStencilOp::Keep;
            GfxStencilOp   pass         = GfxStencilOp::Keep;
            GfxCompareFunc stencil_func = GfxCompareFunc::Always;
        };

        struct GfxDepthStencilState
        {
            GfxCompareFunc    depth_func  = GfxCompareFunc::Always;
            bool              depth_test  = false;
            bool              depth_write = true;
            GfxDepthStencilOp front;
            GfxDepthStencilOp back;
            bool              stencil_test       = false;
            u8                stencil_read_mask  = 0xFF;
            u8                stencil_write_mask = 0xFF;
        };

        struct GfxBlendState
        {
            bool              blend_enable = false;
            GfxBlendFactor    color_src    = GfxBlendFactor::One;
            GfxBlendFactor    color_dst    = GfxBlendFactor::One;
            GfxBlendOp        color_op     = GfxBlendOp::Add;
            GfxBlendFactor    alpha_src    = GfxBlendFactor::One;
            GfxBlendFactor    alpha_dst    = GfxBlendFactor::One;
            GfxBlendOp        alpha_op     = GfxBlendOp::Add;
            GfxColorWriteMask write_mask   = GfxColorWriteMaskAll;
        };

        struct GfxGraphicsPipelineDesc
        {
            IGfxShader*          vs = nullptr;
            IGfxShader*          ps = nullptr;
            GfxRasterizerState   rasterizer_state;
            GfxDepthStencilState depthstencil_state;
            GfxBlendState        blend_state[8];
            GfxFormat            rt_format[8]        = {GfxFormat::Unknown};
            GfxFormat            depthstencil_format = GfxFormat::Unknown;
            GfxPrimitiveType     primitive_type      = GfxPrimitiveType::TriangleList;
        };

        struct GfxMeshShadingPipelineDesc
        {
            IGfxShader*          as = nullptr;
            IGfxShader*          ms = nullptr;
            IGfxShader*          ps = nullptr;
            GfxRasterizerState   rasterizer_state;
            GfxDepthStencilState depthstencil_state;
            GfxBlendState        blend_state[8];
            GfxFormat            rt_format[8]        = {GfxFormat::Unknown};
            GfxFormat            depthstencil_format = GfxFormat::Unknown;
        };

        struct GfxComputePipelineDesc
        {
            IGfxShader* cs = nullptr;
        };
#pragma pack(pop)

        enum class GfxFilter
        {
            Point,
            Linear,
        };

        enum class GfxSamplerAddressMode
        {
            Repeat,
            MirroredRepeat,
            ClampToEdge,
            ClampToBorder,
        };

        enum class GfxSamplerReductionMode
        {
            Standard,
            Compare,
            Min,
            Max,
        };

        struct GfxSamplerDesc
        {
            GfxFilter               min_filter        = GfxFilter::Point;
            GfxFilter               mag_filter        = GfxFilter::Point;
            GfxFilter               mip_filter        = GfxFilter::Point;
            GfxSamplerReductionMode reduction_mode    = GfxSamplerReductionMode::Standard;
            GfxSamplerAddressMode   address_u         = GfxSamplerAddressMode::Repeat;
            GfxSamplerAddressMode   address_v         = GfxSamplerAddressMode::Repeat;
            GfxSamplerAddressMode   address_w         = GfxSamplerAddressMode::Repeat;
            GfxCompareFunc          compare_func      = GfxCompareFunc::Always;
            bool                    enable_anisotropy = false;
            float                   max_anisotropy    = 1.0f;
            float                   mip_bias          = 0.0f;
            float                   min_lod           = 0.0f;
            float                   max_lod           = FLT_MAX;
            float                   border_color[4]   = {};
        };

        struct GfxDrawCommand
        {
            u32 vertex_count;  // per instance
            u32 instance_count;
            u32 start_vertex;
            u32 start_instance;
        };

        struct GfxDrawIndexedCommand
        {
            u32 index_count;  // per instance
            u32 instance_count;
            u32 start_index;
            u32 base_vertex;
            u32 start_instance;
        };

        struct GfxDispatchCommand
        {
            u32 group_count_x;
            u32 group_count_y;
            u32 group_count_z;
        };

        enum class GfxTileMappingType
        {
            Map,
            Unmap,
        };

        struct GfxTileMapping
        {
            GfxTileMappingType type;

            u32 subresource;
            u32 x;  // in tiles
            u32 y;
            u32 z;

            u32  tile_count;
            bool use_box;
            u32  width;  // in tiles
            u32  height;
            u32  depth;

            u32 heap_offset;  // in tiles
        };

        struct GfxTilingDesc
        {
            u32 tile_count;
            u32 standard_mips;
            u32 tile_width;
            u32 tile_height;
            u32 tile_depth;
            u32 packed_mips;
            u32 packed_mip_tiles;
        };

        struct GfxSubresourceTilingDesc
        {
            u32 width;  // in tiles
            u32 height;
            u32 depth;
            u32 tile_offset;
        };

        enum class GfxVendor
        {
            Unkown,
            AMD,
            Nvidia,
            Intel,
            Apple,
        };

        enum GfxRayTracingASFlagBit
        {
            GfxRayTracingASFlagAllowUpdate     = 1 << 0,
            GfxRayTracingASFlagAllowCompaction = 1 << 1,
            GfxRayTracingASFlagPreferFastTrace = 1 << 2,
            GfxRayTracingASFlagPreferFastBuild = 1 << 3,
            GfxRayTracingASFlagLowMemory       = 1 << 4,
        };

        using GfxRayTracingASFlag = u32;

        enum GfxRayTracingInstanceFlagBit
        {
            GfxRayTracingInstanceFlagDisableCull   = 1 << 0,
            GfxRayTracingInstanceFlagFrontFaceCCW  = 1 << 1,
            GfxRayTracingInstanceFlagForceOpaque   = 1 << 2,
            GfxRayTracingInstanceFlagForceNoOpaque = 1 << 3,
        };

        using GfxRayTracingInstanceFlag = u32;

        struct GfxRayTracingGeometry
        {
            IGfxBuffer* vertex_buffer;
            u32         vertex_buffer_offset;
            u32         vertex_count;
            u32         vertex_stride;
            GfxFormat   vertex_format;

            IGfxBuffer* index_buffer;
            u32         index_buffer_offset;
            u32         index_count;
            GfxFormat   index_format;

            bool opaque;
        };

        struct GfxRayTracingInstance
        {
            IGfxRayTracingBLAS*       blas;
            float                     transform[12];  // object to world 3x4 matrix
            u32                       instance_id;
            u8                        instance_mask;
            GfxRayTracingInstanceFlag flags;
        };

        struct GfxRayTracingBLASDesc
        {
            GfxRayTracingGeometry* geometries;
            u32                    geometries_count;
            GfxRayTracingASFlag    flags;
        };

        struct GfxRayTracingTLASDesc
        {
            u32                 instance_count;
            GfxRayTracingASFlag flags;
        };

    }  // namespace ngfx
}  // namespace ncore

#endif
