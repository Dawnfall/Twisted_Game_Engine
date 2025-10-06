file(GLOB_RECURSE STANDALONE_SOURCES CONFIGURE_DEPENDS
    "src/*.h"
    "src/*.cpp"
    "src/*.mpp"
)

file(GLOB_RECURSE STANDALONE_MODULES CONFIGURE_DEPENDS "src/*.mpp")
set_source_files_properties(${STANDALONE_MODULES} PROPERTIES LANGUAGE CXX)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" FILES ${STANDALONE_SOURCES})
source_group("Modules" FILES ${STANDALONE_MODULES})

add_executable(Standalone ${STANDALONE_SOURCES})

set_target_properties(Standalone PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO

    OUTPUT_NAME "Standalone$<$<CONFIG:Debug>:_d>"
)

set_property(TARGET Standalone PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

target_include_directories(Standalone PUBLIC
    ${IncludeDirs_Standalone}
    ${IncludeDirs_TwistedEngine}
    #glm::glm
)

target_link_libraries(Standalone PRIVATE
    TwistedEngine
)

target_compile_definitions(Standalone PRIVATE
    GLFW_INCLUDE_NONE
    GLFW_EXCLUDE_VULKAN

    $<$<CONFIG:Debug>:TWISTED_DEBUG>
    $<$<CONFIG:Release>:TWISTED_RELEASE>
)
target_compile_options(Standalone PRIVATE
    $<$<CONFIG:Debug>:/Zi>      # Enable debug info (MSVC)
    $<$<CONFIG:Release>:/O2>    # Optimize for speed (MSVC)
)