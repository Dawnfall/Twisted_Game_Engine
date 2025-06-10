add_library(Windowing SHARED)

set_target_properties(Windowing PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

set_property(TARGET Windowing PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded&<&<CONFIG:Debug>:Debug>DLL")

set_target_properties(Windowing PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${outputDir}/$<TARGET_NAME>_static"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${outputDir}/$<TARGET_NAME>_exe"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${outputDir}/$<TARGET_NAME>_dynamic"
)

file(GLOB_RECURSE WINDOWING_SOURCES
    "src/*.h"
    "src/*.cpp"
)
target_sources(Windowing PRIVATE ${WINDOWING_SOURCES})

target_include_directories(Windowing PUBLIC
    ${IncludeDirs_Windowing}
    ${IncludeDirs_Core}
    ${includeDirs_spdlog}
    ${IncludeDirs_GLFW}
    ${IncludeDirs_GLM}
    ${IncludeDirs_TwistedEngine}
)

target_link_libraries(Windowing PRIVATE
    Core
    GLFW
)

target_compile_definitions(Windowing PRIVATE
    TWISTED_ENGINE
)

target_compile_definitions(Windowing PRIVATE
    "$<$<CONFIG:Debug>:TWISTED_DEBUG>"
    "$<$<CONFIG:Release>:TWISTED_RELEASE>"
)

target_compile_options(Windowing PRIVATE
    "$<$<CONFIG:Debug>:/Zi>"      # Enable debug info (MSVC)
    "$<$<CONFIG:Release>:/O2>"    # Optimize for speed (MSVC)
)