# ImGui CMake setup for TwistedEngine
# This builds ImGui as a static library including backends for OpenGL and either GLFW or Win32.

set(IMGUI_DIR ${CMAKE_SOURCE_DIR} /3rd/imgui)

# Core ImGui source files
set(IMGUI_SOURCES
    ${CMAKE_SOURCE_DIR}/3rd/imgui/imgui.cpp
    ${CMAKE_SOURCE_DIR}/3rd/imgui/imgui_draw.cpp
    ${CMAKE_SOURCE_DIR}/3rd/imgui/imgui_tables.cpp
    ${CMAKE_SOURCE_DIR}/3rd/imgui/imgui_widgets.cpp
    ${CMAKE_SOURCE_DIR}/3rd/imgui/imgui_demo.cpp
)

# Always include OpenGL3 backend
list(APPEND IMGUI_SOURCES
    ${CMAKE_SOURCE_DIR}/3rd/imgui/backends/imgui_impl_opengl3.cpp
)

# Include GLFW or Win32 backend depending on USE_GLFW option
if(USE_GLFW)
    message(STATUS "ImGui: Using GLFW + OpenGL3 backends")
    list(APPEND IMGUI_SOURCES ${CMAKE_SOURCE_DIR}/3rd/imgui/backends/imgui_impl_glfw.cpp)
else()
    message(STATUS "ImGui: Using Win32 + OpenGL3 backends")
    list(APPEND IMGUI_SOURCES ${CMAKE_SOURCE_DIR}/3rd/imgui/backends/imgui_impl_win32.cpp)
endif()

# Define ImGui static library
add_library(imgui STATIC ${IMGUI_SOURCES})

# Include paths (both core and backends)
target_include_directories(imgui PUBLIC
     ${CMAKE_SOURCE_DIR}/3rd/imgui
     ${CMAKE_SOURCE_DIR}/3rd/imgui/backends
)

# Backend-specific configuration
if(USE_GLFW)
    # Link to glfw (already found in root)
    target_link_libraries(imgui PUBLIC glfw)
    target_compile_definitions(imgui PUBLIC
        GLFW_INCLUDE_NONE
        GLFW_EXCLUDE_VULKAN
        IMGUI_IMPL_OPENGL_LOADER_GLAD
    )
else()
    # Win32 backend only requires <windows.h>
    target_compile_definitions(imgui PUBLIC
        IMGUI_IMPL_WIN32_DISABLE_GAMEPAD
        IMGUI_IMPL_OPENGL_LOADER_GLAD
    )
endif()

# Optional: make it visible as a folder in Visual Studio
set_target_properties(imgui PROPERTIES FOLDER "3rdParty")

message(STATUS "ImGui configured successfully.")
