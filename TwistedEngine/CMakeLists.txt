cmake_minimum_required(VERSION 3.28)

# ------------------------------
# Source files
# ------------------------------

set(TWISTED_ENGINE_SOURCES
    src/Twisted_compile.cpp
)

# ------------------------------
# Header files
# ------------------------------

set(TWISTED_ENGINE_HEADERS
    include/Twisted.h
)

source_group(
    TREE ${CMAKE_CURRENT_SOURCE_DIR}
    FILES ${TWISTED_ENGINE_SOURCES} ${TWISTED_ENGINE_HEADERS}
)

# ------------------------------
# Create shared library
# ------------------------------

add_library(TwistedEngine SHARED
    ${TWISTED_ENGINE_SOURCES}
    ${TWISTED_ENGINE_HEADERS}
)

set_target_properties(TwistedEngine PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    POSITION_INDEPENDENT_CODE ON
)
# ------------------------------
# Include directories
# ------------------------------

target_include_directories(TwistedEngine PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_SOURCE_DIR}/Core/include/"
    "${CMAKE_SOURCE_DIR}/Window/include/"
    "${CMAKE_SOURCE_DIR}/Assets/include/"
    "${CMAKE_SOURCE_DIR}/World/include/"
    "${CMAKE_SOURCE_DIR}/Renderer/include/"
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
    $<LINK_LIBRARY:WHOLE_ARCHIVE,Core>
    $<LINK_LIBRARY:WHOLE_ARCHIVE,Window>
    $<LINK_LIBRARY:WHOLE_ARCHIVE,Renderer>
    $<LINK_LIBRARY:WHOLE_ARCHIVE,World>
    $<LINK_LIBRARY:WHOLE_ARCHIVE,Assets>
    ShaderParser
)

# ------------------------------
# Compile definitions
# ------------------------------

target_compile_definitions(TwistedEngine PRIVATE
    TWISTED_ENGINE
    $<$<CONFIG:Debug>:TWISTED_DEBUG>
    $<$<CONFIG:RelWithDebInfo>:TWISTED_RELEASE>
    $<$<CONFIG:Release>:TWISTED_DEPLOY>
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
