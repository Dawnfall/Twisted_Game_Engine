cmake_minimum_required(VERSION 3.28)

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