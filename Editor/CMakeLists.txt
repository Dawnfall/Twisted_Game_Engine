cmake_minimum_required(VERSION 3.28)

# ------------------------------
# Source files
# ------------------------------

set(EDITOR_SOURCES
    main.cpp
    src/EditorConstants.h
    src/EditorApp/EditorProcessor.cpp
    src/EditorApp/EditorProcessor.h
    src/EditorApp/EditorRegistry.h
    src/EditorApp/EditorService.cpp
    src/EditorApp/EditorService.h
    src/EditorData/EditorConfig.h
    src/EditorData/Selection.cpp
    src/EditorData/Selection.h
    src/EditorWorld/EditorCameraSystem.cpp
    src/EditorWorld/EditorCameraSystem.h
    src/UI/ComponentPainter.h
    src/UI/EditorPanel.h
    src/UI/ImguiExtensions.cpp
    src/UI/ImguiExtensions.h
    src/UI/UIWindow.cpp
    src/UI/UIWindow.h
    src/UI/Components/CCameraPainter.cpp
    src/UI/Components/CCameraPainter.h
    src/UI/Components/CNamePainter.cpp
    src/UI/Components/CNamePainter.h
    src/UI/Components/CRendererPainter.cpp
    src/UI/Components/CRendererPainter.h
    src/UI/Components/CTransformPainter.cpp
    src/UI/Components/CTransformPainter.h
    src/UI/Components/DirectionalLightPainter.cpp
    src/UI/Components/DirectionalLightPainter.h
    src/UI/Components/PointLightPainter.cpp
    src/UI/Components/PointLightPainter.h
    src/UI/Components/SpotLightPainter.cpp
    src/UI/Components/SpotLightPainter.h
    src/UI/Details/EntityPainter.cpp
    src/UI/Details/EntityPainter.h
    src/UI/Details/MaterialPainter.cpp
    src/UI/Details/MaterialPainter.h
    src/UI/Details/ShaderPainter.cpp
    src/UI/Details/ShaderPainter.h
    src/UI/Panels/AssetsPanel.cpp
    src/UI/Panels/AssetsPanel.h
    src/UI/Panels/DetailsPanel.cpp
    src/UI/Panels/DetailsPanel.h
    src/UI/Panels/GameViewPanel.cpp
    src/UI/Panels/GameViewPanel.h
    src/UI/Panels/TreeViewPanel.cpp
    src/UI/Panels/TreeViewPanel.h
    src/UI/Panels/WorldSystemPanel.cpp
    src/UI/Panels/WorldSystemPanel.h
    src/UI/Panels/WorldViewPanel.cpp
    src/UI/Panels/WorldViewPanel.h
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${EDITOR_SOURCES})

# ------------------------------
# Create executable
# ------------------------------

if(WIN32)
    list(APPEND EDITOR_SOURCES src/UI/ImguiExtensions_Win32.cpp)
else()
    list(APPEND EDITOR_SOURCES src/UI/ImguiExtensions_GLFW.cpp)
endif()

add_executable(Editor ${EDITOR_SOURCES})

set_target_properties(Editor PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO

    #OUTPUT_NAME "Editor$<$<CONFIG:Debug>:_d>"
)

# ------------------------------
# Include directories
# ------------------------------

target_include_directories(Editor PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"              # Your engine/editor code
    "${CMAKE_CURRENT_SOURCE_DIR}/../TwistedEngine/include"   # Engine headers
    #"${CMAKE_CURRENT_SOURCE_DIR}/../3rd/imgui"
    #"${CMAKE_CURRENT_SOURCE_DIR}/../3rd/imguizmo"
)

# ------------------------------
# Link libraries
# ------------------------------

find_package(glm CONFIG REQUIRED)
find_package(yaml-cpp CONFIG REQUIRED)

target_link_libraries(Editor PRIVATE
    TwistedEngine
    glm::glm
    imgui
    imguizmo
    yaml-cpp::yaml-cpp
)

if(NOT WIN32)
    find_package(glfw3 CONFIG REQUIRED)
    target_link_libraries(Editor PRIVATE glfw)
endif()

# ------------------------------
# Backend sources depending on GLFW usage
# ------------------------------
#if(USE_GLFW)
#    target_sources(Editor PRIVATE
#        ${CMAKE_SOURCE_DIR}/3rd/imgui/backends/imgui_impl_glfw.cpp
#        ${CMAKE_SOURCE_DIR}/3rd/imgui/backends/imgui_impl_opengl3.cpp
#    )
#else()
#    target_sources(Editor PRIVATE
#        ${CMAKE_SOURCE_DIR}/3rd/imgui/backends/imgui_impl_win32.cpp
#        ${CMAKE_SOURCE_DIR}/3rd/imgui/backends/imgui_impl_opengl3.cpp
#    )
#endif()

# ------------------------------
# Compile definitions
# ------------------------------

target_compile_definitions(Editor PRIVATE
    PROJECT_ROOT_PATH="${CMAKE_SOURCE_DIR}"
    $<$<CONFIG:Debug>:TWISTED_DEBUG>
    $<$<CONFIG:RelWithDebInfo>:TWISTED_RELEASE>
    $<$<CONFIG:Release>:TWISTED_DEPLOY>
)

if(MSVC)
  target_compile_definitions(Editor PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

# ------------------------------
# Compiler options
# ------------------------------

if(MSVC)
    target_compile_options(Editor PRIVATE
        /W4                     # Warning level 4
        /WX                     # Treat warnings as errors (your code only)
        /external:W0            # Suppress warnings from external headers
        /experimental:external  # Required for /external:W0
        /analyze-               # Disable full static analysis
        /wd4251
        /wd26495
    )
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(Editor PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Werror
    )
endif()