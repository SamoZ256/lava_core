#ifndef LV_METAL_ENUMS_H
#define LV_METAL_ENUMS_H

#include <Metal/Metal.hpp>

//Bool
#define LvBool bool

#define LV_TRUE true
#define LV_FALSE false

//Shader stage
#define LvShaderStageFlags uint8_t

#define LV_SHADER_STAGE_VERTEX_BIT 0x1
#define LV_SHADER_STAGE_FRAGMENT_BIT 0x2
#define LV_SHADER_STAGE_COMPUTE_BIT 0x4

#define LV_SHADER_STAGE_VERTEX_INDEX 0
#define LV_SHADER_STAGE_FRAGMENT_INDEX 1
#define LV_SHADER_STAGE_COMPUTE_INDEX 2

//Format
#define LvFormat MTL::PixelFormat

//R

//8
#define LV_FORMAT_R8_UINT MTL::PixelFormatR8Uint
#define LV_FORMAT_R8_SINT MTL::PixelFormatR8Sint
#define LV_FORMAT_R8_UNORM MTL::PixelFormatR8Unorm
#define LV_FORMAT_R8_SNORM MTL::PixelFormatR8Snorm
#define LV_FORMAT_R8_UNORM_SRGB MTL::PixelFormatR8Unorm_sRGB
    
//16
#define LV_FORMAT_R16_UINT MTL::PixelFormatR16Uint
#define LV_FORMAT_R16_SINT MTL::PixelFormatR16Sint
#define LV_FORMAT_R16_UNORM MTL::PixelFormatR16Unorm
#define LV_FORMAT_R16_SNORM MTL::PixelFormatR16Snorm

//32
#define LV_FORMAT_R32_UINT MTL::PixelFormatR32Uint
#define LV_FORMAT_R32_SINT MTL::PixelFormatR32Sint

//RG

//8
#define LV_FORMAT_R8G8_UINT MTL::PixelFormatRG8Uint
#define LV_FORMAT_R8G8_SINT MTL::PixelFormatRG8Sint
#define LV_FORMAT_R8G8_UNORM MTL::PixelFormatRG8Unorm
#define LV_FORMAT_R8G8_SNORM MTL::PixelFormatRG8Snorm
#define LV_FORMAT_R8G8_UNORM_SRGB MTL::PixelFormatRG8Unorm_sRGB
    
//16
#define LV_FORMAT_R16G16_UINT MTL::PixelFormatRG16Uint
#define LV_FORMAT_R16G16_SINT MTL::PixelFormatRG16Sint
#define LV_FORMAT_R16G16_UNORM MTL::PixelFormatRG16Unorm
#define LV_FORMAT_R16G16_SNORM MTL::PixelFormatRG16Snorm

//32
#define LV_FORMAT_R32G32_UINT MTL::PixelFormatRG32Uint
#define LV_FORMAT_R32G32_SINT MTL::PixelFormatRG32Sint

//RGB

//Packed
#define LV_FORMAT_B5G6R5_UNORM MTL::PixelFormatB5G6R5Unorm

#define LV_FORMAT_B10R11G11_UFLOAT MTL::PixelFormatRG11B10Float
#define LV_FORMAT_E5R9G9B9_UFLOAT MTL::PixelFormatRGB9E5Float

//RGBA

//8
#define LV_FORMAT_R8G8B8A8_UINT MTL::PixelFormatRGBA8Uint
#define LV_FORMAT_R8G8B8A8_SINT MTL::PixelFormatRGBA8Sint
#define LV_FORMAT_R8G8B8A8_UNORM MTL::PixelFormatRGBA8Unorm
#define LV_FORMAT_R8G8B8A8_SNORM MTL::PixelFormatRGBA8Snorm
#define LV_FORMAT_R8G8B8A8_UNORM_SRGB MTL::PixelFormatRGBA8Unorm_sRGB

//16
#define LV_FORMAT_R16G16B16A16_UINT MTL::PixelFormatRGBA16Uint
#define LV_FORMAT_R16G16B16A16_SINT MTL::PixelFormatRGBA16Sint
#define LV_FORMAT_R16G16B16A16_UNORM MTL::PixelFormatRGBA16Unorm
#define LV_FORMAT_R16G16B16A16_SNORM MTL::PixelFormatRGBA16Snorm
#define LV_FORMAT_R16G16B16A16_SFLOAT MTL::PixelFormatRGBA16Float

//32
#define LV_FORMAT_R32G32B32A32_UINT MTL::PixelFormatRGBA32Uint
#define LV_FORMAT_R32G32B32A32_SINT MTL::PixelFormatRGBA32Sint
#define LV_FORMAT_R32G32B32A32_SFLOAT MTL::PixelFormatRGBA32Float

//Packed
#define LV_FORMAT_A1B5G5R5_UNORM MTL::PixelFormatA1BGR5Unorm
#define LV_FORMAT_B4G4R4A4_UNORM MTL::PixelFormatABGR4Unorm
#define LV_FORMAT_B5G5R5A1_UNORM MTL::PixelFormatBGR5A1Unorm

#define LV_FORMAT_A2B10G10R10_UNORM MTL::PixelFormatBGR10A2Unorm
#define LV_FORMAT_A2R10G10B10_UNORM MTL::PixelFormatRGB10A2Unorm
#define LV_FORMAT_A2R10G10B10_UINT MTL::PixelFormatRGB10A2Uint

//D

//All
#define LV_FORMAT_D16_UNORM MTL::PixelFormatDepth16Unorm
#define LV_FORMAT_D32_SFLOAT MTL::PixelFormatDepth32Float

//S

//All
#define LV_FORMAT_S8_UINT MTL::PixelFormatStencil8

//DS

//All
#define LV_FORMAT_D24_UNORM_S8_UINT MTL::PixelFormatDepth24Unorm_Stencil8
#define LV_FORMAT_D32_SFLOAT_S8_UINT MTL::PixelFormatDepth32Float_Stencil8

//Cull mode
#define LvCullModeFlags MTL::CullMode

#define LV_CULL_MODE_NONE MTL::CullModeNone
#define LV_CULL_MODE_FRONT_BIT MTL::CullModeFront
#define LV_CULL_MODE_BACK_BIT MTL::CullModeBack

//Front face
#define LvFrontFace MTL::Winding

#define LV_FRONT_FACE_COUNTER_CLOCKWISE MTL::WindingCounterClockwise
#define LV_FRONT_FACE_CLOCKWISE MTL::WindingClockwise

//Image usage
#define LvImageUsageFlags MTL::TextureUsage

#define LV_IMAGE_USAGE_SAMPLED_BIT MTL::TextureUsageShaderRead
#define LV_IMAGE_USAGE_COLOR_ATTACHMENT_BIT MTL::TextureUsageRenderTarget
#define LV_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT MTL::TextureUsageRenderTarget
#define LV_IMAGE_USAGE_STORAGE_BIT MTL::TextureUsageShaderWrite
#define LV_IMAGE_USAGE_TRANSFER_SRC_BIT MTL::TextureUsageUnknown
#define LV_IMAGE_USAGE_TRANSFER_DST_BIT MTL::TextureUsageUnknown

//Filter
#define LvFilter MTL::SamplerMinMagFilter

#define LV_FILTER_NEAREST MTL::SamplerMinMagFilterNearest
#define LV_FILTER_LINEAR MTL::SamplerMinMagFilterLinear

//Image view type
#define LvImageViewType MTL::TextureType

#define LV_IMAGE_VIEW_TYPE_UNDEFINED MTL::TextureType(10)
#define LV_IMAGE_VIEW_TYPE_1D MTL::TextureType1D
#define LV_IMAGE_VIEW_TYPE_2D MTL::TextureType2D
#define LV_IMAGE_VIEW_TYPE_3D MTL::TextureType3D
#define LV_IMAGE_VIEW_TYPE_CUBE MTL::TextureTypeCube
#define LV_IMAGE_VIEW_TYPE_1D_ARRAY MTL::TextureType1DArray
#define LV_IMAGE_VIEW_TYPE_2D_ARRAY MTL::TextureType2DArray
#define LV_IMAGE_VIEW_TYPE_CUBE_ARRAY MTL::TextureTypeCubeArray

//Sampler address mode
#define LvSamplerAddressMode MTL::SamplerAddressMode

#define LV_SAMPLER_ADDRESS_MODE_REPEAT MTL::SamplerAddressModeRepeat
#define LV_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT MTL::SamplerAddressModeMirrorRepeat
#define LV_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE MTL::SamplerAddressModeClampToEdge
#define LV_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER MTL::SamplerAddressModeClampToBorder
#define LV_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE MTL::SamplerAddressModeMirrorClampToEdge

//Index type
#define LvIndexType MTL::IndexType

#define LV_INDEX_TYPE_UINT16 MTL::IndexTypeUInt16
#define LV_INDEX_TYPE_UINT32 MTL::IndexTypeUInt32

//Load Op
#define LvAttachmentLoadOp MTL::LoadAction

#define LV_ATTACHMENT_LOAD_OP_DONT_CARE MTL::LoadActionDontCare
#define LV_ATTACHMENT_LOAD_OP_CLEAR MTL::LoadActionClear
#define LV_ATTACHMENT_LOAD_OP_LOAD MTL::LoadActionLoad

//Store Op
#define LvAttachmentStoreOp MTL::StoreAction

#define LV_ATTACHMENT_STORE_OP_DONT_CARE MTL::StoreActionDontCare
#define LV_ATTACHMENT_STORE_OP_STORE MTL::StoreActionStore

//Compare op
#define LvCompareOp MTL::CompareFunction

#define LV_COMPARE_OP_NEVER MTL::CompareFunctionNever
#define LV_COMPARE_OP_LESS MTL::CompareFunctionLess
#define LV_COMPARE_OP_EQUAL MTL::CompareFunctionEqual
#define LV_COMPARE_OP_LESS_OR_EQUAL MTL::CompareFunctionLessEqual
#define LV_COMPARE_OP_GREATER MTL::CompareFunctionGreater
#define LV_COMPARE_OP_NOT_EQUAL MTL::CompareFunctionNotEqual
#define LV_COMPARE_OP_GREATER_OR_EQUAL MTL::CompareFunctionGreaterEqual
#define LV_COMPARE_OP_ALWAYS MTL::CompareFunctionAlways

//Vertex format
#define LvVertexFormat MTL::VertexFormat

#define LV_VERTEX_FORMAT_R8_UINT MTL::VertexFormatUChar
#define LV_VERTEX_FORMAT_RG8_UINT MTL::VertexFormatUChar2
#define LV_VERTEX_FORMAT_RG32_SFLOAT MTL::VertexFormatFloat2
#define LV_VERTEX_FORMAT_RGB32_SFLOAT MTL::VertexFormatFloat3
#define LV_VERTEX_FORMAT_RGBA32_SFLOAT MTL::VertexFormatFloat4

//Image layout
#define LvImageLayout uint8_t

#define LV_IMAGE_LAYOUT_UNDEFINED 0
#define LV_IMAGE_LAYOUT_GENERAL 1
#define LV_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL 2
#define LV_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL 3
#define LV_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL 4
#define LV_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL 5
#define LV_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL 6
#define LV_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL 7
#define LV_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL 8
#define LV_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL 9
#define LV_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL 10
#define LV_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL 11
#define LV_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL 12
#define LV_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL 13
#define LV_IMAGE_LAYOUT_READ_ONLY_OPTIMAL 14
#define LV_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL 15

//Image aspect
#define LvImageAspectFlags uint8_t

#define LV_IMAGE_ASPECT_COLOR_BIT 0x1
#define LV_IMAGE_ASPECT_DEPTH_BIT 0x2
#define LV_IMAGE_ASPECT_STENCIL_BIT 0x4

//Memory type
#define LvMemoryType MTL::StorageMode

#define LV_MEMORY_TYPE_PRIVATE MTL::StorageModePrivate
#define LV_MEMORY_TYPE_SHARED MTL::StorageModeShared
#define LV_MEMORY_TYPE_MEMORYLESS MTL::StorageModeMemoryless

//Blend Op
#define LvBlendOp MTL::BlendOperation

#define LV_BLEND_OP_ADD MTL::BlendOperationAdd
#define LV_BLEND_OP_SUBTRACT MTL::BlendOperationSubtract
#define LV_BLEND_OP_REVERSE_SUBTRACT MTL::BlendOperationReverseSubtract
#define LV_BLEND_OP_MIN MTL::BlendOperationMin
#define LV_BLEND_OP_MAX MTL::BlendOperationMax

//Blend factor
#define LvBlendFactor MTL::BlendFactor

#define LV_BLEND_FACTOR_ZERO MTL::BlendFactorZero
#define LV_BLEND_FACTOR_ONE MTL::BlendFactorOne
#define LV_BLEND_FACTOR_SRC_COLOR MTL::BlendFactorZero //TODO: finish
#define LV_BLEND_FACTOR_ONE_MINUS_SRC_COLOR MTL::BlendFactorZero
#define LV_BLEND_FACTOR_DST_COLOR MTL::BlendFactorZero
#define LV_BLEND_FACTOR_ONE_MINUS_DST_COLOR MTL::BlendFactorZero
#define LV_BLEND_FACTOR_SRC_ALPHA MTL::BlendFactorZero
#define LV_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA MTL::BlendFactorZero
#define LV_BLEND_FACTOR_DST_ALPHA MTL::BlendFactorZero
#define LV_BLEND_FACTOR_ONE_MINUS_DST_ALPHA MTL::BlendFactorZero
#define LV_BLEND_FACTOR_CONSTANT_COLOR MTL::BlendFactorZero
#define LV_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR MTL::BlendFactorZero
#define LV_BLEND_FACTOR_CONSTANT_ALPHA MTL::BlendFactorZero
#define LV_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA MTL::BlendFactorZero

//Command buffer usage
#define LvCommandBufferUsageFlags uint8_t

#define LV_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT 0x1

//Descriptor type
#define LvDescriptorType uint8_t

#define LV_DESCRIPTOR_TYPE_UNDEFINED 0
#define LV_DESCRIPTOR_TYPE_SAMPLER 1
#define LV_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER 2
#define LV_DESCRIPTOR_TYPE_SAMPLED_IMAGE 3
#define LV_DESCRIPTOR_TYPE_STORAGE_IMAGE 4
#define LV_DESCRIPTOR_TYPE_UNIFORM_BUFFER 5
#define LV_DESCRIPTOR_TYPE_STORAGE_BUFFER 6
#define LV_DESCRIPTOR_TYPE_INPUT_ATTACHMENT 7

//Buffer usage
#define LvBufferUsageFlags uint16_t

#define LV_BUFFER_USAGE_TRANSFER_SRC_BIT 0x1
#define LV_BUFFER_USAGE_TRANSFER_DST_BIT 0x2
#define LV_BUFFER_USAGE_UNIFORM_BUFFER_BIT 0x4
#define LV_BUFFER_USAGE_STORAGE_BUFFER_BIT 0x8
#define LV_BUFFER_USAGE_INDEX_BUFFER_BIT 0x10
#define LV_BUFFER_USAGE_VERTEX_BUFFER_BIT 0x20

//Memory allocation create
#define LvMemoryAllocationCreateFlags uint8_t

#define LV_MEMORY_ALLOCATION_CREATE_DEDICATED_BIT 0x1
#define LV_MEMORY_ALLOCATION_CREATE_MIN_MEMORY_BIT 0x2

#endif
