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
if(USE_GLFW)
    message(STATUS "ImGui: Using GLFW + OpenGL3 backends")
    list(APPEND IMGUI_SOURCES "${IMGUI_DIR}/backends/imgui_impl_glfw.cpp")
else()
    message(STATUS "ImGui: Using Win32 + OpenGL3 backends")
    list(APPEND IMGUI_SOURCES "${IMGUI_DIR}/backends/imgui_impl_win32.cpp")
endif()

# Define ImGui static library
add_library(imgui STATIC ${IMGUI_SOURCES})

# Include paths (both core and backends)
target_include_directories(imgui PUBLIC
     "${IMGUI_DIR}"
     "${IMGUI_DIR}/backends"
)

# Backend-specific configuration
if(USE_GLFW)
    find_package(glfw3 CONFIG REQUIRED)

    # Link to glfw (already found in root)
    if(TARGET glfw)
        target_link_libraries(imgui PUBLIC glfw)
    elseif(TARGET glfw3::glfw)
        target_link_libraries(imgui PUBLIC glfw3::glfw)
    else()
        message(FATAL_ERROR "glfw3 found but no known target (glfw or glfw3::glfw) was exported")
    endif()

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
