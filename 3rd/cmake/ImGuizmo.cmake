set(IMGUI_DIR "${CMAKE_CURRENT_LIST_DIR}/../imgui")
set(IMGUIZMO_DIR "${CMAKE_CURRENT_LIST_DIR}/../imguizmo")
set(IMGUIZMO_SOURCES "${IMGUIZMO_DIR}/ImGuizmo.cpp")

add_library(imguizmo STATIC "${IMGUIZMO_SOURCES}")

target_include_directories(imguizmo PUBLIC
    "${IMGUIZMO_DIR}"
    "${IMGUI_DIR}"   # required because ImGuizmo includes imgui.h
)

# Enables extra math operators in imgui_internal.h required by ImGuizmo
target_compile_definitions(imguizmo PUBLIC
    IMGUI_DEFINE_MATH_OPERATORS
)

target_link_libraries(imguizmo PUBLIC imgui)

set_target_properties(imguizmo PROPERTIES FOLDER "3rdParty")