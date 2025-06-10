cmake_minimum_required(VERSION 3.4)
project (TwistedEngine LANGUAGES CXX)

# intended for x64 architecture
if(CMAKE_SIZE_OF_VOID_P EQUAL 8)
    message(STATUS "Target architecture: x64")
else()
    message(WARNING "Target architecture is not x64")
endif()

# default is debug
if(NOT CMAKE_BUID_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the build type" FORCE)
endif()

set(OUTPUT_DIR "${CMAKE_BUILD_TYPE}-${CMAKE_SYSTEM_NAME}-x64")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${OUTPUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${OUTPUT_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${OUTPUT_DIR})

# include dirs
set(IncludeDirs_Core "${CMAKE_SOURCE_DIR}/Core/src")
set(IncludeDirs_Windowing "${CMAKE_SOURCE_DIR}/Windowing/src")
set(IncludeDirs_Rendering "${CMAKE_SOURCE_DIR}/Rendering/src")
set(IncludeDirs_Gameing "${CMAKE_SOURCE_DIR}/Gameing/src")
set(IncludeDirs_TwistedEngine "${CMAKE_SOURCE_DIR}/TwistedEngine/src")
set(IncludeDirs_Editor "${CMAKE_SOURCE_DIR}/Editor/src")
set(IncludeDirs_Standalone "${CMAKE_SOURCE_DIR}/Standalone/src")

set(IncludeDirs_spdlog "${CMAKE_SOURCE_DIR}/3rd/spdlog/include")
set(IncludeDirs_GLFW "${CMAKE_SOURCE_DIR}/3rd/GLFW/include")
set(IncludeDirs_Glad "${CMAKE_SOURCE_DIR}/3rd/imgui")
set(IncludeDirs_GLM "${CMAKE_SOURCE_DIR}/3rd/glm")
set(IncludeDirs_imgui "${CMAKE_SOURCE_DIR}/3rd/imgui")
set(IncludeDirs_EnTT "${CMAKE_SOURCE_DIR}/3rd/EnTT/include")

#Dlls

#Libs

set(LibDirs_GLFW "${CMAKE_SOURCE_DIR}/3rd/GLFW/lib/glfw3.lib")

#third party
add_subdirectory(spdlog)
add_subdirectory(3rd/GLFW)
add_subdirectory(3rd/GLM)
add_subdirectory(3rd/Glad)
#add_subdirectory(3rd/imgui)
#add_subdirectory(3rd/pugixml)

#submodules
add_subdirectory(Core)
add_subdirectory(Windowing)
add_subdirectory(Rendering)
add_subdirectory(Gameing)
add_subdirectory(TwistedEngine)

#add_subdirectory(Editor)

#add_subdirectory(Standalone)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT TestGame)

