cmake_minimum_required(VERSION 4.1.2)
project(TwistedEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Require 64-bit build
if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "This project only supports x64 architecture.")
endif()

# Default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the build type" FORCE)
endif()

if(NOT DEFINED ENV{VCPKG_ROOT})
    message(FATAL_ERROR "Please set VCPKG_ROOT to your vcpkg installation")
endif()

#if(MSVC)
#    add_compile_options("/utf-8")
#endif()

# Output directories (organized by build type and platform)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>-Windows-x64)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>-Windows-x64)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>-Windows-x64)

find_package(glm CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(entt CONFIG REQUIRED)
find_package(yaml-cpp CONFIG REQUIRED)
find_package(RapidObj CONFIG REQUIRED)

option(USE_GLFW "Use GLFW" ON)
if(USE_GLFW)
    message(STATUS "Building with GLFW")
    find_package(glfw3 CONFIG REQUIRED)
endif()

set(IncludeDirs_TwistedEngine "${CMAKE_SOURCE_DIR}/TwistedEngine/src")
set(IncludeDirs_Editor "${CMAKE_SOURCE_DIR}/Editor/src")
set(IncludeDirs_imgui  "${CMAKE_SOURCE_DIR}/3rd/imgui")

# Add ImGui as a static library (subproject)
add_subdirectory(3rd/imgui)
add_subdirectory(TwistedEngine)
add_subdirectory(Editor)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Define allowed configurations (optional, helps IDEs)
set(CMAKE_CONFIGURATION_TYPES Debug Release Deploy CACHE STRING "" FORCE)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Editor)











## default is debug
#if(NOT CMAKE_BUILD_TYPE)
#    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the build type" FORCE)
#endif()

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /FS")

## include dirs
## set(IncludeDirs_Standalone "${CMAKE_SOURCE_DIR}/Standalone/src")

## 
#

## add_subdirectory(Standalone)
#  


