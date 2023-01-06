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

option(BACKEND_VULKAN "Build using vulkan backend" OFF)
option(BACKEND_METAL "Build using metal backend" OFF)

set(BACKEND)

if (BACKEND_VULKAN)
    message("Building using vulkan backend")

    set(BACKEND vulkan)

    #set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

    include_directories(
        "external/vma/include"
    )

    if (APPLE)
        include_directories(
            "/Users/samuliak/VulkanSDK/1.3.236.0/macOS/include"
        )
    elseif (WIN32)
        find_package(Vulkan REQUIRED)
    endif ()
elseif (BACKEND_METAL)
    message("Building using metal backend")

    set(BACKEND metal)

    include_directories(
        "external/metal-cpp/include"
    )
endif ()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib/${BACKEND})

include_directories(
    "/Users/samuliak/Documents/lvnd/include"
    "external/stb/include"
    "include/${BACKEND}"
    "include"
)

file(GLOB src
    "src/${BACKEND}/core/*.cpp"
    "src/${BACKEND}/libraries/*.cpp"
    "src/${BACKEND}/window/*.cpp"
    "src/filesystem/*.cpp"
    "src/libraries/*.cpp"
)

add_library(${PROJECT_NAME} SHARED
    ${src}
)
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION 1)

find_library(
    LVND_LIB
    NAMES lvnd liblvnd
    HINTS "/Users/samuliak/Documents/lvnd/lib/${BACKEND}"
    NO_DEFAULT_PATH
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    ${LVND_LIB}
)

if (BACKEND_VULKAN)
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
elseif (BACKEND_METAL)
    target_sources(${PROJECT_NAME} PRIVATE
        src/metal/core/dispatch_data_bridge.mm
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
