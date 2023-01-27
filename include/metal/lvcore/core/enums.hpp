#ifndef LV_ENUMS_H
#define LV_ENUMS_H

#include <Metal/Metal.hpp>

#define LV_TRUE true
#define LV_FALSE false

//Shader stage
enum LvShaderStage {
    LV_SHADER_STAGE_VERTEX_BIT,
    LV_SHADER_STAGE_FRAGMENT_BIT,
    LV_SHADER_STAGE_COMPUTE_BIT
};

//Format

//R

//8
#define LV_FORMAT_R8_UINT MTL::PixelFormatR8Uint
#define LV_FORMAT_R8_SINT MTL::PixelFormatR8Sint
#define LV_FORMAT_R8_UNORM MTL::PixelFormatR8Unorm
#define LV_FORMAT_R8_SNORM MTL::PixelFormatR8Snorm
#define LV_FORMAT_R8_SRGB MTL::PixelFormatR8Unorm_sRGB
    
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
#define LV_FORMAT_RG8_UINT MTL::PixelFormatRG8Uint
#define LV_FORMAT_RG8_SINT MTL::PixelFormatRG8Sint
#define LV_FORMAT_RG8_UNORM MTL::PixelFormatRG8Unorm
#define LV_FORMAT_RG8_SNORM MTL::PixelFormatRG8Snorm
#define LV_FORMAT_RG8_SRGB MTL::PixelFormatRG8Unorm_sRGB
    
//16
#define LV_FORMAT_RG16_UINT MTL::PixelFormatRG16Uint
#define LV_FORMAT_RG16_SINT MTL::PixelFormatRG16Sint
#define LV_FORMAT_RG16_UNORM MTL::PixelFormatRG16Unorm
#define LV_FORMAT_RG16_SNORM MTL::PixelFormatRG16Snorm

//32
#define LV_FORMAT_RG32_UINT MTL::PixelFormatRG32Uint
#define LV_FORMAT_RG32_SINT MTL::PixelFormatRG32Sint

//RGBA

//8
#define LV_FORMAT_RGBA8_UINT MTL::PixelFormatRGBA8Uint
#define LV_FORMAT_RGBA8_SINT MTL::PixelFormatRGBA8Sint
#define LV_FORMAT_RGBA8_UNORM MTL::PixelFormatRGBA8Unorm
#define LV_FORMAT_RGBA8_SNORM MTL::PixelFormatRGBA8Snorm
#define LV_FORMAT_RGBA8_SRGB MTL::PixelFormatRGBA8Unorm_sRGB

//16
#define LV_FORMAT_RGBA16_UINT MTL::PixelFormatRGBA16Uint
#define LV_FORMAT_RGBA16_SINT MTL::PixelFormatRGBA16Sint
#define LV_FORMAT_RGBA16_UNORM MTL::PixelFormatRGBA16Unorm
#define LV_FORMAT_RGBA16_SNORM MTL::PixelFormatRGBA16Snorm
#define LV_FORMAT_RGBA16_SFLOAT MTL::PixelFormatRGBA16Float

//32
#define LV_FORMAT_RGBA32_UINT MTL::PixelFormatRGBA32Uint
#define LV_FORMAT_RGBA32_SINT MTL::PixelFormatRGBA32Sint
#define LV_FORMAT_RGBA32_SFLOAT MTL::PixelFormatRGBA32Float

//Cull mode
#define LV_CULL_MODE_NONE MTL::CullModeNone
#define LV_CULL_MODE_FRONT_BIT MTL::CullModeFront
#define LV_CULL_MODE_BACK_BIT MTL::CullModeBack

//Front face
#define LV_FRONT_FACE_COUNTER_CLOCKWISE MTL::WindingCounterClockwise
#define LV_FRONT_FACE_CLOCKWISE MTL::WindingClockwise

//Image usage
#define LV_IMAGE_USAGE_SAMPLED_BIT MTL::TextureUsageShaderRead
#define LV_IMAGE_USAGE_COLOR_ATTACHMENT_BIT MTL::TextureUsageRenderTarget
#define LV_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT MTL::TextureUsageRenderTarget
#define LV_IMAGE_USAGE_STORAGE_BIT MTL::TextureUsageShaderWrite

//Filter
#define LV_FILTER_NEAREST MTL::SamplerMinMagFilterNearest
#define LV_FILTER_LINEAR MTL::SamplerMinMagFilterLinear

//Image view type
#define LV_IMAGE_VIEW_TYPE_1D MTL::TextureType1D
#define LV_IMAGE_VIEW_TYPE_2D MTL::TextureType2D
#define LV_IMAGE_VIEW_TYPE_3D MTL::TextureType3D
#define LV_IMAGE_VIEW_TYPE_CUBE MTL::TextureTypeCube
#define LV_IMAGE_VIEW_TYPE_1D_ARRAY MTL::TextureType1DArray
#define LV_IMAGE_VIEW_TYPE_2D_ARRAY MTL::TextureType2DArray
#define LV_IMAGE_VIEW_TYPE_CUBE_ARRAY MTL::TextureTypeCubeArray

//Sampler address mode
#define LV_SAMPLER_ADDRESS_MODE_REPEAT MTL::SamplerAddressModeRepeat
#define LV_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT MTL::SamplerAddressModeMirrorRepeat
#define LV_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE MTL::SamplerAddressModeClampToEdge
#define LV_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER MTL::SamplerAddressModeClampToBorder
#define LV_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE MTL::SamplerAddressModeMirrorClampToEdge

//Index type
#define LV_INDEX_TYPE_UINT16 MTL::IndexTypeUInt16
#define LV_INDEX_TYPE_UINT32 MTL::IndexTypeUInt32

//Load Op
#define LV_ATTACHMENT_LOAD_OP_DONT_CARE MTL::LoadActionDontCare
#define LV_ATTACHMENT_LOAD_OP_CLEAR MTL::LoadActionClear
#define LV_ATTACHMENT_LOAD_OP_LOAD MTL::LoadActionLoad

//Store Op
#define LV_ATTACHMENT_STORE_OP_DONT_CARE MTL::StoreActionDontCare
#define LV_ATTACHMENT_STORE_OP_STORE MTL::StoreActionStore

//Compare op
#define LV_COMPARE_OP_NEVER MTL::CompareFunctionNever
#define LV_COMPARE_OP_LESS MTL::CompareFunctionLess
#define LV_COMPARE_OP_EQUAL MTL::CompareFunctionEqual
#define LV_COMPARE_OP_LESS_OR_EQUAL MTL::CompareFunctionLessEqual
#define LV_COMPARE_OP_GREATER MTL::CompareFunctionGreater
#define LV_COMPARE_OP_NOT_EQUAL MTL::CompareFunctionNotEqual
#define LV_COMPARE_OP_GREATER_OR_EQUAL MTL::CompareFunctionGreaterEqual
#define LV_COMPARE_OP_ALWAYS MTL::CompareFunctionAlways

//Vertex format
#define LV_VERTEX_FORMAT_R8_UINT MTL::VertexFormatUChar
#define LV_VERTEX_FORMAT_RG8_UINT MTL::VertexFormatUChar2
#define LV_VERTEX_FORMAT_RG32_SFLOAT MTL::VertexFormatFloat2
#define LV_VERTEX_FORMAT_RGB32_SFLOAT MTL::VertexFormatFloat3

#endif
