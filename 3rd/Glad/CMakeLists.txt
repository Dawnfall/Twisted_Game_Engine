cmake_minimum_required(VERSION 3.40)

# Create a static library named "Glad"
add_library(Glad STATIC
    src/glad.c
    include/glad/glad.h
    include/KHR/khrplatform.h
)

# Specify the include directory for the library
target_include_directories(Glad PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Set language to C
set_target_properties(Glad PROPERTIES
    LINKER_LANGUAGE C
)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# Set output directories
set_target_properties(Glad PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/Glad"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/Glad"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/Glad"
)

# Optional: Add definitions/settings per configuration
target_compile_definitions(Glad PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:Release>:NDEBUG>
)

# Platform-specific settings (optional in this case)
#if(WIN32)
    # Example if you want to set Windows SDK version or platform-specific settings
    # Not necessary unless you're targeting something specific
#endif()