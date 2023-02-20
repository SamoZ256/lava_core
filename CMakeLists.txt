cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME lavacore)

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Release)

add_compile_options(
    -O3
    -Wno-nullability-completeness -Wno-deprecated-volatile
    -fPIC
)

#set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION 1)

if (APPLE)
    include_directories(
        "/Users/samuliak/VulkanSDK/1.3.236.0/macOS/include"
    )
elseif (WIN32)
    find_package(Vulkan REQUIRED)
endif ()

if (APPLE)
    message("Building using metal backend")

    set(BACKEND metal)

    include_directories(
        "external/metal-cpp/include"
    )
endif ()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

include_directories(
    "/Users/samuliak/Documents/lava_window/include"
    "external/vma/include"
    "external/stb/include"
    "external/json/include"
    "include"
)

file(GLOB src
    "src/vulkan/core/*.cpp"
    "src/vulkan/libraries/*.cpp"
    "src/vulkan/window/*.cpp"
    "src/filesystem/*.cpp"
    "src/threading/*.cpp"
    "src/libraries/*.cpp"
)

add_library(${PROJECT_NAME} SHARED
    ${src}
)
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION 1)

find_library(
    LVND_LIB
    NAMES lvnd liblvnd
    HINTS "/Users/samuliak/Documents/lava_window/lib"
    NO_DEFAULT_PATH
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    ${LVND_LIB}
)

if (APPLE)
    find_library (
        Vulkan_LIBRARIES
        NAMES vulkan libvulkan # what to look for
        HINTS "/Users/samuliak/VulkanSDK/1.3.236.0/macOS/lib" # where to look
    )
endif ()

target_link_libraries(${PROJECT_NAME} PUBLIC
    ${Vulkan_LIBRARIES}
)

if (APPLE)
    file(GLOB metalSrc
        "src/metal/core/*.cpp"
        "src/metal/libraries/*.cpp"
        "src/metal/window/*.cpp"
    )

    target_sources(${PROJECT_NAME} PRIVATE
        src/metal/core/dispatch_data_bridge.mm
        ${metalSrc}
    )

    find_library(NS_FOUNDATION_LIB Foundation)
    find_library(QUARTZCORE_LIB QuartzCore)
    find_library(METAL_LIB Metal)

    target_link_libraries(${PROJECT_NAME} PRIVATE
        ${NS_FOUNDATION_LIB}
        ${QUARTZCORE_LIB}
        ${METAL_LIB}
    )
endif ()

add_compile_options(
    -ldl -lpthread -lX11
)
