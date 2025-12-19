cmake_minimum_required(VERSION 4.1.1)
project(TwistedEngine LANGUAGES CXX)

# ------------------------------
# C++ standard
# ------------------------------
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ------------------------------
# Enforce 64-bit build
# ------------------------------

if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "This project only supports x64 architecture.")
endif()

# ------------------------------
# Default build type
# ------------------------------

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the build type" FORCE)
endif()

# ------------------------------
# Check vcpkg environment
# ------------------------------

if(NOT DEFINED ENV{VCPKG_ROOT})
    message(FATAL_ERROR "Please set VCPKG_ROOT to your vcpkg installation")
endif()

# ------------------------------
# Output directories (organized by build type and platform)
# ------------------------------

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>-Windows-x64)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>-Windows-x64)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>-Windows-x64)

# ------------------------------
# External dependencies (vcpkg)
# ------------------------------

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

# ------------------------------
# Project include directories
# ------------------------------

set(IncludeDirs_TwistedEngine "${CMAKE_SOURCE_DIR}/TwistedEngine/src")
set(IncludeDirs_Editor        "${CMAKE_SOURCE_DIR}/Editor/src")
set(IncludeDirs_imgui         "${CMAKE_SOURCE_DIR}/3rd/imgui")
set(IncludeDirs_imguizmo      "${CMAKE_SOURCE_DIR}/3rd/imguizmo")
set(IncludeDirs_ShaderParser  "${CMAKE_SOURCE_DIR}/3rd/ShaderParser/ShaderParser/src")

# ------------------------------
# Third-party subprojects
# ------------------------------

include(cmake/ImGui.cmake)
include(cmake/ImGuizmo.cmake)

add_subdirectory(3rd/ShaderParser/ShaderParser)
add_subdirectory(TwistedEngine)
add_subdirectory(Editor)

# ------------------------------
# IDE and folder settings
# ------------------------------

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Editor)

# ------------------------------
# Define allowed configurations (optional, helps IDEs)
# ------------------------------
set(CMAKE_CONFIGURATION_TYPES Debug RelWithDebInfo Release CACHE STRING "" FORCE)
