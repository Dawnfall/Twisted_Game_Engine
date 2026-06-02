set(WINDOW_SOURCES
    src/Input.cpp
    src/Window.cpp
    src/MonitorInfo.cpp
)

if(WIN32)
    list(APPEND WINDOW_SOURCES src/Win32/NativeUtils_Win.cpp)
elseif(UNIX AND NOT APPLE)
    list(APPEND WINDOW_SOURCES src/Linux/NativeUtils_Linux.cpp)
endif()

set(WINDOW_HEADERS
    include/ButtonCodes.h
    include/Input.h
    include/KeyCodes.h
    include/MonitorInfo.h
    include/Window.h
    include/WindowCoreAPI.h
    include/WindowEventRegistry.h
    include/WindowEvents.h
    include/WindowsService.h
)

source_group(
    TREE ${CMAKE_CURRENT_SOURCE_DIR}
    FILES ${WINDOW_SOURCES} ${WINDOW_HEADERS}
)

add_library(Window STATIC
    ${WINDOW_SOURCES}
    ${WINDOW_HEADERS}
)

set_target_properties(Window PROPERTIES
    FOLDER "Engine"
)

target_include_directories(Window PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include/"
)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(Window PRIVATE glfw)

target_link_libraries(Window PUBLIC Core)

if(WIN32)
    target_link_libraries(Window PRIVATE ole32)
endif()

target_compile_definitions(Window PRIVATE
    TWISTED_SHARED_LIB
    TWISTED_ENGINE
    $<$<CONFIG:Debug>:TWISTED_DEBUG>
    $<$<CONFIG:RelWithDebInfo>:TWISTED_RELEASE>
    $<$<CONFIG:Release>:TWISTED_DEPLOY>
)

target_compile_options(Window PRIVATE
    -Wall
    -Wextra
    -Wno-c++98-compat
    -Wno-c++98-compat-pedantic
    -Wno-pre-c++14-compat
    -Wno-pre-c++17-compat
    -Wno-global-constructors
    -Wno-exit-time-destructors
    -Wno-unsafe-buffer-usage
    -Wno-old-style-cast
    -Wno-sign-conversion
    -Wno-cast-function-type-strict
    -Wno-implicit-int-float-conversion
    -Wno-missing-field-initializers
    -Wno-missing-braces
    -Wno-nonportable-system-include-path
    -Wno-float-equal
    -Wno-shadow-field-in-constructor
    -Wno-switch-default
    -Wno-double-promotion
    -Wno-deprecated-declarations
)

if(TWISTED_WERROR)
    target_compile_options(Window PRIVATE -Werror)
endif()
