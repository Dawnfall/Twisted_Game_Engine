cmake_minimum_required(VERSION 3.4)
project (TwistedEngine LANGUAGES CXX)

if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "This project only supports x64 architecture.")
endif()

# default is debug
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the build type" FORCE)
endif()

set(OUTPUT_DIR "${CMAKE_BUILD_TYPE}-${CMAKE_SYSTEM_NAME}-x64")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${OUTPUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${OUTPUT_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${OUTPUT_DIR})

# include dirs
set(IncludeDirs_TwistedEngine "${CMAKE_SOURCE_DIR}/TwistedEngine/src")
set(IncludeDirs_Editor "${CMAKE_SOURCE_DIR}/Editor/src")
set(IncludeDirs_Standalone "${CMAKE_SOURCE_DIR}/Standalone/src")

set(IncludeDirs_imgui 
    "${CMAKE_SOURCE_DIR}/3rd/imgui"
    "${CMAKE_SOURCE_DIR}/3rd/imgui/backends"
)

find_package(glm CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(entt CONFIG REQUIRED)

#third party
add_subdirectory(3rd/imgui)
#add_subdirectory(3rd/pugixml)

#submodules
add_subdirectory(TwistedEngine)
add_subdirectory(Editor)
add_subdirectory(Standalone)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Editor)

