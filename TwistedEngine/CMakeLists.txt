file(GLOB_RECURSE TWISTED_ENGINE_SOURCES CONFIGURE_DEPENDS
    "src/*.cpp"
    "src/*.h"
    )

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" FILES ${TWISTED_ENGINE_SOURCES})

add_library(TwistedEngine SHARED ${TWISTED_ENGINE_SOURCES})

set_target_properties(TwistedEngine PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO

    OUTPUT_NAME "TwistedEngine$<$<CONFIG:Debug>:_d>"
    # MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL"
)

target_include_directories(TwistedEngine PUBLIC
    ${IncludeDirs_TwistedEngine}
    "${CMAKE_SOURCE_DIR}/vcpkg_installed/x64-windows/include"
)

# Create the source groups for source tree with root at CMAKE_CURRENT_SOURCE_DIR.
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${TWISTED_ENGINE_SOURCES})

target_link_libraries(TwistedEngine PRIVATE
    opengl32
    glm::glm
    #glfw
    glad::glad
    spdlog::spdlog
    yaml-cpp::yaml-cpp
    rapidobj::rapidobj
)

target_compile_definitions(TwistedEngine PRIVATE
    TWISTED_ENGINE
    $<$<CONFIG:Debug>:TWISTED_DEBUG>
    $<$<CONFIG:Release>:TWISTED_RELEASE>
    $<$<CONFIG:Deploy>:TWISTED_DEPLOY>
    #GLFW_INCLUDE_NONE
    #GLFW_EXCLUDE_VULKAN
)
target_compile_options(TwistedEngine PRIVATE
    $<$<CONFIG:Debug>:/Od /Zi>
    $<$<CONFIG:Release>:/O2 /Zi>
    $<$<CONFIG:Deploy>:/Ox>
)