option(USE_GLFW "Use glfw" ON) # TODO:... remove this


# ImGui CMake setup for TwistedEngine
# This builds ImGui as a static library including backends for OpenGL and either GLFW or Win32.

set(IMGUI_DIR "${CMAKE_CURRENT_LIST_DIR}/../3rd/imgui")

# Core ImGui source files
set(IMGUI_SOURCES
    "${IMGUI_DIR}/imgui.cpp"
    "${IMGUI_DIR}/imgui_draw.cpp"
    "${IMGUI_DIR}/imgui_tables.cpp"
    "${IMGUI_DIR}/imgui_widgets.cpp"
    "${IMGUI_DIR}/imgui_demo.cpp"
)

# Always include OpenGL3 backend
list(APPEND IMGUI_SOURCES
    "${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp"
)

# Include GLFW or Win32 backend depending on USE_GLFW option
message(STATUS "ImGui: Using Win32 + OpenGL3 backends")
list(APPEND IMGUI_SOURCES "${IMGUI_DIR}/backends/imgui_impl_win32.cpp")

# Define ImGui static library
add_library(imgui STATIC ${IMGUI_SOURCES})

# Include paths (both core and backends)
target_include_directories(imgui PUBLIC
     "${IMGUI_DIR}"
     "${IMGUI_DIR}/backends"
)

# Win32 backend only requires <windows.h>
target_compile_definitions(imgui PUBLIC
    IMGUI_IMPL_WIN32_DISABLE_GAMEPAD
    IMGUI_IMPL_OPENGL_LOADER_GLAD
)

# Optional: make it visible as a folder in Visual Studio
set_target_properties(imgui PROPERTIES FOLDER "3rdParty")

message(STATUS "ImGui configured successfully.")
