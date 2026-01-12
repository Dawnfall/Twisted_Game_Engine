cmake_minimum_required(VERSION 4.1.1)


# ------------------------------
# Collect source files
# ------------------------------

file(GLOB_RECURSE TWISTED_ENGINE_SOURCES CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB_RECURSE TWISTED_ENGINE_HEADERS CONFIGURE_DEPENDS "include/*.h")

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${TWISTED_ENGINE_SOURCES} ${TWISTED_ENGINE_HEADERS})

# ------------------------------
# Create shared library
# ------------------------------

add_library(TwistedEngine SHARED ${TWISTED_ENGINE_SOURCES} ${TWISTED_ENGINE_HEADERS})

set_target_properties(TwistedEngine PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

# ------------------------------
# Include directories
# ------------------------------

target_include_directories(TwistedEngine PUBLIC
    "${CMAKE_SOURCE_DIR}/TwistedEngine/include"
    #"${CMAKE_SOURCE_DIR}/3rd/ShaderParser/ShaderParser/src"
)

# third party with suppressed warnings
#target_include_directories(TwistedEngine SYSTEM PUBLIC
#    
#    "${CMAKE_SOURCE_DIR}/vcpkg_installed/x64-windows/include"
#)

# ------------------------------
# Libraries
# ------------------------------

target_link_libraries(TwistedEngine PRIVATE
    opengl32
    glm::glm
    glad::glad
    spdlog::spdlog
    yaml-cpp::yaml-cpp
    rapidobj::rapidobj
    #ShaderParser

    $<$<BOOL:${USE_GLFW}>:glfw>
)

# ------------------------------
# Compile definitions
# ------------------------------

target_compile_definitions(TwistedEngine PRIVATE
    TWISTED_ENGINE
    $<$<CONFIG:Debug>:TWISTED_DEBUG>
    $<$<CONFIG:RelWithDebInfo>:TWISTED_RELEASE>
    $<$<CONFIG:Release>:TWISTED_DEPLOY>
    $<$<BOOL:${USE_GLFW}>:GLFW_INCLUDE_NONE>
    $<$<BOOL:${USE_GLFW}>:GLFW_EXCLUDE_VULKAN>

    _CRT_SECURE_NO_WARNINGS  # only affects third-party headers
)

# ------------------------------
# Compiler options
# ------------------------------

if(MSVC)
    target_compile_options(TwistedEngine PRIVATE
        /W4                     # Warning level 4
        /WX                     # Treat warnings as errors (engine code only)
        /external:W0            # Suppress warnings from external headers
        /experimental:external  # Required for /external:W0 to work
        /analyze-               # Disable full static analysis on this target

        $<$<CONFIG:Debug>:/Od /Zi>
        $<$<CONFIG:RelWithDebInfo>:/O2 /Zi>
        $<$<CONFIG:Release>:/Ox>
    )
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(TwistedEngine PRIVATE
        -Wall 
        -Wextra 
        -Wpedantic 
        -Werror

        $<$<CONFIG:Debug>:-O0 -g>
        $<$<CONFIG:RelWithDebInfo>:-O2 -g>
        $<$<CONFIG:Release>:-O3>
    )
endif()
