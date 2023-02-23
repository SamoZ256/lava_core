#ifndef LV_OPENGL_ENUMS_H
#define LV_OPENGL_ENUMS_H

#include <glad/glad.h>

//Bool
#define LvBool GLboolean

#define LV_TRUE GL_TRUE
#define LV_FALSE GL_FALSE

//Shader stage
#define LvShaderStageFlags GLenum

#define LV_SHADER_STAGE_VERTEX_BIT GL_VERTEX_SHADER
#define LV_SHADER_STAGE_FRAGMENT_BIT GL_FRAGMENT_SHADER
#define LV_SHADER_STAGE_COMPUTE_BIT GL_COMPUTE_SHADER

//Format
#define LvFormat GLenum

//#define LV_OPENGL_FORMAT_MAKE(internalFormat, type) (internalFormat * 1000000 + type)

//R

//8
#define LV_FORMAT_R8_UINT GL_R8UI
#define LV_FORMAT_R8_SINT GL_R8I
#define LV_FORMAT_R8_UNORM GL_R8
#define LV_FORMAT_R8_SNORM GL_R8_SNORM
#define LV_FORMAT_R8_UNORM_SRGB 0
    
//16
#define LV_FORMAT_R16_UINT GL_R16UI
#define LV_FORMAT_R16_SINT GL_R16I
#define LV_FORMAT_R16_UNORM GL_R16
#define LV_FORMAT_R16_SNORM GL_R16_SNORM

//32
#define LV_FORMAT_R32_UINT GL_R32UI
#define LV_FORMAT_R32_SINT GL_R32I

//RG

//8
#define LV_FORMAT_R8G8_UINT GL_RG8UI
#define LV_FORMAT_R8G8_SINT GL_RG8I
#define LV_FORMAT_R8G8_UNORM GL_RG8
#define LV_FORMAT_R8G8_SNORM GL_RG8_SNORM
#define LV_FORMAT_R8G8_UNORM_SRGB 0
    
//16
#define LV_FORMAT_R16G16_UINT GL_RG16UI
#define LV_FORMAT_R16G16_SINT GL_RG16I
#define LV_FORMAT_R16G16_UNORM GL_RG16
#define LV_FORMAT_R16G16_SNORM GL_RG16_SNORM

//32
#define LV_FORMAT_R32G32_UINT GL_RG32UI
#define LV_FORMAT_R32G32_SINT GL_RG32I

//RGB

//Packed
#define LV_FORMAT_B5G6R5_UNORM 0

#define LV_FORMAT_B10R11G11_UFLOAT GL_R11F_G11F_B10F
#define LV_FORMAT_E5R9G9B9_UFLOAT GL_RGB9_E5

//RGBA

//8
#define LV_FORMAT_R8G8B8A8_UINT GL_RGBA8UI
#define LV_FORMAT_R8G8B8A8_SINT GL_RGBA8I
#define LV_FORMAT_R8G8B8A8_UNORM GL_RGBA8
#define LV_FORMAT_R8G8B8A8_SNORM GL_RGBA8_SNORM
#define LV_FORMAT_R8G8B8A8_UNORM_SRGB GL_SRGB8_ALPHA8

//16
#define LV_FORMAT_R16G16B16A16_UINT GL_RGBA16UI
#define LV_FORMAT_R16G16B16A16_SINT GL_RGBA16I
#define LV_FORMAT_R16G16B16A16_UNORM GL_RGBA16
#define LV_FORMAT_R16G16B16A16_SNORM GL_RGBA16_SNORM
#define LV_FORMAT_R16G16B16A16_SFLOAT GL_RGBA16F

//32
#define LV_FORMAT_R32G32B32A32_UINT GL_RGBA32UI
#define LV_FORMAT_R32G32B32A32_SINT GL_RGBA32I
#define LV_FORMAT_R32G32B32A32_SFLOAT GL_RGBA32F

//Packed
#define LV_FORMAT_A1B5G5R5_UNORM GL_RGB5_A1
#define LV_FORMAT_B4G4R4A4_UNORM GL_RGBA4
#define LV_FORMAT_B5G5R5A1_UNORM GL_RGB5_A1

#define LV_FORMAT_A2B10G10R10_UNORM GL_RGB10_A2
#define LV_FORMAT_A2R10G10B10_UNORM GL_RGB10_A2
#define LV_FORMAT_A2R10G10B10_UINT GL_RGB10_A2UI

//D

//All
#define LV_FORMAT_D16_UNORM GL_DEPTH_COMPONENT16
#define LV_FORMAT_D32_SFLOAT GL_DEPTH_COMPONENT32F

//S

//All
#define LV_FORMAT_S8_UINT 0

//DS

//All
#define LV_FORMAT_D24_UNORM_S8_UINT GL_DEPTH_STENCIL
#define LV_FORMAT_D32_SFLOAT_S8_UINT GL_DEPTH_STENCIL

//Cull mode
#define LvCullModeFlags GLenum

#define LV_CULL_MODE_NONE 0 //glDisable(GL_CULL_FACE)
#define LV_CULL_MODE_FRONT_BIT GL_FRONT
#define LV_CULL_MODE_BACK_BIT GL_BACK

//Front face
#define LvFrontFace GLenum

#define LV_FRONT_FACE_COUNTER_CLOCKWISE GL_CCW
#define LV_FRONT_FACE_CLOCKWISE GL_CW

//Image usage
#define LvImageUsageFlags uint16_t

#define LV_IMAGE_USAGE_SAMPLED_BIT 0x1
#define LV_IMAGE_USAGE_COLOR_ATTACHMENT_BIT 0x2
#define LV_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT 0x4
#define LV_IMAGE_USAGE_STORAGE_BIT 0x8
#define LV_IMAGE_USAGE_TRANSFER_SRC_BIT 0x10
#define LV_IMAGE_USAGE_TRANSFER_DST_BIT 0x20

//Filter
#define LvFilter GLenum

#define LV_FILTER_NEAREST GL_NEAREST
#define LV_FILTER_LINEAR GL_LINEAR

//Image view type
#define LvImageViewType GLenum

#define LV_IMAGE_VIEW_TYPE_UNDEFINED 0
#define LV_IMAGE_VIEW_TYPE_1D GL_TEXTURE_1D
#define LV_IMAGE_VIEW_TYPE_2D GL_TEXTURE_2D
#define LV_IMAGE_VIEW_TYPE_3D LV_IMAGE_VIEW_TYPE_UNDEFINED
#define LV_IMAGE_VIEW_TYPE_CUBE GL_TEXTURE_CUBE_MAP
#define LV_IMAGE_VIEW_TYPE_1D_ARRAY GL_TEXTURE_1D_ARRAY
#define LV_IMAGE_VIEW_TYPE_2D_ARRAY GL_TEXTURE_2D_ARRAY
#define LV_IMAGE_VIEW_TYPE_CUBE_ARRAY LV_IMAGE_VIEW_TYPE_UNDEFINED

//Sampler address mode
#define LvSamplerAddressMode GLenum

#define LV_SAMPLER_ADDRESS_MODE_REPEAT GL_REPEAT
#define LV_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT GL_MIRRORED_REPEAT
#define LV_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE GL_CLAMP_TO_EDGE
#define LV_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER GL_CLAMP_TO_BORDER
#define LV_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE 0

//Index type
#define LvIndexType GLenum

#define LV_INDEX_TYPE_UINT16 GL_UNSIGNED_SHORT
#define LV_INDEX_TYPE_UINT32 GL_UNSIGNED_INT

//Load Op
#define LvAttachmentLoadOp uint8_t

#define LV_ATTACHMENT_LOAD_OP_DONT_CARE LV_ATTACHMENT_LOAD_OP_LOAD
#define LV_ATTACHMENT_LOAD_OP_CLEAR 2
#define LV_ATTACHMENT_LOAD_OP_LOAD 1

//Store Op
#define LvAttachmentStoreOp uint8_t

#define LV_ATTACHMENT_STORE_OP_DONT_CARE 1
#define LV_ATTACHMENT_STORE_OP_STORE 2

//Compare op
#define LvCompareOp GLenum

//GL_TEXTURE_COMPARE_MODE = GL_COMPARE_REF_TO_TEXTURE
#define LV_COMPARE_OP_NEVER GL_NEVER
#define LV_COMPARE_OP_LESS GL_LESS
#define LV_COMPARE_OP_EQUAL GL_EQUAL
#define LV_COMPARE_OP_LESS_OR_EQUAL GL_LEQUAL
#define LV_COMPARE_OP_GREATER GL_GREATER
#define LV_COMPARE_OP_NOT_EQUAL GL_NOT_EQUAL
#define LV_COMPARE_OP_GREATER_OR_EQUAL GL_GEQUAL
#define LV_COMPARE_OP_ALWAYS GL_ALWAYS

//Vertex format
#define LvVertexFormat VkFormat

#define LV_OPENGL_VERTEX_FORMAT_MAKE(numberComponents, type) (numberComponents * 1000000 + type)
#define LV_OPENGL_VERTEX_FORMAT_GET(vertexFormat, numberComponents, type) \
numberComponents = vertexFormat / 1000000; \
type = vertexFormat % 1000000;

#define LV_VERTEX_FORMAT_R8_UINT LV_OPENGL_VERTEX_FORMAT_MAKE(1, GL_UNSIGNED_BYTE)
#define LV_VERTEX_FORMAT_RG8_UINT LV_OPENGL_VERTEX_FORMAT_MAKE(2, GL_UNSIGNED_BYTE)
#define LV_VERTEX_FORMAT_RG32_SFLOAT LV_OPENGL_VERTEX_FORMAT_MAKE(2, GL_FLOAT)
#define LV_VERTEX_FORMAT_RGB32_SFLOAT LV_OPENGL_VERTEX_FORMAT_MAKE(3, GL_FLOAT)
#define LV_VERTEX_FORMAT_RGBA32_SFLOAT LV_OPENGL_VERTEX_FORMAT_MAKE(4, GL_FLOAT)

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

//Memory usage
#define LvMemoryType uint8_t

#define LV_MEMORY_TYPE_PRIVATE 1
#define LV_MEMORY_TYPE_SHARED 2
#define LV_MEMORY_TYPE_MEMORYLESS 3

//Blend Op
#define LvBlendOp GLenum

#define LV_BLEND_OP_ADD GL_FUNC_ADD
#define LV_BLEND_OP_SUBTRACT GL_FUNC_SUBTRACT
#define LV_BLEND_OP_REVERSE_SUBTRACT GL_FUNC_REVERSE_SUBTRACT
#define LV_BLEND_OP_MIN GL_MIN
#define LV_BLEND_OP_MAX GL_MAX

//Blend factor
#define LvBlendFactor GLenum

#define LV_BLEND_FACTOR_ZERO GL_ZERO
#define LV_BLEND_FACTOR_ONE GL_ONE
#define LV_BLEND_FACTOR_SRC_COLOR GL_SRC_COLOR
#define LV_BLEND_FACTOR_ONE_MINUS_SRC_COLOR GL_ONE_MINUS_SRC_COLOR
#define LV_BLEND_FACTOR_DST_COLOR GL_DST_COLOR
#define LV_BLEND_FACTOR_ONE_MINUS_DST_COLOR GL_ONE_MINUS_DST_COLOR
#define LV_BLEND_FACTOR_SRC_ALPHA GL_SRC_ALPHA
#define LV_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
#define LV_BLEND_FACTOR_DST_ALPHA GL_DST_ALPHA
#define LV_BLEND_FACTOR_ONE_MINUS_DST_ALPHA GL_ONE_MINUS_DST_ALPHA
#define LV_BLEND_FACTOR_CONSTANT_COLOR GL_CONSTANT_COLOR
#define LV_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR GL_ONE_MINUS_CONSTANT_COLOR
#define LV_BLEND_FACTOR_CONSTANT_ALPHA GL_CONSTANT_ALPHA
#define LV_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA GL_ONE_MINUS_CONSTANT_ALPHA

//Command buffer usage
#define LvCommandBufferUsageFlags uint8_t

#define LV_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT 0x1

//Descriptor type
#define LvDescriptorType uint8_t

#define LV_DESCRIPTOR_TYPE_SAMPLER 0
#define LV_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER 1
#define LV_DESCRIPTOR_TYPE_SAMPLED_IMAGE 2
#define LV_DESCRIPTOR_TYPE_STORAGE_IMAGE 3
#define LV_DESCRIPTOR_TYPE_UNIFORM_BUFFER 4
#define LV_DESCRIPTOR_TYPE_STORAGE_BUFFER 5
#define LV_DESCRIPTOR_TYPE_INPUT_ATTACHMENT 6

//Buffer usage
#define LvBufferUsageFlags uint16_t

//Gets read and interpreted as GL_STREAM_DRAW, GL_STREAM_READ, GL_STREAM_COPY, GL_STATIC_DRAW...
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