cmake_minimum_required(VERSION 3.28)

# ------------------------------
# Check vcpkg environment
# ------------------------------

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    if(DEFINED ENV{VCPKG_R})
        if(EXISTS "$ENV{VCPKG_R}/scripts/buildsystems/vcpkg.cmake")
            set( CMAKE_TOOLCHAIN_FILE 
            "$ENV{VCPKG_R}/scripts/buildsystems/vcpkg.cmake" 
            CACHE FILEPATH "")
        else()
            message(FATAL_ERROR "Invalid VCPKG_R path")
        endif()
    else()
        message("Please set VCPKG_R variable as root VCPKG folder")
    endif()
endif()
    


# ------------------------------
# C++ standard
# ------------------------------

project(TwistedEngine LANGUAGES CXX)

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
# Default build type (single-config generators only)
# ------------------------------

get_property(_isMultiConfig GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(NOT _isMultiConfig)
    if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the build type" FORCE)
    endif()
endif()

# ------------------------------
# Output directories (organized by config + platform + arch)
# ------------------------------

set(_platform "${CMAKE_SYSTEM_NAME}")

# Normalize common architecture strings
set(_arch "${CMAKE_SYSTEM_PROCESSOR}")
if(_arch STREQUAL "AMD64")
    set(_arch "x86_64")
elseif(_arch STREQUAL "x64")
    set(_arch "x86_64")
endif()

set(_outdir "${CMAKE_BINARY_DIR}/bin/$<CONFIG>-${_platform}-${_arch}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${_outdir}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${_outdir}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${_outdir}")

# ------------------------------
# Third-party subprojects
# ------------------------------

include(cmake/ImGui.cmake)
include(cmake/ImGuizmo.cmake)
#add_subdirectory(3rd/ShaderParser/ShaderParser)
add_subdirectory(TwistedEngine)
add_subdirectory(Editor)

# ------------------------------
# IDE and folder settings
# ------------------------------

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Visual Studio-only startup project setting
if(MSVC)
    set_property(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" PROPERTY VS_STARTUP_PROJECT Editor)
endif()

# ------------------------------
# Define allowed configurations (optional, helps IDEs)
# ------------------------------

if(_isMultiConfig)
    set(CMAKE_CONFIGURATION_TYPES Debug RelWithDebInfo Release CACHE STRING "" FORCE)
endif()
