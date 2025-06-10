add_library(Rendering SHARED)

set_target_properties(Rendering PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

set_property(TARGET Rendering PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded&<&<CONFIG:Debug>:Debug>DLL")

set_target_properties(Rendering PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${outputDir}/$<TARGET_NAME>_static"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${outputDir}/$<TARGET_NAME>_exe"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${outputDir}/$<TARGET_NAME>_dynamic"
)

file(GLOB_RECURSE RENDERING_SOURCES
    "src/*.h"
    "src/*.cpp"
)
target_sources(Rendering PRIVATE ${RENDERING_SOURCES})

target_include_directories(Rendering PUBLIC
    ${IncludeDirs_Rendering}
    ${IncludeDirs_Core}
    ${includeDirs_spdlog}
    ${IncludeDirs_Glad}
    ${IncludeDirs_GLM}
)

target_link_libraries(Rendering PRIVATE
    Core
    Glad
)

target_compile_definitions(Rendering PRIVATE
    TWISTED_ENGINE
)

target_compile_definitions(Rendering PRIVATE
    "$<$<CONFIG:Debug>:TWISTED_DEBUG>"
    "$<$<CONFIG:Release>:TWISTED_RELEASE>"
)

target_compile_options(Rendering PRIVATE
    "$<$<CONFIG:Debug>:/Zi>"      # Enable debug info (MSVC)
    "$<$<CONFIG:Release>:/O2>"    # Optimize for speed (MSVC)
)