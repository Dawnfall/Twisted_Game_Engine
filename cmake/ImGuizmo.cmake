
set(IMGUIZMO_DIR ${CMAKE_SOURCE_DIR}/3rd/imguizmo)
set(IMGUIZMO_SOURCES
    ${CMAKE_SOURCE_DIR}/3rd/imguizmo/ImGuizmo.cpp
)

add_library(imguizmo STATIC ${IMGUIZMO_SOURCES})

target_include_directories(imguizmo PUBLIC
    ${CMAKE_SOURCE_DIR}/3rd/imguizmo
    ${CMAKE_SOURCE_DIR}/3rd/imgui   # required because ImGuizmo includes imgui.h
)

# Required for ImGuizmo — defines extra math ops in imgui_internal.h
target_compile_definitions(imguizmo PUBLIC
    IMGUI_DEFINE_MATH_OPERATORS
)

# ImGuizmo depends on ImGui but does NOT include it automatically.
# You must link against imgui explicitly.
target_link_libraries(imguizmo PUBLIC imgui)

# Optional: make it visible as a folder in Visual Studio
set_target_properties(imguizmo PROPERTIES FOLDER "3rdParty")