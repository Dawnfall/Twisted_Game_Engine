add_library(TwistedEngine SHARED)

set_target_properties(TwistedEngine PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

set_property(TARGET TwistedEngine PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded&<&<CONFIG:Debug>:Debug>DLL")

set_target_properties(TwistedEngine PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${outputDir}/$<TARGET_NAME>_static"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${outputDir}/$<TARGET_NAME>_exe"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${outputDir}/$<TARGET_NAME>_dynamic"
)

file(GLOB_RECURSE TWISTED_ENGINE_SOURCES
    "src/*.h"
    "src/*.cpp"
)
target_sources(TwistedEngine PRIVATE ${TWISTED_ENGINE_SOURCES})

target_include_directories(TwistedEngine PUBLIC
    ${IncludeDirs_TwistedEngine}
    ${IncludeDirs_Core}
    ${IncludeDirs_Gameing}
    ${IncludeDirs_Rendering}
    ${IncludeDirs_Windowing}
    ${includeDirs_spdlog}
    ${IncludeDirs_EnTT}
    ${IncludeDirs_GLM}
)

target_link_libraries(Gameing PRIVATE
    Core
    Windowing
    Rendering
    Gameing
)

target_compile_definitions(Gameing PRIVATE
    TWISTED_ENGINE
    GLFW_INCLUDE_NONE
    GLFW_EXCLUDE_VULKAN
)

target_compile_definitions(Rendering PRIVATE
    "$<$<CONFIG:Debug>:TWISTED_DEBUG>"
    "$<$<CONFIG:Release>:TWISTED_RELEASE>"
)

target_compile_options(Rendering PRIVATE
    "$<$<CONFIG:Debug>:/Zi>"      # Enable debug info (MSVC)
    "$<$<CONFIG:Release>:/O2>"    # Optimize for speed (MSVC)
)