#ifndef __CGFX_GFX_DEFINES_H__
#define __CGFX_GFX_DEFINES_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
    #pragma once
#endif

#include "cbase/c_carray.h"

namespace ncore
{
    namespace ngfx
    {
#define D_GFX_CS_COMPONENT        static u16 const __ncs_index__
#define D_GFX_CS_COMPONENT_SET(i) static u16 const __ncs_index__ = (i)

        struct device_t;
        struct command_list_t;
        struct descriptor_t;
        struct fence_t;
        struct heap_t;
        struct heap_desc_t;
        struct pipeline_state_t;
        struct swapchain_t;
        struct swapchain_desc_t;
        struct buffer_t;
        struct buffer_desc_t;
        struct texture_t;
        struct texture_desc_t;
        struct shader_t;
        struct shader_desc_t;
        struct blas_t;
        struct blas_desc_t;
        struct tlas_t;
        struct tlas_desc_t;

        namespace constants
        {
            static const u32 CMAX_INFLIGHT_FRAMES           = 3;
            static const u32 CMAX_ROOT_CONSTANTS            = 8;
            static const u32 CMAX_CBV_BINDINGS              = 3;  // root constants in slot 0
            static const u32 CMAX_RESOURCE_DESCRIPTOR_COUNT = 65536;
            static const u32 CMAX_SAMPLER_DESCRIPTOR_COUNT  = 128;

            static const f32 CFLOAT_MAX = 3.402823466e+38F;

            static const u32 CALL_SUB_RESOURCE = 0xFFFFFFFF;
            static const u32 CINVALID_RESOURCE = 0xFFFFFFFF;

        }  // namespace constants

        namespace enums
        {
            // Example:
            // enums::backend_t b = enums::Backend_D3D12;
            // enums::backend backend = enums::cast<enums::backend>(b);
            template <typename T, typename F>
            inline static T cast(F type)
            {
                return static_cast<T>(type);
            }

            enum Component
            {
                ComponentName,
                ComponentBuffer,
                ComponentTexture,
                ComponentDevice,
                ComponentFence,
                ComponentSwapchain,
                ComponentCommandList,
                ComponentResource,
                ComponentShader,
                ComponentPipelineState,
                ComponentGraphicsPipelineState,
                ComponentMeshShadingPipelineState,
                ComponentComputePipelineState,
                ComponentDescriptor,
                ComponentDescriptorSrvTexture,
                ComponentDescriptorSrvBuffer,
                ComponentDescriptorSrvRayTracingTLAS,
                ComponentDescriptorUavTexture,
                ComponentDescriptorUavBuffer,
                ComponentDescriptorCbv,
                ComponentDescriptorSampler,
                ComponentHeap,
                ComponentBlas,
                ComponentTlas,

                ComponentBackEndBuffer,
                ComponentBackEndTexture,
                ComponentBackEndDevice,
                ComponentBackEndFence,
                ComponentBackEndSwapchain,
                ComponentBackEndCommandList,
                ComponentBackEndShader,
                ComponentBackEndGraphicsPipelineState,
                ComponentBackEndMeshShadingPipelineState,
                ComponentBackEndComputePipelineState,
                ComponentBackEndDescriptor,
                ComponentBackEndDescriptorSrvTexture,
                ComponentBackEndDescriptorSrvBuffer,
                ComponentBackEndDescriptorSrvRayTracingTLAS,
                ComponentBackEndDescriptorUavTexture,
                ComponentBackEndDescriptorUavBuffer,
                ComponentBackEndDescriptorCbv,
                ComponentBackEndDescriptorSampler,
                ComponentBackEndHeap,
                ComponentBackEndBlas,
                ComponentBackEndTlas,

                ComponentMaxCount = ComponentBackEndTlas + 1,

                ComponentMockBuffer                      = ComponentBackEndBuffer,
                ComponentMockTexture                     = ComponentBackEndTexture,
                ComponentMockDevice                      = ComponentBackEndDevice,
                ComponentMockFence                       = ComponentBackEndFence,
                ComponentMockSwapchain                   = ComponentBackEndSwapchain,
                ComponentMockCommandList                 = ComponentBackEndCommandList,
                ComponentMockShader                      = ComponentBackEndShader,
                ComponentMockGraphicsPipelineState       = ComponentBackEndGraphicsPipelineState,
                ComponentMockMeshShadingPipelineState    = ComponentBackEndMeshShadingPipelineState,
                ComponentMockComputePipelineState        = ComponentBackEndComputePipelineState,
                ComponentMockDescriptor                  = ComponentBackEndDescriptor,
                ComponentMockDescriptorSrvTexture        = ComponentBackEndDescriptorSrvTexture,
                ComponentMockDescriptorSrvBuffer         = ComponentBackEndDescriptorSrvBuffer,
                ComponentMockDescriptorSrvRayTracingTLAS = ComponentBackEndDescriptorSrvRayTracingTLAS,
                ComponentMockDescriptorUavTexture        = ComponentBackEndDescriptorUavTexture,
                ComponentMockDescriptorUavBuffer         = ComponentBackEndDescriptorUavBuffer,
                ComponentMockDescriptorCbv               = ComponentBackEndDescriptorCbv,
                ComponentMockDescriptorSampler           = ComponentBackEndDescriptorSampler,
                ComponentMockHeap                        = ComponentBackEndHeap,
                ComponentMockBlas                        = ComponentBackEndBlas,
                ComponentMockTlas                        = ComponentBackEndTlas,

                ComponentMetalBuffer                      = ComponentBackEndBuffer,
                ComponentMetalTexture                     = ComponentBackEndTexture,
                ComponentMetalDevice                      = ComponentBackEndDevice,
                ComponentMetalFence                       = ComponentBackEndFence,
                ComponentMetalSwapchain                   = ComponentBackEndSwapchain,
                ComponentMetalCommandList                 = ComponentBackEndCommandList,
                ComponentMetalShader                      = ComponentBackEndShader,
                ComponentMetalGraphicsPipelineState       = ComponentBackEndGraphicsPipelineState,
                ComponentMetalMeshShadingPipelineState    = ComponentBackEndMeshShadingPipelineState,
                ComponentMetalComputePipelineState        = ComponentBackEndComputePipelineState,
                ComponentMetalDescriptor                  = ComponentBackEndDescriptor,
                ComponentMetalDescriptorSrvTexture        = ComponentBackEndDescriptorSrvTexture,
                ComponentMetalDescriptorSrvBuffer         = ComponentBackEndDescriptorSrvBuffer,
                ComponentMetalDescriptorSrvRayTracingTLAS = ComponentBackEndDescriptorSrvRayTracingTLAS,
                ComponentMetalDescriptorUavTexture        = ComponentBackEndDescriptorUavTexture,
                ComponentMetalDescriptorUavBuffer         = ComponentBackEndDescriptorUavBuffer,
                ComponentMetalDescriptorCbv               = ComponentBackEndDescriptorCbv,
                ComponentMetalDescriptorSampler           = ComponentBackEndDescriptorSampler,
                ComponentMetalHeap                        = ComponentBackEndHeap,
                ComponentMetalBlas                        = ComponentBackEndBlas,
                ComponentMetalTlas                        = ComponentBackEndTlas,

                ComponentD3D12Buffer                      = ComponentBackEndBuffer,
                ComponentD3D12Texture                     = ComponentBackEndTexture,
                ComponentD3D12Device                      = ComponentBackEndDevice,
                ComponentD3D12Fence                       = ComponentBackEndFence,
                ComponentD3D12Swapchain                   = ComponentBackEndSwapchain,
                ComponentD3D12CommandList                 = ComponentBackEndCommandList,
                ComponentD3D12Shader                      = ComponentBackEndShader,
                ComponentD3D12GraphicsPipelineState       = ComponentBackEndGraphicsPipelineState,
                ComponentD3D12MeshShadingPipelineState    = ComponentBackEndMeshShadingPipelineState,
                ComponentD3D12ComputePipelineState        = ComponentBackEndComputePipelineState,
                ComponentD3D12Descriptor                  = ComponentBackEndDescriptor,
                ComponentD3D12DescriptorSrvTexture        = ComponentBackEndDescriptorSrvTexture,
                ComponentD3D12DescriptorSrvBuffer         = ComponentBackEndDescriptorSrvBuffer,
                ComponentD3D12DescriptorSrvRayTracingTLAS = ComponentBackEndDescriptorSrvRayTracingTLAS,
                ComponentD3D12DescriptorUavTexture        = ComponentBackEndDescriptorUavTexture,
                ComponentD3D12DescriptorUavBuffer         = ComponentBackEndDescriptorUavBuffer,
                ComponentD3D12DescriptorCbv               = ComponentBackEndDescriptorCbv,
                ComponentD3D12DescriptorSampler           = ComponentBackEndDescriptorSampler,
                ComponentD3D12Heap                        = ComponentBackEndHeap,
                ComponentD3D12Blas                        = ComponentBackEndBlas,
                ComponentD3D12Tlas                        = ComponentBackEndTlas,
            };

            enum Tag
            {
                TagNone,
                TagBuffer,
                TagTexture,
                TagHeap,
                TagDescriptorSrvTexture,
                TagDescriptorSrvBuffer,
                TagDescriptorSrvRayTracingTLAS,
                TagDescriptorUavTexture,
                TagDescriptorUavBuffer,
                TagDescriptorCbv,
                TagDescriptorSampler,
                TagPipeline,
                TagPipelineGraphics,
                TagPipelineMeshShading,
                TagPipelineCompute,
                TagShader,
                TagFence,
                TagCommandList,
                TagSwapchain,
                TagBlas,
                TagTlas,
                TagMax,
            };

            typedef u8 backend_t;
            enum backend
            {
                Backend_D3D12,
                Backend_Metal,
                Backend_Mock,
                Backend_Default = Backend_Mock,
            };

            typedef u8 format_t;
            enum format
            {
                FORMAT_UNKNOWN      = 0,
                FORMAT_RGBA32F      = 1,
                FORMAT_RGBA32UI     = 2,
                FORMAT_RGBA32SI     = 3,
                FORMAT_RGBA16F      = 4,
                FORMAT_RGBA16UI     = 5,
                FORMAT_RGBA16SI     = 6,
                FORMAT_RGBA16UNORM  = 7,
                FORMAT_RGBA16SNORM  = 8,
                FORMAT_RGBA8UI      = 9,
                FORMAT_RGBA8SI      = 10,
                FORMAT_RGBA8UNORM   = 11,
                FORMAT_RGBA8SNORM   = 12,
                FORMAT_RGBA8SRGB    = 13,
                FORMAT_BGRA8UNORM   = 14,
                FORMAT_BGRA8SRGB    = 15,
                FORMAT_RGB10A2UI    = 16,
                FORMAT_RGB10A2UNORM = 17,
                FORMAT_RGB32F       = 18,
                FORMAT_RGB32UI      = 19,
                FORMAT_RGB32SI      = 20,
                FORMAT_R11G11B10F   = 21,
                FORMAT_RGB9E5       = 22,
                FORMAT_RG32F        = 23,
                FORMAT_RG32UI       = 24,
                FORMAT_RG32SI       = 25,
                FORMAT_RG16F        = 26,
                FORMAT_RG16UI       = 27,
                FORMAT_RG16SI       = 28,
                FORMAT_RG16UNORM    = 29,
                FORMAT_RG16SNORM    = 30,
                FORMAT_RG8UI        = 31,
                FORMAT_RG8SI        = 32,
                FORMAT_RG8UNORM     = 33,
                FORMAT_RG8SNORM     = 34,
                FORMAT_R32F         = 35,
                FORMAT_R32UI        = 36,
                FORMAT_R32SI        = 37,
                FORMAT_R16F         = 38,
                FORMAT_R16UI        = 39,
                FORMAT_R16SI        = 40,
                FORMAT_R16UNORM     = 41,
                FORMAT_R16SNORM     = 42,
                FORMAT_R8UI         = 43,
                FORMAT_R8SI         = 44,
                FORMAT_R8UNORM      = 45,
                FORMAT_R8SNORM      = 46,
                FORMAT_D32F         = 47,
                FORMAT_D32FS8       = 48,
                FORMAT_D16          = 49,
                FORMAT_BC1UNORM     = 50,
                FORMAT_BC1SRGB      = 51,
                FORMAT_BC2UNORM     = 52,
                FORMAT_BC2SRGB      = 53,
                FORMAT_BC3UNORM     = 54,
                FORMAT_BC3SRGB      = 55,
                FORMAT_BC4UNORM     = 56,
                FORMAT_BC4SNORM     = 57,
                FORMAT_BC5UNORM     = 58,
                FORMAT_BC5SNORM     = 59,
                FORMAT_BC6U16F      = 60,
                FORMAT_BC6S16F      = 61,
                FORMAT_BC7UNORM     = 62,
                FORMAT_BC7SRGB      = 63,
                FORMAT_MAX          = 64,
            };
            inline format cast(format_t type) { return static_cast<format>(type); }

            u32  GetFormatRowPitch(enums::format format, u32 width);
            u32  GetFormatBlockWidth(enums::format format);
            u32  GetFormatBlockHeight(enums::format format);
            u32  GetFormatComponentNum(enums::format format);
            bool IsDepthFormat(enums::format format);
            bool IsStencilFormat(enums::format format);
            bool IsSRGBFormat(enums::format format);

            inline u32  GetFormatRowPitch(enums::format_t format, u32 width) { return GetFormatRowPitch(cast(format), width); }
            inline u32  GetFormatBlockWidth(enums::format_t format) { return GetFormatBlockWidth(cast(format)); }
            inline u32  GetFormatBlockHeight(enums::format_t format) { return GetFormatBlockHeight(cast(format)); }
            inline u32  GetFormatComponentNum(enums::format_t format) { return GetFormatComponentNum(cast(format)); }
            inline bool IsDepthFormat(enums::format_t format) { return IsDepthFormat(cast(format)); }
            inline bool IsStencilFormat(enums::format_t format) { return IsStencilFormat(cast(format)); }
            inline bool IsSRGBFormat(enums::format_t format) { return IsSRGBFormat(cast(format)); }

            typedef u8 memory_t;
            enum memory
            {
                MemoryGpuOnly,
                MemoryCpuOnly,   // staging buffers
                MemoryCpuToGpu,  // frequently updated buffers
                MemoryGpuToCpu,  // readback
            };

            typedef u8 allocation_t;
            enum allocation
            {
                AllocationCommitted,
                AllocationPlaced,
                AllocationSparse,
            };

            typedef u8 buffer_usage_t;
            enum buffer_usage
            {
                BufferUsageNone             = 0,
                BufferUsageConstantBuffer   = 1 << 0,
                BufferUsageStructuredBuffer = 1 << 1,
                BufferUsageTypedBuffer      = 1 << 2,
                BufferUsageRawBuffer        = 1 << 3,
                BufferUsageUnorderedAccess  = 1 << 4,
            };

            typedef u8 command_queue_type_t;
            enum command_queue_type
            {
                CommandQueueGraphics,
                CommandQueueCompute,
                CommandQueueCopy,
            };

            typedef u32 access_flags_t;
            enum access_flags
            {
                AccessPresent         = 1 << 0,
                AccessRTV             = 1 << 1,
                AccessDSV             = 1 << 2,
                AccessDSVReadOnly     = 1 << 3,
                AccessVertexShaderSRV = 1 << 4,
                AccessPixelShaderSRV  = 1 << 5,
                AccessComputeSRV      = 1 << 6,
                AccessVertexShaderUAV = 1 << 7,
                AccessPixelShaderUAV  = 1 << 8,
                AccessComputeUAV      = 1 << 9,
                AccessClearUAV        = 1 << 10,
                AccessCopyDst         = 1 << 11,
                AccessCopySrc         = 1 << 12,
                AccessShadingRate     = 1 << 13,
                AccessIndexBuffer     = 1 << 14,
                AccessIndirectArgs    = 1 << 15,
                AccessASRead          = 1 << 16,
                AccessASWrite         = 1 << 17,
                AccessDiscard         = 1 << 18,  // aliasing barrier

                AccessMaskVS   = AccessVertexShaderSRV | AccessVertexShaderUAV,
                AccessMaskPS   = AccessPixelShaderSRV | AccessPixelShaderUAV,
                AccessMaskCS   = AccessComputeSRV | AccessComputeUAV,
                AccessMaskSRV  = AccessVertexShaderSRV | AccessPixelShaderSRV | AccessComputeSRV,
                AccessMaskUAV  = AccessVertexShaderUAV | AccessPixelShaderUAV | AccessComputeUAV,
                AccessMaskDSV  = AccessDSV | AccessDSVReadOnly,
                AccessMaskCopy = AccessCopyDst | AccessCopySrc,
                AccessMaskAS   = AccessASRead | AccessASWrite,
            };

            typedef u8 load_op_t;
            enum load_op
            {
                LoadOpLoad,
                LoadOpClear,
                LoadOpDontCare,
            };

            typedef u8 store_op_t;
            enum store_op
            {
                StoreOpStore,
                StoreOpDontCare,
            };

            typedef u8 shader_resource_view_t;
            enum shader_resource_view
            {
                SRV_Texture2D,
                SRV_Texture2DArray,
                SRV_Texture3D,
                SRV_TextureCube,
                SRV_TextureCubeArray,
                SRV_StructuredBuffer,
                SRV_TypedBuffer,
                SRV_RawBuffer,
                SRV_RayTracingTLAS,
            };

            typedef u8 unordered_access_view_t;
            enum unordered_access_view
            {
                UAV_Texture2D,
                UAV_Texture2DArray,
                UAV_Texture3D,
                UAV_StructuredBuffer,
                UAV_TypedBuffer,
                UAV_RawBuffer,
            };

            typedef u8 descriptor_type_t;
            enum descriptor_type
            {
                DescriptorTypeSrvTexture,
                DescriptorTypeSrvBuffer,
                DescriptorTypeSrvRayTracingTLAS,
                DescriptorTypeUavTexture,
                DescriptorTypeUavBuffer,
                DescriptorTypeCbv,
                DescriptorTypeSampler,
            };

            typedef u8 filter_t;
            enum filter
            {
                FilterPoint,
                FilterLinear,
            };

            typedef u8 sampler_address_mode_t;
            enum sampler_address_mode
            {
                SamplerAddressModeRepeat,
                SamplerAddressModeMirroredRepeat,
                SamplerAddressModeClampToEdge,
                SamplerAddressModeClampToBorder,
            };

            typedef u8 sampler_reduction_mode_t;
            enum sampler_reduction_mode
            {
                SamplerReductionModeStandard,
                SamplerReductionModeCompare,
                SamplerReductionModeMin,
                SamplerReductionModeMax,
            };

            typedef u8 tile_mapping_type_t;
            enum tile_mapping_type
            {
                TileMappingMap,
                TileMappingUnmap,
            };

            typedef u8 cullmode_t;
            enum cullmode
            {
                CullNone,
                CullFront,
                CullBack,
            };

            typedef u8 comparefunc_t;
            enum comparefunc
            {
                CompareFuncNever,
                CompareFuncLess,
                CompareFuncEqual,
                CompareFuncLessEqual,
                CompareFuncGreater,
                CompareFuncNotEqual,
                CompareFuncGreaterEqual,
                CompareFuncAlways,
            };

            typedef u8 stencil_t;
            enum stencil
            {
                StencilKeep,
                StencilZero,
                StencilReplace,
                StencilIncreaseClamp,
                StencilDecreaseClamp,
                StencilInvert,
                StencilIncreaseWrap,
                StencilDecreaseWrap,
            };

            typedef u8 blendfactor_t;
            enum blendfactor
            {
                BlendFactorZero,
                BlendFactorOne,
                BlendFactorSrcColor,
                BlendFactorInvSrcColor,
                BlendFactorSrcAlpha,
                BlendFactorInvSrcAlpha,
                BlendFactorDstAlpha,
                BlendFactorInvDstAlpha,
                BlendFactorDstColor,
                BlendFactorInvDstColor,
                BlendFactorSrcAlphaClamp,
                BlendFactorConstantFactor,
                BlendFactorInvConstantFactor,
            };

            typedef u8 blendop_t;
            enum blendop
            {
                BlendOpAdd,
                BlendOpSubtract,
                BlendOpReverseSubtract,
                BlendOpMin,
                BlendOpMax,
            };

            typedef u8 colorwritemask_t;
            enum colorwritemask
            {
                ColorWriteMaskR   = 1,
                ColorWriteMaskG   = 2,
                ColorWriteMaskB   = 4,
                ColorWriteMaskA   = 8,
                ColorWriteMaskAll = (ColorWriteMaskR | ColorWriteMaskG | ColorWriteMaskB | ColorWriteMaskA),
            };

            typedef u8 primitive_t;
            enum primitive
            {
                PrimitivePointList,
                PrimitiveLineList,
                PrimitiveLineStrip,
                PrimitiveTriangleList,
                PrimitiveTriangleTrip,
            };

            typedef u8 pipeline_t;
            enum pipeline
            {
                PipelineGraphics,
                PipelineMeshShading,
                PipelineCompute,
                PipelineDefault = PipelineGraphics,
            };

            typedef u8 shadertype_t;
            enum shadertype
            {
                Shader_AS,
                Shader_MS,
                Shader_VS,
                Shader_PS,
                Shader_CS,
            };

            typedef u8 shadercompilerflag_t;
            enum shadercompilerflag
            {
                ShaderCompilerFlagO0 = 1 << 0,
                ShaderCompilerFlagO1 = 1 << 1,
                ShaderCompilerFlagO2 = 1 << 2,
                ShaderCompilerFlagO3 = 1 << 3
            };

            typedef u8 vendor_t;
            enum vendor
            {
                VendorUnkown,
                VendorAMD,
                VendorNvidia,
                VendorIntel,
                VendorApple,
                VendorMock,
            };

        }  // namespace enums

        struct device_desc_t
        {
            enums::backend backend = enums::Backend_D3D12;
        };

        struct swapchain_desc_t
        {
            void*           window_handle     = nullptr;
            u32             width             = 1;
            u32             height            = 1;
            u32             backbuffer_count  = 3;
            enums::format_t backbuffer_format = enums::FORMAT_RGBA8SRGB;
        };

        struct heap_desc_t
        {
            u32             size        = 1;
            enums::memory_t memory_type = enums::MemoryGpuOnly;
        };

        struct descriptor_desc_t
        {
            enums::descriptor_type_t type = enums::DescriptorTypeSrvTexture;
        };

        struct cbv_desc_t
        {
            u32 size   = 0;
            u32 offset = 0;
        };

        struct srv_desc_t
        {
            enums::shader_resource_view_t type   = enums::shader_resource_view::SRV_Texture2D;
            enums::format_t               format = enums::FORMAT_UNKNOWN;

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

            srv_desc_t()
                : texture()
            {
            }
        };

        inline bool operator==(const srv_desc_t& lhs, const srv_desc_t& rhs)
        {
            return lhs.type == rhs.type && lhs.texture.mip_slice == rhs.texture.mip_slice && lhs.texture.mip_levels == rhs.texture.mip_levels && lhs.texture.array_slice == rhs.texture.array_slice && lhs.texture.array_size == rhs.texture.array_size &&
                   lhs.texture.plane_slice == rhs.texture.plane_slice;
        }

        struct uav_desc_t
        {
            enums::unordered_access_view type   = enums::unordered_access_view::UAV_Texture2D;
            enums::format                format = enums::FORMAT_UNKNOWN;

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

            uav_desc_t()
                : texture()
            {
            }
        };

        inline bool operator==(const uav_desc_t& lhs, const uav_desc_t& rhs)
        {
            return lhs.type == rhs.type && lhs.texture.mip_slice == rhs.texture.mip_slice && lhs.texture.array_slice == rhs.texture.array_slice && lhs.texture.array_size == rhs.texture.array_size && lhs.texture.plane_slice == rhs.texture.plane_slice;
        }

        struct renderpass_colorattachment_t
        {
            texture_t*      texture        = nullptr;
            u32             mip_slice      = 0;
            u32             array_slice    = 0;
            enums::load_op  load_op        = enums::LoadOpLoad;
            enums::store_op store_op       = enums::StoreOpStore;
            float           clear_color[4] = {};
        };

        struct renderpass_depthattachment_t
        {
            texture_t*      texture          = nullptr;
            u32             mip_slice        = 0;
            u32             array_slice      = 0;
            enums::load_op  load_op          = enums::LoadOpLoad;
            enums::store_op store_op         = enums::StoreOpStore;
            enums::load_op  stencil_load_op  = enums::LoadOpLoad;
            enums::store_op stencil_store_op = enums::StoreOpStore;
            float           clear_depth      = 0.0f;
            u32             clear_stencil    = 0;
            bool            read_only        = false;
        };

        struct renderpass_desc_t
        {
            renderpass_colorattachment_t color[8];
            renderpass_depthattachment_t depth;
        };

        struct shader_desc_t
        {
            enums::shadertype           type;
            const char*                 file;
            const char*                 entry_point;
            const char**                defines;
            u32                         define_count;
            enums::shadercompilerflag_t flags = 0;
        };

#pragma pack(push, 1)
        struct rasterizer_state_t
        {
            enums::cullmode_t cull_mode           = enums::CullNone;
            float             depth_bias          = 0.0f;
            float             depth_bias_clamp    = 0.0f;
            float             depth_slope_scale   = 0.0f;
            bool              wireframe           = false;
            bool              front_ccw           = false;
            bool              depth_clip          = true;
            bool              line_aa             = false;
            bool              conservative_raster = false;
        };

        struct depth_stencilop_t
        {
            enums::stencil_t     stencil_fail = enums::StencilKeep;
            enums::stencil_t     depth_fail   = enums::StencilKeep;
            enums::stencil_t     pass         = enums::StencilKeep;
            enums::comparefunc_t stencil_func = enums::CompareFuncAlways;
        };

        struct depth_stencilstate_t
        {
            enums::comparefunc_t depth_func  = enums::CompareFuncAlways;
            bool                 depth_test  = false;
            bool                 depth_write = true;
            depth_stencilop_t    front;
            depth_stencilop_t    back;
            bool                 stencil_test       = false;
            u8                   stencil_read_mask  = 0xFF;
            u8                   stencil_write_mask = 0xFF;
        };

        struct blendstate_t
        {
            bool                    blend_enable = false;
            enums::blendfactor_t    color_src    = enums::BlendFactorOne;
            enums::blendfactor_t    color_dst    = enums::BlendFactorOne;
            enums::blendop_t        color_op     = enums::BlendOpAdd;
            enums::blendfactor_t    alpha_src    = enums::BlendFactorOne;
            enums::blendfactor_t    alpha_dst    = enums::BlendFactorOne;
            enums::blendop_t        alpha_op     = enums::BlendOpAdd;
            enums::colorwritemask_t write_mask   = enums::ColorWriteMaskAll;
        };

        struct graphics_pipeline_desc_t
        {
            shader_t*            vs = nullptr;
            shader_t*            ps = nullptr;
            rasterizer_state_t   rasterizer_state;
            depth_stencilstate_t depthstencil_state;
            blendstate_t         blend_state[8];
            enums::format        rt_format[8]        = {enums::FORMAT_UNKNOWN};
            enums::format        depthstencil_format = enums::FORMAT_UNKNOWN;
            enums::primitive_t   primitive_type      = enums::PrimitiveTriangleList;
        };

        struct mesh_shading_pipeline_desc_t
        {
            shader_t*            as = nullptr;
            shader_t*            ms = nullptr;
            shader_t*            ps = nullptr;
            rasterizer_state_t   rasterizer_state;
            depth_stencilstate_t depthstencil_state;
            blendstate_t         blend_state[8];
            enums::format        rt_format[8]        = {enums::FORMAT_UNKNOWN};
            enums::format        depthstencil_format = enums::FORMAT_UNKNOWN;
        };

        struct compute_pipeline_desc_t
        {
            shader_t* cs = nullptr;
        };

#pragma pack(pop)

        struct sampler_desc_t
        {
            enums::filter_t                 min_filter        = enums::FilterPoint;
            enums::filter_t                 mag_filter        = enums::FilterPoint;
            enums::filter_t                 mip_filter        = enums::FilterPoint;
            enums::sampler_reduction_mode_t reduction_mode    = enums::SamplerReductionModeStandard;
            enums::sampler_address_mode_t   address_u         = enums::SamplerAddressModeRepeat;
            enums::sampler_address_mode_t   address_v         = enums::SamplerAddressModeRepeat;
            enums::sampler_address_mode_t   address_w         = enums::SamplerAddressModeRepeat;
            enums::comparefunc              compare_func      = enums::CompareFuncAlways;
            bool                            enable_anisotropy = false;
            float                           max_anisotropy    = 1.0f;
            float                           mip_bias          = 0.0f;
            float                           min_lod           = 0.0f;
            float                           max_lod           = constants::CFLOAT_MAX;
            float                           border_color[4]   = {};
        };

        struct draw_command_t
        {
            u32 vertex_count;  // per instance
            u32 instance_count;
            u32 start_vertex;
            u32 start_instance;
        };

        struct draw_indexed_command_t
        {
            u32 index_count;  // per instance
            u32 instance_count;
            u32 start_index;
            u32 base_vertex;
            u32 start_instance;
        };

        struct dispatch_command_t
        {
            u32 group_count_x;
            u32 group_count_y;
            u32 group_count_z;
        };

        struct tile_mapping_t
        {
            enums::tile_mapping_type_t type;

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

        struct tiling_desc_t
        {
            u32 tile_count;
            u32 standard_mips;
            u32 tile_width;
            u32 tile_height;
            u32 tile_depth;
            u32 packed_mips;
            u32 packed_mip_tiles;
        };

        struct subresource_tiling_desc_t
        {
            u32 width;  // in tiles
            u32 height;
            u32 depth;
            u32 tile_offset;
        };

        namespace enums
        {
            namespace rt
            {
                typedef u8 accstruct_flag_t;
                enum accstruct_flag
                {
                    AsFlagAllowUpdate     = 1 << 0,
                    AsFlagAllowCompaction = 1 << 1,
                    AsFlagPreferFastTrace = 1 << 2,
                    AsFlagPreferFastBuild = 1 << 3,
                    AsFlagLowMemory       = 1 << 4,
                };

                typedef u8 instance_flag_t;
                enum instance_flag
                {
                    DisableCull   = 1 << 0,
                    FrontFaceCCW  = 1 << 1,
                    ForceOpaque   = 1 << 2,
                    ForceNoOpaque = 1 << 3,
                };
            }  // namespace rt
        }  // namespace enums

        // TODO We could create vertex_buffer_t and index_buffer_t as components
        struct rt_geometry_t
        {
            buffer_t*     vertex_buffer;
            u32           vertex_buffer_offset;
            u32           vertex_count;
            u32           vertex_stride;
            enums::format vertex_format;

            buffer_t*     index_buffer;
            u32           index_buffer_offset;
            u32           index_count;
            enums::format index_format;

            bool opaque;
        };

        struct rt_instance_t
        {
            blas_t*                    blas;
            f32                        transform[12];  // object to world 3x4 matrix
            u32                        instance_id;
            u8                         instance_mask;
            enums::rt::instance_flag_t instance_flags;
        };

        struct blas_desc_t
        {
            carray_t<rt_geometry_t*>    geometries;
            enums::rt::accstruct_flag_t flags;
        };

        struct tlas_desc_t
        {
            u32                         instance_count;
            enums::rt::accstruct_flag_t flags;
        };

    }  // namespace ngfx
}  // namespace ncore

#endif
