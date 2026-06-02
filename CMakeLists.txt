cmake_minimum_required(VERSION 3.28)

# Setup vcpkg triplet BEFORE toolchain include
if(NOT DEFINED VCPKG_TARGET_TRIPLET)
    if(WIN32)
        set(VCPKG_TARGET_TRIPLET "x64-windows" CACHE STRING "vcpkg triplet")
    elseif(UNIX AND NOT APPLE)
        set(VCPKG_TARGET_TRIPLET "x64-linux" CACHE STRING "vcpkg triplet")
    endif()
endif()

# Setup vcpkg toolchain BEFORE project() so find_package works
if(NOT CMAKE_TOOLCHAIN_FILE)
    set(VCPKG_ROOT "")

    if(DEFINED ENV{VCPKG_ROOT})
        set(VCPKG_ROOT "$ENV{VCPKG_ROOT}")
    else()
        foreach(VCPKG_GUESS_PATH
            "D:/Programiranje/Tools/vcpkg"
            "$ENV{USERPROFILE}/vcpkg"
            "$ENV{USERPROFILE}/scoop/apps/vcpkg/current"
            "C:/vcpkg"
            "/home/$ENV{USER}/vcpkg"
            "$HOME/vcpkg"
        )
            if(EXISTS "${VCPKG_GUESS_PATH}/scripts/buildsystems/vcpkg.cmake")
                set(VCPKG_ROOT "${VCPKG_GUESS_PATH}")
                message(STATUS "[TwistedEngine] Auto-detected vcpkg at: ${VCPKG_ROOT}")
                break()
            endif()
        endforeach()
    endif()

    if(NOT VCPKG_ROOT OR NOT EXISTS "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
        message(FATAL_ERROR
            "vcpkg not found!\n"
            "Either:\n"
            "  1. Set environment variable: set VCPKG_ROOT=C:\\path\\to\\vcpkg\n"
            "  2. Clone vcpkg: git clone https://github.com/microsoft/vcpkg.git\n"
            "  3. Bootstrap: .\\vcpkg\\bootstrap-vcpkg.bat"
        )
    endif()

    set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE FILEPATH "vcpkg toolchain")
endif()

# Include vcpkg toolchain file directly to set up find_package() system
include(${CMAKE_TOOLCHAIN_FILE})

project(TwistedEngine LANGUAGES CXX)

if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    message(FATAL_ERROR
        "This project requires Clang.\n"
        "Configure with a preset or pass -DCMAKE_CXX_COMPILER=clang++"
    )
endif()
message(STATUS "[TwistedEngine] Using Clang compiler ✓")

set(CMAKE_CXX_STANDARD 23)
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

option(TWISTED_WERROR "Treat warnings as errors" OFF)

# Output directories
if(WIN32)
    set(PLATFORM_SUFFIX "Windows-x64")
elseif(UNIX AND NOT APPLE)
    set(PLATFORM_SUFFIX "Linux-x64")
else()
    set(PLATFORM_SUFFIX "Unknown")
endif()
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${PLATFORM_SUFFIX})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${PLATFORM_SUFFIX})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${PLATFORM_SUFFIX})

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# 3rd party
include(3rd/cmake/ImGui.cmake)
include(3rd/cmake/ImGuizmo.cmake)

# Code generation
include(cmake/TwistedGen.cmake)

# Engine
add_subdirectory(ShaderParser)
add_subdirectory(TwistedEngine)

# Editor
add_subdirectory(Editor)
