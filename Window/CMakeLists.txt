cmake_minimum_required(VERSION 3.28)

####
# sources
###

set(WINDOW_SOURCES
    src/Input.cpp
    src/WindowsService.cpp
)

if(WIN32)
    list(APPEND WINDOW_SOURCES
        src/Win32/Win32Utils.cpp
        src/Win32/MonitorInfo_win32.cpp
        src/Win32/Window_win32.cpp
    )
else()
    list(APPEND WINDOW_SOURCES
        src/Linux/NativeUtils_Linux.cpp
        src/GLFW/Window_glfw.cpp
    )
endif()

####
# headers
####

set(WINDOW_HEADERS
    include/ButtonCodes.h
    include/Input.h
    include/KeyCodes.h
    include/MonitorInfo.h
    include/WIN32/Win32Utils.h
    include/GLFW/Window_GLFW.h
    include/Window.h
    include/WindowCoreAPI.h
    include/WindowEventRegistry.h
    include/WindowEvents.h
    include/WindowsService.h
    include/WIN32/Window_Win32.h
    include/WIN32/WindowsUtils_Win32.h
)


#source_group(
#    TREE ${CMAKE_CURRENT_SOURCE_DIR}
#    FILES ${TWISTED_ENGINE_SOURCES} ${TWISTED_ENGINE_HEADERS}
#)

# ------------------------------
# Create static library
# ------------------------------

add_library(Window STATIC
    ${WINDOW_SOURCES} 
    ${WINDOW_HEADERS}
)

set_target_properties(Window PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    POSITION_INDEPENDENT_CODE ON
)

# ------------------------------
# Include directories
# ------------------------------

target_include_directories(Window PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include/"
    "${CMAKE_SOURCE_DIR}/Core/include/"
)

# ------------------------------
# Libraries
# ------------------------------

if(WIN32)
    target_link_libraries(Window PRIVATE dwmapi shell32 glad::glad)
else()
    find_package(glfw3 CONFIG REQUIRED)
    target_link_libraries(Window PRIVATE glfw)
endif()

# ------------------------------
# Compile definitions
# ------------------------------

target_compile_definitions(Window PRIVATE
    TWISTED_SHARED_LIB
    TWISTED_ENGINE
    $<$<CONFIG:Debug>:TWISTED_DEBUG>
    $<$<CONFIG:RelWithDebInfo>:TWISTED_RELEASE>
    $<$<CONFIG:Release>:TWISTED_DEPLOY>
)

#if(MSVC)
#  target_compile_definitions(TwistedEngine PRIVATE 
#    _CRT_SECURE_NO_WARNINGS)
#endif()

# ------------------------------
# Compiler options
# ------------------------------
option(TWISTED_WERROR "Treat warnings as errors" OFF)

if(MSVC)
  target_compile_options(Window PRIVATE
    /W4
    /wd4251
    /external:W0
    /experimental:external
    /analyze-
  )
  if(TWISTED_WERROR)
    target_compile_options(Window PRIVATE /WX)
  endif()
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
  target_compile_options(Window PRIVATE
    -Wall
    -Wextra
    -Wpedantic
  )
  if(TWISTED_WERROR)
    target_compile_options(Window PRIVATE -Werror)
  endif()
endif()
