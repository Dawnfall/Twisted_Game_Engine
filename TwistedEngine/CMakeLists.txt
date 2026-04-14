cmake_minimum_required(VERSION 3.28)

# ------------------------------
# Source files
# ------------------------------

set(TWISTED_ENGINE_SOURCES
    src/Debug/Logger.cpp
    src/Serialization/BinSerializer.cpp
    src/Twisted_compile.cpp
    src/Utils/FileUtils.cpp
    src/Utils/GlmUtils.cpp
    src/Utils/MeshGenerator.cpp
    src/Utils/Utils.cpp
    src/Utils/YamlUtils.cpp
    src/Twisted/TObject.cpp
    src/Twisted/Application/Application.cpp
    src/Twisted/AssetsLayer/AssetImporterRegistry.cpp
    src/Twisted/AssetsLayer/AssetInfo.cpp
    src/Twisted/AssetsLayer/AssetUuid.cpp
    src/Twisted/AssetsLayer/AssetsService.cpp
    src/Twisted/AssetsLayer/Project.cpp
    src/Twisted/AssetsLayer/ProjectConfig.cpp
    src/Twisted/AssetsLayer/Importers/MaterialImpoter.cpp
    src/Twisted/AssetsLayer/Importers/ModelImporter.cpp
    src/Twisted/AssetsLayer/Importers/ShaderImporter.cpp
    src/Twisted/AssetsLayer/Importers/TextureImporter.cpp
    src/Twisted/AssetsLayer/Importers/WorldImporter.cpp
    src/Twisted/Gameing/Entity.cpp
    src/Twisted/Gameing/GameService.cpp
    src/Twisted/Gameing/World.cpp
    src/Twisted/Gameing/WorldRegistry.cpp
    src/Twisted/Gameing/Components/CTransform.cpp
    src/Twisted/Gameing/Managers/CameraManager.cpp
    src/Twisted/Rendering/_compile.cpp
    src/Twisted/Rendering/Material.cpp
    src/Twisted/Rendering/RenderConvert.cpp
    src/Twisted/Rendering/RenderService.cpp
    src/Twisted/Rendering/Data/MeshData.cpp
    src/Twisted/Rendering/OpenGL/FrameBuffer_OpenGL.cpp
    src/Twisted/Rendering/OpenGL/Mesh_OpenGL.cpp
    src/Twisted/Rendering/OpenGL/RenderAPI_OpenGL.cpp
    src/Twisted/Rendering/OpenGL/Shader_OpenGL.cpp
    src/Twisted/Rendering/OpenGL/ShaderReflection_OpenGL.cpp
    src/Twisted/Rendering/OpenGL/Texture_OpenGL.cpp
    src/Twisted/Rendering/OpenGL/UniformBuffer_OpenGL.cpp
    src/Twisted/Windowing/Input.cpp
    src/Twisted/Windowing/WindowsService.cpp
)

if(WIN32)
    list(APPEND TWISTED_ENGINE_SOURCES
        src/Twisted/Windowing/Win32/Win32Utils.cpp
        src/Twisted/Windowing/Win32/MonitorInfo_win32.cpp
        src/Twisted/Windowing/Win32/Window_win32.cpp
    )
else()
    list(APPEND TWISTED_ENGINE_SOURCES
        src/Twisted/Windowing/Linux/NativeUtils_Linux.cpp
        src/Twisted/Windowing/GLFW/Window_glfw.cpp
    )
endif()

# ------------------------------
# Header files
# ------------------------------

set(TWISTED_ENGINE_HEADERS
    include/AppCore.h
    include/Twisted.h
    include/Data/Color.h
    include/Data/Vertex.h
    include/Debug/Logger.h
    include/Serialization/BinSerializer.h
    include/Serialization/SerializedBuffer.h
    include/Utils/AlgebraUtils.h
    include/Utils/Event.h
    include/Utils/FileUtils.h
    include/Utils/GlmUtils.h
    include/Utils/MeshGenerator.h
    include/Utils/Utils.h
    include/Utils/WPtr.h
    include/Utils/WPtrBase.h
    include/Utils/YamlUtils.h
    include/Twisted/Constants.h
    include/Twisted/ObjectID.h
    include/Twisted/TObject.h
    include/Twisted/Application/Application.h
    include/Twisted/Application/Processor.h
    include/Twisted/Application/Service.h
    include/Twisted/Application/TimeService.h
    include/Twisted/AssetsLayer/AssetImporter.h
    include/Twisted/AssetsLayer/AssetImporterRegistry.h
    include/Twisted/AssetsLayer/AssetInfo.h
    include/Twisted/AssetsLayer/AssetUuid.h
    include/Twisted/AssetsLayer/AssetUuidYaml.h
    include/Twisted/AssetsLayer/AssetsService.h
    include/Twisted/AssetsLayer/Project.h
    include/Twisted/AssetsLayer/ProjectConfig.h
    include/Twisted/AssetsLayer/Importers/MaterialImporter.h
    include/Twisted/AssetsLayer/Importers/ModelImporter.h
    include/Twisted/AssetsLayer/Importers/ShaderImporter.h
    include/Twisted/AssetsLayer/Importers/TextureImporter.h
    include/Twisted/AssetsLayer/Importers/WorldImporter.h
    include/Twisted/BuiltIn/BuiltInRegistry.h
    include/Twisted/BuiltIn/Meshes/CubeMesh.h
    include/Twisted/BuiltIn/Meshes/QuadMesh.h
    include/Twisted/BuiltIn/Meshes/SphericalMesh.h
    include/Twisted/BuiltIn/Meshes/TriangleMesh.h
    include/Twisted/BuiltIn/Shaders/Debug_Normal_Shader.h
    include/Twisted/BuiltIn/Shaders/Debug_UV_Shader.h
    include/Twisted/BuiltIn/Shaders/Default_Shader.h
    include/Twisted/BuiltIn/Shaders/Light_Shader.h
    include/Twisted/BuiltIn/Shaders/Simple_Shader.h
    include/Twisted/Gameing/AComponent.h
    include/Twisted/Gameing/Entity.h
    include/Twisted/Gameing/GameService.h
    include/Twisted/Gameing/ManagerBase.h
    include/Twisted/Gameing/SystemBase.h
    include/Twisted/Gameing/World.h
    include/Twisted/Gameing/WorldRegistry.h
    include/Twisted/Gameing/Components/CCamera.h
    include/Twisted/Gameing/Components/CName.h
    include/Twisted/Gameing/Components/CRenderer.h
    include/Twisted/Gameing/Components/CTransform.h
    include/Twisted/Gameing/Components/DirectionalLightComponent.h
    include/Twisted/Gameing/Components/PointLightComponent.h
    include/Twisted/Gameing/Components/SpotLightComponent.h
    include/Twisted/Gameing/Managers/CameraManager.h
    include/Twisted/Gameing/Managers/RootTransformManager.h
    include/Twisted/Rendering/BufferGPU.h
    include/Twisted/Rendering/FrameBuffer.h
    include/Twisted/Rendering/Material.h
    include/Twisted/Rendering/Mesh.h
    include/Twisted/Rendering/RenderAPI.h
    include/Twisted/Rendering/RenderConvert.h
    include/Twisted/Rendering/RenderService.h
    include/Twisted/Rendering/Shader.h
    include/Twisted/Rendering/Texture.h
    include/Twisted/Rendering/UniformBuffer.h
    include/Twisted/Rendering/Data/ClearParams.h
    include/Twisted/Rendering/Data/MeshData.h
    include/Twisted/Rendering/Data/RenderContext.h
    include/Twisted/Rendering/Data/ShaderData.h
    include/Twisted/Rendering/Data/ShaderLayouts.h
    include/Twisted/Rendering/Data/TextureData.h
    include/Twisted/Rendering/OpenGL/FrameBuffer_OpenGL.h
    include/Twisted/Rendering/OpenGL/Mesh_OpenGL.h
    include/Twisted/Rendering/OpenGL/ShaderReflection_OpenGL.h
    include/Twisted/Rendering/OpenGL/Shader_OpenGL.h
    include/Twisted/Rendering/OpenGL/Texture_OpenGL.h
    include/Twisted/Rendering/OpenGL/UniformBuffer_OpenGL.h
    include/Twisted/Windowing/ButtonCodes.h
    include/Twisted/Windowing/Input.h
    include/Twisted/Windowing/KeyCodes.h
    include/Twisted/Windowing/MonitorInfo.h
        include/Twisted/Windowing/WIN32/Win32Utils.h
    include/Twisted/Windowing/GLFW/Window_GLFW.h
    include/Twisted/Windowing/Window.h
    include/Twisted/Windowing/WindowCoreAPI.h
    include/Twisted/Windowing/WindowEventRegistry.h
    include/Twisted/Windowing/WindowEvents.h
    include/Twisted/Windowing/WindowsService.h
    include/Twisted/Windowing/WIN32/Window_Win32.h
    include/Twisted/Windowing/WIN32/WindowsUtils_Win32.h
)

source_group(
    TREE ${CMAKE_CURRENT_SOURCE_DIR}
    FILES ${TWISTED_ENGINE_SOURCES} ${TWISTED_ENGINE_HEADERS})

# ------------------------------
# Create shared library
# ------------------------------

add_library(TwistedEngine SHARED 
    ${TWISTED_ENGINE_SOURCES} 
    ${TWISTED_ENGINE_HEADERS}
)

set_target_properties(TwistedEngine PROPERTIES
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    POSITION_INDEPENDENT_CODE ON
)
target_compile_features(TwistedEngine PUBLIC cxx_std_23)
# ------------------------------
# Include directories
# ------------------------------

target_include_directories(TwistedEngine PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    #"${CMAKE_SOURCE_DIR}/3rd/ShaderParser/ShaderParser/src"
)

# third party with suppressed warnings
#target_include_directories(TwistedEngine SYSTEM PUBLIC
#    
#    "${CMAKE_SOURCE_DIR}/vcpkg_installed/x64-windows/include"
#)

# ------------------------------
# Libraries
# ------------------------------
find_package(OpenGL REQUIRED)
#find_package(Vulkan REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(yaml-cpp CONFIG REQUIRED)


target_link_libraries(TwistedEngine PRIVATE
    OpenGL::GL
#    Vulkan::Vulkan
    EnTT::EnTT
    glm::glm
    glad::glad
    spdlog::spdlog
    yaml-cpp::yaml-cpp
    #ShaderParser
)

if(WIN32)
    target_link_libraries(TwistedEngine PRIVATE dwmapi)
else()
    find_package(glfw3 CONFIG REQUIRED)
    target_link_libraries(TwistedEngine PRIVATE glfw)
endif()

# ------------------------------
# Compile definitions
# ------------------------------

target_compile_definitions(TwistedEngine PRIVATE
    TWISTED_ENGINE
    $<$<CONFIG:Debug>:TWISTED_DEBUG>
    $<$<CONFIG:RelWithDebInfo>:TWISTED_RELEASE>
    $<$<CONFIG:Release>:TWISTED_DEPLOY>
)

if(MSVC)
  target_compile_definitions(TwistedEngine PRIVATE 
    _CRT_SECURE_NO_WARNINGS)
endif()

# ------------------------------
# Compiler options
# ------------------------------
option(TWISTED_WERROR "Treat warnings as errors" OFF)

if(MSVC)
  target_compile_options(TwistedEngine PRIVATE
    /W4
    /external:W0
    /experimental:external
    /analyze-
  )
  if(TWISTED_WERROR)
    target_compile_options(TwistedEngine PRIVATE /WX)
  endif()
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
  target_compile_options(TwistedEngine PRIVATE
    -Wall
    -Wextra
    -Wpedantic
  )
  if(TWISTED_WERROR)
    target_compile_options(TwistedEngine PRIVATE -Werror)
  endif()
endif()
