#ifndef LV_VULKAN_ENUMS_H
#define LV_VULKAN_ENUMS_H

#include <vma/vk_mem_alloc.h>

//Bool
#define LvBool uint8_t

#define LV_TRUE VK_TRUE
#define LV_FALSE VK_FALSE

//Shader stage
#define LvShaderStageFlags VkShaderStageFlags

#define LV_SHADER_STAGE_VERTEX_BIT VK_SHADER_STAGE_VERTEX_BIT
#define LV_SHADER_STAGE_FRAGMENT_BIT VK_SHADER_STAGE_FRAGMENT_BIT
#define LV_SHADER_STAGE_COMPUTE_BIT VK_SHADER_STAGE_COMPUTE_BIT

//Format
#define LvFormat VkFormat

//R

//8
#define LV_FORMAT_R8_UINT VK_FORMAT_R8_UINT
#define LV_FORMAT_R8_SINT VK_FORMAT_R8_SINT
#define LV_FORMAT_R8_UNORM VK_FORMAT_R8_UNORM
#define LV_FORMAT_R8_SNORM VK_FORMAT_R8_SNORM
#define LV_FORMAT_R8_UNORM_SRGB VK_FORMAT_R8_SRGB
    
//16
#define LV_FORMAT_R16_UINT VK_FORMAT_R16_UINT
#define LV_FORMAT_R16_SINT VK_FORMAT_R16_SINT
#define LV_FORMAT_R16_UNORM VK_FORMAT_R16_UNORM
#define LV_FORMAT_R16_SNORM VK_FORMAT_R16_SNORM

//32
#define LV_FORMAT_R32_UINT VK_FORMAT_R32_UINT
#define LV_FORMAT_R32_SINT VK_FORMAT_R32_SINT

//RG

//8
#define LV_FORMAT_R8G8_UINT VK_FORMAT_R8G8_UINT
#define LV_FORMAT_R8G8_SINT VK_FORMAT_R8G8_SINT
#define LV_FORMAT_R8G8_UNORM VK_FORMAT_R8G8_UNORM
#define LV_FORMAT_R8G8_SNORM VK_FORMAT_R8G8_SNORM
#define LV_FORMAT_R8G8_UNORM_SRGB VK_FORMAT_R8G8_SRGB
    
//16
#define LV_FORMAT_R16G16_UINT VK_FORMAT_R16G16_UINT
#define LV_FORMAT_R16G16_SINT VK_FORMAT_R16G16_SINT
#define LV_FORMAT_R16G16_UNORM VK_FORMAT_R16G16_UNORM
#define LV_FORMAT_R16G16_SNORM VK_FORMAT_R16G16_SNORM

//32
#define LV_FORMAT_R32G32_UINT VK_FORMAT_R32G32_UINT
#define LV_FORMAT_R32G32_SINT VK_FORMAT_R32G32_SINT

//RGB

//Packed
#define LV_FORMAT_B5G6R5_UNORM VK_FORMAT_B5G6R5_UNORM_PACK16

#define LV_FORMAT_B10R11G11_UFLOAT VK_FORMAT_B10G11R11_UFLOAT_PACK32
#define LV_FORMAT_E5R9G9B9_UFLOAT VK_FORMAT_E5B9G9R9_UFLOAT_PACK32

//RGBA

//8
#define LV_FORMAT_R8G8B8A8_UINT VK_FORMAT_R8G8B8A8_UINT
#define LV_FORMAT_R8G8B8A8_SINT VK_FORMAT_R8G8B8A8_SINT
#define LV_FORMAT_R8G8B8A8_UNORM VK_FORMAT_R8G8B8A8_UNORM
#define LV_FORMAT_R8G8B8A8_SNORM VK_FORMAT_R8G8B8A8_SNORM
#define LV_FORMAT_R8G8B8A8_UNORM_SRGB VK_FORMAT_R8G8B8A8_SRGB

//16
#define LV_FORMAT_R16G16B16A16_UINT VK_FORMAT_R16G16B16A16_UINT
#define LV_FORMAT_R16G16B16A16_SINT VK_FORMAT_R16G16B16A16_SINT
#define LV_FORMAT_R16G16B16A16_UNORM VK_FORMAT_R16G16B16A16_UNORM
#define LV_FORMAT_R16G16B16A16_SNORM VK_FORMAT_R16G16B16A16_SNORM
#define LV_FORMAT_R16G16B16A16_SFLOAT VK_FORMAT_R16G16B16A16_SFLOAT

//32
#define LV_FORMAT_R32G32B32A32_UINT VK_FORMAT_R32G32B32A32_UINT
#define LV_FORMAT_R32G32B32A32_SINT VK_FORMAT_R32G32B32A32_SINT
#define LV_FORMAT_R32G32B32A32_SFLOAT VK_FORMAT_R32G32B32A32_SFLOAT

//Packed
#define LV_FORMAT_A1B5G5R5_UNORM VK_FORMAT_A1R5G5B5_UNORM_PACK16
#define LV_FORMAT_B4G4R4A4_UNORM VK_FORMAT_B4G4R4A4_UNORM_PACK16
#define LV_FORMAT_B5G5R5A1_UNORM VK_FORMAT_B5G5R5A1_UNORM_PACK16

#define LV_FORMAT_A2B10G10R10_UNORM VK_FORMAT_A2B10G10R10_UNORM_PACK32
#define LV_FORMAT_A2R10G10B10_UNORM VK_FORMAT_A2R10G10B10_UNORM_PACK32
#define LV_FORMAT_A2R10G10B10_UINT VK_FORMAT_A2R10G10B10_UINT_PACK32

//D

//All
#define LV_FORMAT_D16_UNORM VK_FORMAT_D16_UNORM
#define LV_FORMAT_D32_SFLOAT VK_FORMAT_D32_SFLOAT

//S

//All
#define LV_FORMAT_S8_UINT VK_FORMAT_S8_UINT

//DS

//All
#define LV_FORMAT_D24_UNORM_S8_UINT VK_FORMAT_D24_UNORM_S8_UINT
#define LV_FORMAT_D32_SFLOAT_S8_UINT VK_FORMAT_D32_SFLOAT_S8_UINT

//Cull mode
#define LvCullModeFlags VkCullModeFlags

#define LV_CULL_MODE_NONE VK_CULL_MODE_NONE
#define LV_CULL_MODE_FRONT_BIT VK_CULL_MODE_FRONT_BIT
#define LV_CULL_MODE_BACK_BIT VK_CULL_MODE_BACK_BIT

//Front face
#define LvFrontFace VkFrontFace

#define LV_FRONT_FACE_COUNTER_CLOCKWISE VK_FRONT_FACE_COUNTER_CLOCKWISE
#define LV_FRONT_FACE_CLOCKWISE VK_FRONT_FACE_CLOCKWISE

//Image usage
#define LvImageUsageFlags VkImageUsageFlags

#define LV_IMAGE_USAGE_SAMPLED_BIT VK_IMAGE_USAGE_SAMPLED_BIT
#define LV_IMAGE_USAGE_COLOR_ATTACHMENT_BIT VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT
#define LV_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT
#define LV_IMAGE_USAGE_STORAGE_BIT VK_IMAGE_USAGE_STORAGE_BIT
#define LV_IMAGE_USAGE_TRANSFER_SRC_BIT VK_IMAGE_USAGE_TRANSFER_SRC_BIT
#define LV_IMAGE_USAGE_TRANSFER_DST_BIT VK_IMAGE_USAGE_TRANSFER_DST_BIT

//Filter
#define LvFilter VkFilter

#define LV_FILTER_NEAREST VK_FILTER_NEAREST
#define LV_FILTER_LINEAR VK_FILTER_LINEAR

//Image view type
#define LvImageViewType VkImageViewType

#define LV_IMAGE_VIEW_TYPE_UNDEFINED VK_IMAGE_VIEW_TYPE_MAX_ENUM
#define LV_IMAGE_VIEW_TYPE_1D VK_IMAGE_VIEW_TYPE_1D
#define LV_IMAGE_VIEW_TYPE_2D VK_IMAGE_VIEW_TYPE_2D
#define LV_IMAGE_VIEW_TYPE_3D VK_IMAGE_VIEW_TYPE_3D
#define LV_IMAGE_VIEW_TYPE_CUBE VK_IMAGE_VIEW_TYPE_CUBE
#define LV_IMAGE_VIEW_TYPE_1D_ARRAY VK_IMAGE_VIEW_TYPE_1D_ARRAY
#define LV_IMAGE_VIEW_TYPE_2D_ARRAY VK_IMAGE_VIEW_TYPE_2D_ARRAY
#define LV_IMAGE_VIEW_TYPE_CUBE_ARRAY VK_IMAGE_VIEW_TYPE_CUBE_ARRAY

//Sampler address mode
#define LvSamplerAddressMode VkSamplerAddressMode

#define LV_SAMPLER_ADDRESS_MODE_REPEAT VK_SAMPLER_ADDRESS_MODE_REPEAT
#define LV_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT
#define LV_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE
#define LV_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER
#define LV_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE

//Index type
#define LvIndexType VkIndexType

#define LV_INDEX_TYPE_UINT16 VK_INDEX_TYPE_UINT16
#define LV_INDEX_TYPE_UINT32 VK_INDEX_TYPE_UINT32

//Load Op
#define LvAttachmentLoadOp VkAttachmentLoadOp

#define LV_ATTACHMENT_LOAD_OP_DONT_CARE VK_ATTACHMENT_LOAD_OP_DONT_CARE
#define LV_ATTACHMENT_LOAD_OP_CLEAR VK_ATTACHMENT_LOAD_OP_CLEAR
#define LV_ATTACHMENT_LOAD_OP_LOAD VK_ATTACHMENT_LOAD_OP_LOAD

//Store Op
#define LvAttachmentStoreOp VkAttachmentStoreOp

#define LV_ATTACHMENT_STORE_OP_DONT_CARE VK_ATTACHMENT_STORE_OP_DONT_CARE
#define LV_ATTACHMENT_STORE_OP_STORE VK_ATTACHMENT_STORE_OP_STORE

//Compare op
#define LvCompareOp VkCompareOp

#define LV_COMPARE_OP_NEVER VK_COMPARE_OP_NEVER
#define LV_COMPARE_OP_LESS VK_COMPARE_OP_LESS
#define LV_COMPARE_OP_EQUAL VK_COMPARE_OP_EQUAL
#define LV_COMPARE_OP_LESS_OR_EQUAL VK_COMPARE_OP_LESS_OR_EQUAL
#define LV_COMPARE_OP_GREATER VK_COMPARE_OP_GREATER
#define LV_COMPARE_OP_NOT_EQUAL VK_COMPARE_OP_NOT_EQUAL
#define LV_COMPARE_OP_GREATER_OR_EQUAL VK_COMPARE_OP_GREATER_OR_EQUAL
#define LV_COMPARE_OP_ALWAYS VK_COMPARE_OP_ALWAYS

//Vertex format
#define LvVertexFormat VkFormat

#define LV_VERTEX_FORMAT_R8_UINT VK_FORMAT_R8_UINT
#define LV_VERTEX_FORMAT_RG8_UINT VK_FORMAT_R8G8_UINT
#define LV_VERTEX_FORMAT_RG32_SFLOAT VK_FORMAT_R32G32_SFLOAT
#define LV_VERTEX_FORMAT_RGB32_SFLOAT VK_FORMAT_R32G32B32_SFLOAT
#define LV_VERTEX_FORMAT_RGBA32_SFLOAT VK_FORMAT_R32G32B32A32_SFLOAT

//Image layout
#define LvImageLayout VkImageLayout

#define LV_IMAGE_LAYOUT_UNDEFINED VK_IMAGE_LAYOUT_UNDEFINED
#define LV_IMAGE_LAYOUT_GENERAL VK_IMAGE_LAYOUT_GENERAL
#define LV_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
#define LV_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
#define LV_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL
#define LV_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
#define LV_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL
#define LV_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
#define LV_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL
#define LV_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL
#define LV_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL
#define LV_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL
#define LV_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL
#define LV_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL VK_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL
#define LV_IMAGE_LAYOUT_READ_ONLY_OPTIMAL VK_IMAGE_LAYOUT_READ_ONLY_OPTIMAL
#define LV_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL

//Image aspect
#define LvImageAspectFlags VkImageAspectFlags

#define LV_IMAGE_ASPECT_COLOR_BIT VK_IMAGE_ASPECT_COLOR_BIT
#define LV_IMAGE_ASPECT_DEPTH_BIT VK_IMAGE_ASPECT_DEPTH_BIT
#define LV_IMAGE_ASPECT_STENCIL_BIT VK_IMAGE_ASPECT_STENCIL_BIT

//Memory usage
#define LvMemoryType VkMemoryPropertyFlags

#define LV_MEMORY_TYPE_PRIVATE VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
#define LV_MEMORY_TYPE_SHARED (VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
#define LV_MEMORY_TYPE_MEMORYLESS VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT

//Blend Op
#define LvBlendOp VkBlendOp

#define LV_BLEND_OP_ADD VK_BLEND_OP_ADD
#define LV_BLEND_OP_SUBTRACT VK_BLEND_OP_SUBTRACT
#define LV_BLEND_OP_REVERSE_SUBTRACT VK_BLEND_OP_REVERSE_SUBTRACT
#define LV_BLEND_OP_MIN VK_BLEND_OP_MIN
#define LV_BLEND_OP_MAX VK_BLEND_OP_MAX

//Blend factor
#define LvBlendFactor VkBlendFactor

#define LV_BLEND_FACTOR_ZERO VK_BLEND_FACTOR_ZERO
#define LV_BLEND_FACTOR_ONE VK_BLEND_FACTOR_ONE
#define LV_BLEND_FACTOR_SRC_COLOR VK_BLEND_FACTOR_SRC_COLOR
#define LV_BLEND_FACTOR_ONE_MINUS_SRC_COLOR VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR
#define LV_BLEND_FACTOR_DST_COLOR VK_BLEND_FACTOR_DST_COLOR
#define LV_BLEND_FACTOR_ONE_MINUS_DST_COLOR VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR
#define LV_BLEND_FACTOR_SRC_ALPHA VK_BLEND_FACTOR_SRC_ALPHA
#define LV_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA
#define LV_BLEND_FACTOR_DST_ALPHA VK_BLEND_FACTOR_DST_ALPHA
#define LV_BLEND_FACTOR_ONE_MINUS_DST_ALPHA VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA
#define LV_BLEND_FACTOR_CONSTANT_COLOR VK_BLEND_FACTOR_CONSTANT_COLOR
#define LV_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR
#define LV_BLEND_FACTOR_CONSTANT_ALPHA VK_BLEND_FACTOR_CONSTANT_ALPHA
#define LV_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA

//Command buffer usage
#define LvCommandBufferUsageFlags VkCommandBufferUsageFlags

#define LV_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT

//Descriptor type
#define LvDescriptorType VkDescriptorType

#define LV_DESCRIPTOR_TYPE_SAMPLER VK_DESCRIPTOR_TYPE_SAMPLER
#define LV_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER
#define LV_DESCRIPTOR_TYPE_SAMPLED_IMAGE VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE
#define LV_DESCRIPTOR_TYPE_STORAGE_IMAGE VK_DESCRIPTOR_TYPE_STORAGE_IMAGE
#define LV_DESCRIPTOR_TYPE_UNIFORM_BUFFER VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER
#define LV_DESCRIPTOR_TYPE_STORAGE_BUFFER VK_DESCRIPTOR_TYPE_STORAGE_BUFFER
#define LV_DESCRIPTOR_TYPE_INPUT_ATTACHMENT VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT

//Buffer usage
#define LvBufferUsageFlags VkBufferUsageFlags

#define LV_BUFFER_USAGE_TRANSFER_SRC_BIT VK_BUFFER_USAGE_TRANSFER_SRC_BIT
#define LV_BUFFER_USAGE_TRANSFER_DST_BIT VK_BUFFER_USAGE_TRANSFER_DST_BIT
#define LV_BUFFER_USAGE_UNIFORM_BUFFER_BIT VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT
#define LV_BUFFER_USAGE_STORAGE_BUFFER_BIT VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
#define LV_BUFFER_USAGE_INDEX_BUFFER_BIT VK_BUFFER_USAGE_INDEX_BUFFER_BIT
#define LV_BUFFER_USAGE_VERTEX_BUFFER_BIT VK_BUFFER_USAGE_VERTEX_BUFFER_BIT

//Memory allocation create
#define LvMemoryAllocationCreateFlags VmaAllocationCreateFlags

#define LV_MEMORY_ALLOCATION_CREATE_DEDICATED_BIT VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT
#define LV_MEMORY_ALLOCATION_CREATE_MIN_MEMORY_BIT VMA_ALLOCATION_CREATE_STRATEGY_MIN_MEMORY_BIT

#endif
