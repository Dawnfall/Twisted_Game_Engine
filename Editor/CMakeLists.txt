file(GLOB_RECURSE EDITOR_SOURCES CONFIGURE_DEPENDS
    "src/*.h"
    "src/*.cpp"
)

add_executable(Editor ${EDITOR_SOURCES})

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
)

# add_custom_command(TARGET Editor POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_if_different
#         "$<TARGET_FILE:TwistedEngine>"  # Full path to built DLL
#         "$<TARGET_FILE_DIR:TwistedEditor>"
# )

        # postbuildcommands {
        #     "{COPY} %{wks.location}/bin/".. outputDir .. "/TwistedEngine/TwistedEngine_d.dll %{cfg.targetdir}",
        # }

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