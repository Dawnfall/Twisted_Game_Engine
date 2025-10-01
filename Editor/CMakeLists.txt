file(GLOB_RECURSE EDITOR_SOURCES CONFIGURE_DEPENDS
    "src/*.h"
    "src/*.cpp"
)

add_executable(Editor ${EDITOR_SOURCES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" FILES ${EDITOR_SOURCES})

set_target_properties(Editor PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO

    OUTPUT_NAME "Editor$<$<CONFIG:Debug>:_d>"
)


set_property(TARGET Editor PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

target_include_directories(Editor PRIVATE
    ${IncludeDirs_Editor}
    ${IncludeDirs_TwistedEngine}
    ${IncludeDirs_imgui}
)

target_link_libraries(Editor PRIVATE
    TwistedEngine
    glfw
    glm::glm
    imgui
    yaml-cpp::yaml-cpp
)

target_compile_definitions(Editor PRIVATE
    GLFW_INCLUDE_NONE
    GLFW_EXCLUDE_VULKAN

    $<$<CONFIG:Debug>:TWISTED_DEBUG>
    $<$<CONFIG:Release>:TWISTED_RELEASE>
)
target_compile_options(Editor PRIVATE
    $<$<CONFIG:Debug>:/Zi>      # Enable debug info (MSVC)
    $<$<CONFIG:Release>:/O2>    # Optimize for speed (MSVC)
)