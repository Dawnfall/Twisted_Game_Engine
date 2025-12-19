cmake_minimum_required(VERSION 4.1.1)

# ------------------------------
# Collect source files
# ------------------------------

file(GLOB_RECURSE EDITOR_SOURCES CONFIGURE_DEPENDS
    "*.cpp"
    "*.h"
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${EDITOR_SOURCES})

# ------------------------------
# Create executable
# ------------------------------

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
    ${IncludeDirs_Editor}          # Your engine/editor code
    ${IncludeDirs_TwistedEngine}   # Engine headers
    ${IncludeDirs_imgui}
    ${IncludeDirs_imguizmo}
)

# Third-party includes (suppress warnings)
target_include_directories(Editor SYSTEM PRIVATE
    "${CMAKE_SOURCE_DIR}/vcpkg_installed/x64-windows/include"
)

# ------------------------------
# Link libraries
# ------------------------------

target_link_libraries(Editor PRIVATE
    TwistedEngine
    glm::glm
    imgui
    imguizmo
    yaml-cpp::yaml-cpp
    $<$<BOOL:${USE_GLFW}>:glfw>
)

# ------------------------------
# Backend sources depending on GLFW usage
# ------------------------------
if(USE_GLFW)
    target_sources(Editor PRIVATE
        ${CMAKE_SOURCE_DIR}/3rd/imgui/backends/imgui_impl_glfw.cpp
        ${CMAKE_SOURCE_DIR}/3rd/imgui/backends/imgui_impl_opengl3.cpp
    )
else()
    target_sources(Editor PRIVATE
        ${CMAKE_SOURCE_DIR}/3rd/imgui/backends/imgui_impl_win32.cpp
        ${CMAKE_SOURCE_DIR}/3rd/imgui/backends/imgui_impl_opengl3.cpp
    )
endif()

# ------------------------------
# Compile definitions
# ------------------------------

add_compile_definitions(PROJECT_ROOT_PATH="${CMAKE_SOURCE_DIR}")

target_compile_definitions(Editor PRIVATE
    $<$<CONFIG:Debug>:TWISTED_DEBUG>
    $<$<CONFIG:RelWithDebInfo>:TWISTED_RELEASE>
    $<$<CONFIG:Release>:TWISTED_DEPLOY>
    $<$<BOOL:${USE_GLFW}>:GLFW_INCLUDE_NONE>
    $<$<BOOL:${USE_GLFW}>:GLFW_EXCLUDE_VULKAN>

    _CRT_SECURE_NO_WARNINGS  # only affects 3rd-party headers
)

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

        $<$<CONFIG:Debug>:/Od /Zi>
        $<$<CONFIG:RelWithDebInfo>:/O2 /Zi>
        $<$<CONFIG:Release>:/Ox>
    )
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(Editor PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Werror

        $<$<CONFIG:Debug>:-O0 -g>
        $<$<CONFIG:RelWithDebInfo>:-O2 -g>
        $<$<CONFIG:Release>:-O3>
    )
endif()