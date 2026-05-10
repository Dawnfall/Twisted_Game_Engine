cmake_minimum_required(VERSION 3.28)

# ------------------------------
# Backend selection
# ------------------------------

set(TWISTED_BACKEND "OpenGL" CACHE STRING "Rendering backend (OpenGL, Vulkan)")
set_property(CACHE TWISTED_BACKEND PROPERTY STRINGS "OpenGL" "Vulkan")
message(STATUS "[TwistedEngine] Backend: ${TWISTED_BACKEND}")

if(NOT TWISTED_BACKEND STREQUAL "OpenGL" AND NOT TWISTED_BACKEND STREQUAL "Vulkan")
    message(FATAL_ERROR "Unknown TWISTED_BACKEND '${TWISTED_BACKEND}'. Valid values: OpenGL, Vulkan")
endif()

####
# sources
###

set(RENDERER_SOURCES
    src/_compile.cpp
    src/Material.cpp
    src/RenderService.cpp
    src/Data/MeshData.cpp
)

if(TWISTED_BACKEND STREQUAL "OpenGL")
    list(APPEND RENDERER_SOURCES
        src/OpenGL/FrameBuffer_OpenGL.cpp
        src/OpenGL/Mesh_OpenGL.cpp
        src/OpenGL/RenderAPI_OpenGL.cpp
        src/OpenGL/Shader_OpenGL.cpp
        src/OpenGL/ShaderReflection_OpenGL.cpp
        src/OpenGL/Texture_OpenGL.cpp
        src/OpenGL/UniformBuffer_OpenGL.cpp
    )
elseif(TWISTED_BACKEND STREQUAL "Vulkan")
    list(APPEND RENDERER_SOURCES
        src/Vulkan/FrameBuffer_Vulkan.cpp
        src/Vulkan/Mesh_Vulkan.cpp
        src/Vulkan/RenderAPI_Vulkan.cpp
        src/Vulkan/Shader_Vulkan.cpp
        src/Vulkan/Texture_Vulkan.cpp
        src/Vulkan/VulkanContext.cpp
    )
endif()

####
# headers
####

set(RENDERER_HEADERS
    include/BufferGPU.h
    include/FrameBuffer.h
    include/LightsFunctionality.h
    include/Material.h
    include/Mesh.h
    include/Model.h
    include/RenderAPI.h
    include/RenderService.h
    include/RenderTarget.h
    include/Shader.h
    include/Texture.h
    include/UniformBuffer.h
    include/BuiltIn/BuiltInRegistry.h
    include/BuiltIn/Meshes/CubeMesh.h
    include/BuiltIn/Meshes/QuadMesh.h
    include/BuiltIn/Meshes/SphericalMesh.h
    include/BuiltIn/Meshes/TriangleMesh.h
    include/Data/ClearParams.h
    include/Data/MeshData.h
    include/Data/RenderContext.h
    include/Data/ShaderData.h
    include/Data/ShaderLayouts.h
    include/Data/TextureData.h
    include/OpenGL/FrameBuffer_OpenGL.h
    include/OpenGL/Mesh_OpenGL.h
    include/OpenGL/ShaderReflection_OpenGL.h
    include/OpenGL/Shader_OpenGL.h
    include/OpenGL/Texture_OpenGL.h
    include/OpenGL/UniformBuffer_OpenGL.h
    include/Vulkan/FrameBuffer_Vulkan.h
    include/Vulkan/Mesh_Vulkan.h
    include/Vulkan/Shader_Vulkan.h
    include/Vulkan/Texture_Vulkan.h
    include/Vulkan/VulkanContext.h
)


#source_group(
#    TREE ${CMAKE_CURRENT_SOURCE_DIR}
#    FILES ${TWISTED_ENGINE_SOURCES} ${TWISTED_ENGINE_HEADERS}
#)

# ------------------------------
# Create static library
# ------------------------------

add_library(Renderer STATIC 
    ${RENDERER_SOURCES} 
    ${RENDERER_HEADERS}
)

set_target_properties(Renderer PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    POSITION_INDEPENDENT_CODE ON
)

# ------------------------------
# Include directories
# ------------------------------

target_include_directories(Renderer PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include/"
    "${CMAKE_SOURCE_DIR}/Core/include/"
    "${CMAKE_SOURCE_DIR}/Window/include/"
    "${CMAKE_SOURCE_DIR}/ShaderParser/include"
)

# ------------------------------
# Libraries
# ------------------------------

if(TWISTED_BACKEND STREQUAL "OpenGL")
    find_package(OpenGL REQUIRED)
    find_package(glad CONFIG REQUIRED)
elseif(TWISTED_BACKEND STREQUAL "Vulkan")
    find_package(Vulkan REQUIRED)
    find_package(shaderc CONFIG REQUIRED)
endif()

if(TWISTED_BACKEND STREQUAL "OpenGL")
    target_link_libraries(Renderer PRIVATE OpenGL::GL glad::glad)
elseif(TWISTED_BACKEND STREQUAL "Vulkan")
    target_link_libraries(Renderer PRIVATE Vulkan::Vulkan shaderc_combined)
endif()

target_link_libraries(Renderer
    PUBLIC
        Core
        ShaderParser
    PRIVATE
        Window
)

# ------------------------------
# Compile definitions
# ------------------------------

#if(MSVC)
#  target_compile_definitions(TwistedEngine PRIVATE 
#    _CRT_SECURE_NO_WARNINGS)
#endif()

target_compile_definitions(Renderer PRIVATE TWISTED_ENGINE)

if(TWISTED_BACKEND STREQUAL "OpenGL")
    target_compile_definitions(Renderer PUBLIC TWISTED_BACKEND_OPENGL)
elseif(TWISTED_BACKEND STREQUAL "Vulkan")
    target_compile_definitions(Renderer PUBLIC TWISTED_BACKEND_VULKAN)
endif()

# ------------------------------
# Compiler options
# ------------------------------
option(TWISTED_WERROR "Treat warnings as errors" OFF)

if(MSVC)
  target_compile_options(Renderer PRIVATE
    /W4
    /wd4251
    /external:W0
    /experimental:external
    /analyze-
  )
  if(TWISTED_WERROR)
    target_compile_options(Renderer PRIVATE /WX)
  endif()
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
  target_compile_options(Renderer PRIVATE
    -Wall
    -Wextra
    -Wpedantic
  )
  if(TWISTED_WERROR)
    target_compile_options(Renderer PRIVATE -Werror)
  endif()
endif()
