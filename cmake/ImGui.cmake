# ImGui CMake setup for TwistedEngine
# This builds ImGui as a static library including backends for OpenGL and Win32 (Windows only).

set(IMGUI_DIR "${CMAKE_CURRENT_LIST_DIR}/../3rd/imgui")

# Core ImGui source files
set(IMGUI_SOURCES
    "${IMGUI_DIR}/imgui.cpp"
    "${IMGUI_DIR}/imgui_draw.cpp"
    "${IMGUI_DIR}/imgui_tables.cpp"
    "${IMGUI_DIR}/imgui_widgets.cpp"
    "${IMGUI_DIR}/imgui_demo.cpp"
    "${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp"
)

if(WIN32)
    message(STATUS "ImGui: Using Win32 + OpenGL3 backends")
    list(APPEND IMGUI_SOURCES "${IMGUI_DIR}/backends/imgui_impl_win32.cpp")
else()
    message(STATUS "ImGui: Using GLFW + OpenGL3 backends")
    list(APPEND IMGUI_SOURCES "${IMGUI_DIR}/backends/imgui_impl_glfw.cpp")
endif()

# Define ImGui static library
add_library(imgui STATIC ${IMGUI_SOURCES})

# Include paths (both core and backends)
target_include_directories(imgui PUBLIC
     "${IMGUI_DIR}"
     "${IMGUI_DIR}/backends"
)

target_compile_definitions(imgui PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD)

if(WIN32)
    target_compile_definitions(imgui PUBLIC IMGUI_IMPL_WIN32_DISABLE_GAMEPAD)
else()
    find_package(glfw3 CONFIG REQUIRED)
    target_link_libraries(imgui PRIVATE glfw)
endif()

set_target_properties(imgui PROPERTIES FOLDER "3rdParty")

message(STATUS "ImGui configured successfully.")
