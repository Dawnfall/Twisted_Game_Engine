file(GLOB_RECURSE EDITOR_SOURCES CONFIGURE_DEPENDS
    "src/*.cpp"
    "src/*.h"
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" FILES ${EDITOR_SOURCES})

add_executable(Editor ${EDITOR_SOURCES})

set_target_properties(Editor PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO

    OUTPUT_NAME "Editor$<$<CONFIG:Debug>:_d>"
    # MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL"
)

target_include_directories(Editor PRIVATE
    ${IncludeDirs_Editor}
    ${IncludeDirs_TwistedEngine}
    ${IncludeDirs_imgui}
)

target_link_libraries(Editor PRIVATE
    TwistedEngine
    #glfw
    glm::glm
    imgui
    yaml-cpp::yaml-cpp
)

target_compile_definitions(Editor PRIVATE
    $<$<CONFIG:Debug>:TWISTED_DEBUG>
    $<$<CONFIG:Release>:TWISTED_RELEASE>
    $<$<CONFIG:Deploy>:TWISTED_DEPLOY>
    #GLFW_INCLUDE_NONE
    #GLFW_EXCLUDE_VULKAN
)
target_compile_options(Editor PRIVATE
    $<$<CONFIG:Debug>:/Od /Zi>
    $<$<CONFIG:Release>:/O2 /Zi>
    $<$<CONFIG:Deploy>:/Ox>
)