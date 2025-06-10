add_library(Core STATIC)

set_target_properties(Core PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

set_property(TARGET Core PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded&<&<CONFIG:Debug>:Debug>DLL")

set_target_properties(Core PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${outputDir}/$<TARGET_NAME>_static"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${outputDir}/$<TARGET_NAME>_exe"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${outputDir}/$<TARGET_NAME>_dynamic"
)

file(GLOB_RECURSE CORE_SOURCES
    "src/*.h"
    "src/*.cpp"
)
target_sources(Core PRIVATE ${CORE_SOURCES})

target_include_directories(Core PUBLIC
    ${IncludeDirs_Core}
    ${IncludeDirs_spdlog}
    ${IncludeDirs_GLM}
    #${IncludeDirs_stbi_image}
    #${IncludeDirs_rapidobj}
)

target_compile_definitions(Core PRIVATE
    TWISTED_ENGINE
)

target_compile_definitions(Core PRIVATE
    "$<$<CONFIG:Debug>:TWISTED_DEBUG>"
    "$<$<CONFIG:Release>:TWISTED_RELEASE>"
)

target_compile_options(Core PRIVATE
    "$<$<CONFIG:Debug>:/Zi>"      # Enable debug info (MSVC)
    "$<$<CONFIG:Release>:/O2>"    # Optimize for speed (MSVC)
)