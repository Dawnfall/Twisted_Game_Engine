file(GLOB_RECURSE TWISTED_ENGINE_SOURCES CONFIGURE_DEPENDS
    "*.cpp"
    "*.h"
    )

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${TWISTED_ENGINE_SOURCES})

add_library(TwistedEngine SHARED ${TWISTED_ENGINE_SOURCES})

set_target_properties(TwistedEngine PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO

    OUTPUT_NAME "TwistedEngine$<$<CONFIG:Debug>:_d>"
    # MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL"
)

target_include_directories(TwistedEngine PUBLIC
    ${IncludeDirs_TwistedEngine}
    ${IncludeDirs_ShaderParser}
    "${CMAKE_SOURCE_DIR}/vcpkg_installed/x64-windows/include"
)

# Create the source groups for source tree with root at CMAKE_CURRENT_SOURCE_DIR.
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${TWISTED_ENGINE_SOURCES})

target_link_libraries(TwistedEngine PRIVATE
    opengl32
    glm::glm
    glad::glad
    spdlog::spdlog
    yaml-cpp::yaml-cpp
    rapidobj::rapidobj
    ShaderParser

    $<$<BOOL:${USE_GLFW}>:glfw>
)

target_compile_definitions(TwistedEngine PRIVATE
    TWISTED_ENGINE
    $<$<CONFIG:Debug>:TWISTED_DEBUG>
    $<$<CONFIG:Release>:TWISTED_RELEASE>
    $<$<CONFIG:Deploy>:TWISTED_DEPLOY>

    $<$<BOOL:${USE_GLFW}>:GLFW_INCLUDE_NONE>
    $<$<BOOL:${USE_GLFW}>:GLFW_EXCLUDE_VULKAN>
)

target_compile_options(TwistedEngine PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
    $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra -Wpedantic -Werror>
    $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -Wpedantic -Werror>
)

target_compile_options(TwistedEngine PRIVATE
    $<$<CONFIG:Debug>:/Od /Zi>
    $<$<CONFIG:Release>:/O2 /Zi>
    $<$<CONFIG:Deploy>:/Ox>
)

if(MSVC)
    target_compile_options(TwistedEngine PRIVATE
        /W4                     # Warning level 4
        /WX                     # Treat warnings as errors (engine code only)
        /external:W0            # Suppress warnings from external headers
        /experimental:external  # Required for /external:W0 to work
        /analyze-               # Disable full static analysis on this target
    )
endif()



target_include_directories(TwistedEngine SYSTEM PUBLIC
    "${CMAKE_SOURCE_DIR}/vcpkg_installed/x64-windows/include"
)

target_compile_definitions(TwistedEngine PRIVATE _CRT_SECURE_NO_WARNINGS)