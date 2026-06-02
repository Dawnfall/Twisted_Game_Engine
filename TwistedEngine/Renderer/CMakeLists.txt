set(RENDERER_SOURCES
    src/Material.cpp
    src/RenderService.cpp
    src/Data/MeshData.cpp
    src/FrameBuffer.cpp
    src/Mesh.cpp
    src/RenderAPI.cpp
    src/Shader.cpp
    src/Texture.cpp
    src/VulkanContext.cpp
)

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
        include/VulkanContext.h
)

source_group(
    TREE ${CMAKE_CURRENT_SOURCE_DIR}
    FILES ${RENDERER_SOURCES} ${RENDERER_HEADERS}
)

add_library(Renderer STATIC
    ${RENDERER_SOURCES}
    ${RENDERER_HEADERS}
)

set_target_properties(Renderer PROPERTIES
    FOLDER "Engine"
)

target_include_directories(Renderer PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include/"
)

find_package(Vulkan REQUIRED)
find_package(unofficial-shaderc CONFIG REQUIRED)
target_link_libraries(Renderer PUBLIC Vulkan::Vulkan)
target_link_libraries(Renderer PRIVATE unofficial::shaderc::shaderc)

target_link_libraries(Renderer
    PUBLIC
        Core
        ShaderParser
    PRIVATE
        Window
)

target_compile_definitions(Renderer PRIVATE TWISTED_ENGINE)

target_compile_options(Renderer PRIVATE
    -Wall
    -Wextra
    -Wno-c++98-compat
    -Wno-c++98-compat-pedantic
    -Wno-pre-c++14-compat
    -Wno-pre-c++17-compat
    -Wno-unused-macros
    -Wno-covered-switch-default
    -Wno-float-equal
    -Wno-switch-enum
    -Wno-unsafe-buffer-usage
    -Wno-old-style-cast
    -Wno-sign-conversion
    -Wno-implicit-int-float-conversion
    -Wno-shadow-field-in-constructor
    -Wno-missing-field-initializers
    -Wno-cast-function-type-strict
    -Wno-nonportable-system-include-path
    -Wno-switch-default
    -Wno-double-promotion
    -Wno-deprecated-declarations
    -Wno-missing-braces
    -Wno-global-constructors
    -Wno-exit-time-destructors
    -Wno-zero-as-null-pointer-constant
)

if(TWISTED_WERROR)
    target_compile_options(Renderer PRIVATE -Werror)
endif()
