file(GLOB_RECURSE TWISTED_ENGINE_SOURCES CONFIGURE_DEPENDS
    "*.cpp"
    "*.h"
    )

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${TWISTED_ENGINE_SOURCES})

add_library(TwistedEngine SHARED ${TWISTED_ENGINE_SOURCES})

set_target_properties(TwistedEngine PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO

    OUTPUT_NAME "TwistedEngine$<$<CONFIG:Debug>:_d>"
    # MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL"

    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES
)

target_include_directories(TwistedEngine PUBLIC
    ${IncludeDirs_TwistedEngine}
)

# Create the source groups for source tree with root at CMAKE_CURRENT_SOURCE_DIR.
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${TWISTED_ENGINE_SOURCES})

find_package(OpenGL REQUIRED)

target_link_libraries(TwistedEngine PRIVATE
    OpenGL::GL
    glm::glm
    glad::glad
    spdlog::spdlog
    yaml-cpp::yaml-cpp
    assimp::assimp
)

target_compile_definitions(TwistedEngine PRIVATE
    TWISTED_ENGINE
    $<$<CONFIG:Debug>:TWISTED_DEBUG>
    $<$<CONFIG:Release>:TWISTED_RELEASE>
    $<$<CONFIG:Deploy>:TWISTED_DEPLOY>
)
if(MSVC)
    target_compile_options(TwistedEngine PRIVATE
        $<$<CONFIG:Debug>:/Od /Zi>
        $<$<CONFIG:Release>:/O2 /Zi>
        $<$<CONFIG:Deploy>:/Ox>
    )
else()
    target_compile_options(TwistedEngine PRIVATE
        $<$<CONFIG:Debug>:-O0 -g>
        $<$<CONFIG:Release>:-O2 -g>
        $<$<CONFIG:Deploy>:-O3>
    )
endif()