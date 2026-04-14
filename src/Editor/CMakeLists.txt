file(GLOB_RECURSE EDITOR_SOURCES CONFIGURE_DEPENDS
    "*.cpp"
    "*.h"
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${EDITOR_SOURCES})

add_executable(Editor ${EDITOR_SOURCES} )

set_target_properties(Editor PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO

    OUTPUT_NAME "Editor$<$<CONFIG:Debug>:_d>"
    # MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL"
)

target_include_directories(Editor PRIVATE
    ${IncludeDirs_Editor}
    ${IncludeDirs_TwistedEngine}
    ${IncludeDirs_imgui}
    ${IncludeDirs_imguizmo}
)

target_link_libraries(Editor PRIVATE
    TwistedEngine
    glm::glm
    imgui
    imguizmo
    yaml-cpp::yaml-cpp

)

if(WIN32)
    target_sources(Editor PRIVATE
        ${CMAKE_SOURCE_DIR}/3rd/imgui/backends/imgui_impl_win32.cpp
        ${CMAKE_SOURCE_DIR}/3rd/imgui/backends/imgui_impl_opengl3.cpp
    )
else()
    target_sources(Editor PRIVATE
        # TODO: Add Linux X11 imgui platform backend
        ${CMAKE_SOURCE_DIR}/3rd/imgui/backends/imgui_impl_opengl3.cpp
    )
endif()

add_compile_definitions(PROJECT_ROOT_PATH="${CMAKE_SOURCE_DIR}")

target_compile_definitions(Editor PRIVATE
    $<$<CONFIG:Debug>:TWISTED_DEBUG>
    $<$<CONFIG:Release>:TWISTED_RELEASE>
    $<$<CONFIG:Deploy>:TWISTED_DEPLOY>
)
if(MSVC)
    target_compile_options(Editor PRIVATE
        $<$<CONFIG:Debug>:/Od /Zi>
        $<$<CONFIG:Release>:/O2 /Zi>
        $<$<CONFIG:Deploy>:/Ox>
    )
else()
    target_compile_options(Editor PRIVATE
        $<$<CONFIG:Debug>:-O0 -g>
        $<$<CONFIG:Release>:-O2 -g>
        $<$<CONFIG:Deploy>:-O3>
    )
endif()