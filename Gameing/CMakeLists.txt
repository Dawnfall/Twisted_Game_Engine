add_library(Gameing SHARED)

set_target_properties(Gameing PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

set_property(TARGET Gameing PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded&<&<CONFIG:Debug>:Debug>DLL")

set_target_properties(Gameing PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${outputDir}/$<TARGET_NAME>_static"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${outputDir}/$<TARGET_NAME>_exe"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${outputDir}/$<TARGET_NAME>_dynamic"
)

file(GLOB_RECURSE GAMEING_SOURCES
    "src/*.h"
    "src/*.cpp"
)
target_sources(Gameing PRIVATE ${GAMEING_SOURCES})

target_include_directories(Gameing PUBLIC
    ${IncludeDirs_Gameing}
    ${IncludeDirs_Core}
    ${IncludeDirs_Rendering}
    ${includeDirs_spdlog}
    ${IncludeDirs_TwistedEngine}
    ${IncludeDirs_GLM}
    ${IncludeDirs_EnTT}
)

target_link_libraries(Gameing PRIVATE
    Core
    Rendering
)

target_compile_definitions(Gameing PRIVATE
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