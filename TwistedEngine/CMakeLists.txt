cmake_minimum_required(VERSION 3.28)

# ------------------------------
# Collect source files
# ------------------------------

file(GLOB_RECURSE TWISTED_ENGINE_SOURCES CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB_RECURSE TWISTED_ENGINE_HEADERS CONFIGURE_DEPENDS "include/*.h")

source_group(
    TREE ${CMAKE_CURRENT_SOURCE_DIR} 
    FILES ${TWISTED_ENGINE_SOURCES} ${TWISTED_ENGINE_HEADERS})

# ------------------------------
# Create shared library
# ------------------------------

add_library(TwistedEngine SHARED 
    ${TWISTED_ENGINE_SOURCES} 
    ${TWISTED_ENGINE_HEADERS}
)

set_target_properties(TwistedEngine PROPERTIES
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    POSITION_INDEPENDENT_CODE ON
)
target_compile_features(TwistedEngine PUBLIC cxx_std_23)
# ------------------------------
# Include directories
# ------------------------------

target_include_directories(TwistedEngine PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
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
option(USE_GLFW "Use GLFW" ON)

find_package(OpenGL REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(entt CONFIG REQUIRED)
find_package(yaml-cpp CONFIG REQUIRED)
find_package(rapidobj CONFIG REQUIRED)

if(USE_GLFW)
    message(STATUS "Building with GLFW")
    find_package(glfw3 CONFIG REQUIRED)
endif()

target_link_libraries(TwistedEngine PRIVATE
    OpenGL::GL
    EnTT::EnTT
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
)

if(MSVC)
  target_compile_definitions(TwistedEngine PRIVATE 
    _CRT_SECURE_NO_WARNINGS)
endif()

# ------------------------------
# Compiler options
# ------------------------------
option(TWISTED_WERROR "Treat warnings as errors" OFF)

if(MSVC)
  target_compile_options(TwistedEngine PRIVATE
    /W4
    /external:W0
    /experimental:external
    /analyze-
  )
  if(TWISTED_WERROR)
    target_compile_options(TwistedEngine PRIVATE /WX)
  endif()
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
  target_compile_options(TwistedEngine PRIVATE
    -Wall
    -Wextra
    -Wpedantic
  )
  if(TWISTED_WERROR)
    target_compile_options(TwistedEngine PRIVATE -Werror)
  endif()
endif()
