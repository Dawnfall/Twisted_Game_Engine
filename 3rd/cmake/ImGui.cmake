set(IMGUI_DIR "${CMAKE_CURRENT_LIST_DIR}/../imgui")

set(IMGUI_SOURCES
    "${IMGUI_DIR}/imgui.cpp"
    "${IMGUI_DIR}/imgui_draw.cpp"
    "${IMGUI_DIR}/imgui_tables.cpp"
    "${IMGUI_DIR}/imgui_widgets.cpp"
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    list(APPEND IMGUI_SOURCES "${IMGUI_DIR}/imgui_demo.cpp")
endif()

list(APPEND IMGUI_SOURCES "${IMGUI_DIR}/backends/imgui_impl_vulkan.cpp")
list(APPEND IMGUI_SOURCES "${IMGUI_DIR}/backends/imgui_impl_glfw.cpp")

add_library(imgui STATIC ${IMGUI_SOURCES})

target_include_directories(imgui PUBLIC
     "${IMGUI_DIR}"
     "${IMGUI_DIR}/backends"
)

find_package(Vulkan REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(imgui PRIVATE Vulkan::Vulkan glfw)

set_target_properties(imgui PROPERTIES FOLDER "3rdParty")

message(STATUS "ImGui configured successfully.")
