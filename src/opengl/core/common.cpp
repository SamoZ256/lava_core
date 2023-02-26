#include "opengl/lvcore/core/common.hpp"

#include <map>

namespace lv {

struct OpenGL_FormatInfo {
    GLenum format;
    GLenum type;
};

std::map<LvFormat, OpenGL_FormatInfo> formatProperties = {
    {LV_FORMAT_R8_UINT, {GL_RED, GL_UNSIGNED_BYTE}},
    {LV_FORMAT_R8_SINT, {GL_RED, GL_BYTE}},
    {LV_FORMAT_R8_UNORM, {GL_RED, GL_UNSIGNED_BYTE}},
    {LV_FORMAT_R8_SNORM, {GL_RED, GL_BYTE}},
    {LV_FORMAT_R8_UNORM_SRGB, {0, 0}},

    {LV_FORMAT_R16_UINT, {GL_RED, GL_UNSIGNED_SHORT}},
    {LV_FORMAT_R16_SINT, {GL_RED, GL_SHORT}},
    {LV_FORMAT_R16_UNORM, {GL_RED, GL_UNSIGNED_SHORT}},
    {LV_FORMAT_R16_SNORM, {GL_RED, GL_SHORT}},

    {LV_FORMAT_R32_UINT, {GL_RED, GL_UNSIGNED_INT}},
    {LV_FORMAT_R32_SINT, {GL_RED, GL_INT}},

    {LV_FORMAT_R8G8_UINT, {GL_RG, GL_UNSIGNED_BYTE}},
    {LV_FORMAT_R8G8_SINT, {GL_RG, GL_BYTE}},
    {LV_FORMAT_R8G8_UNORM, {GL_RG, GL_UNSIGNED_BYTE}},
    {LV_FORMAT_R8G8_SNORM, {GL_RG, GL_BYTE}},
    {LV_FORMAT_R8G8_UNORM_SRGB, {0, 0}},

    {LV_FORMAT_R16G16_UINT, {GL_RG, GL_UNSIGNED_SHORT}},
    {LV_FORMAT_R16G16_SINT, {GL_RG, GL_SHORT}},
    {LV_FORMAT_R16G16_UNORM, {GL_RG, GL_UNSIGNED_SHORT}},
    {LV_FORMAT_R16G16_SNORM, {GL_RG, GL_SHORT}},

    {LV_FORMAT_R32G32_UINT, {GL_RG, GL_UNSIGNED_INT}},
    {LV_FORMAT_R32G32_SINT, {GL_RG, GL_INT}},

    {LV_FORMAT_B5G6R5_UNORM, {0, 0}},

    {LV_FORMAT_B10R11G11_UFLOAT, {GL_RGB, GL_FLOAT}},
    {LV_FORMAT_E5R9G9B9_UFLOAT, {GL_RGB, GL_FLOAT}},

    {LV_FORMAT_R8G8B8A8_UINT, {GL_RGBA, GL_UNSIGNED_BYTE}},
    {LV_FORMAT_R8G8B8A8_SINT, {GL_RGBA, GL_BYTE}},
    {LV_FORMAT_R8G8B8A8_UNORM, {GL_RGBA, GL_UNSIGNED_BYTE}},
    {LV_FORMAT_R8G8B8A8_SNORM, {GL_RGBA, GL_BYTE}},
    {LV_FORMAT_R8G8B8A8_UNORM_SRGB, {GL_RGBA, GL_UNSIGNED_BYTE}},

    {LV_FORMAT_R16G16B16A16_UINT, {GL_RGBA, GL_UNSIGNED_SHORT}},
    {LV_FORMAT_R16G16B16A16_SINT, {GL_RGBA, GL_SHORT}},
    {LV_FORMAT_R16G16B16A16_UNORM, {GL_RGBA, GL_UNSIGNED_SHORT}},
    {LV_FORMAT_R16G16B16A16_SNORM, {GL_RGBA, GL_SHORT}},
    {LV_FORMAT_R16G16B16A16_SFLOAT, {GL_RGBA, GL_HALF_FLOAT}},

    {LV_FORMAT_R32G32B32A32_UINT, {GL_RGBA, GL_UNSIGNED_INT}},
    {LV_FORMAT_R32G32B32A32_SINT, {GL_RGBA, GL_INT}},
    {LV_FORMAT_R32G32B32A32_SFLOAT, {GL_RGBA, GL_FLOAT}},

    {LV_FORMAT_A1B5G5R5_UNORM, {GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1}},
    {LV_FORMAT_B4G4R4A4_UNORM, {GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4}},
    {LV_FORMAT_B5G5R5A1_UNORM, {GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1}},

    {LV_FORMAT_A2B10G10R10_UNORM, {GL_RGBA, GL_UNSIGNED_INT_2_10_10_10_REV}},
    {LV_FORMAT_A2R10G10B10_UNORM, {GL_RGBA, GL_UNSIGNED_INT_2_10_10_10_REV}},
    {LV_FORMAT_A2R10G10B10_UINT, {GL_RGBA, GL_UNSIGNED_INT_2_10_10_10_REV}},

    {LV_FORMAT_D16_UNORM, {GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT}},
    {LV_FORMAT_D32_SFLOAT, {GL_DEPTH_COMPONENT, GL_FLOAT}},

    {LV_FORMAT_S8_UINT, {0, 0}},

    {LV_FORMAT_D24_UNORM_S8_UINT, {GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8}},
    {LV_FORMAT_D32_SFLOAT_S8_UINT, {GL_DEPTH_STENCIL, GL_FLOAT_32_UNSIGNED_INT_24_8_REV}}
};

void getOpenGLInternalFormatProperties(LvFormat internalFormat, GLenum* format, GLenum* type) {
    OpenGL_FormatInfo& formatInfo = formatProperties[internalFormat];
    *format = formatInfo.format;
    *type = formatInfo.type;
}

} //namespace lv