cmake_minimum_required(VERSION 3.28)

# ------------------------------
# Source files
# ------------------------------

set(SHADER_PARSER_SOURCES
    src/ShaderParser.cpp
    src/ShaderFile.cpp
    src/ShaderReflection.cpp
    src/ShaderData.cpp
    src/Utils.h
)

set(SHADER_PARSER_HEADERS
    include/ShaderParser/API.h
    include/ShaderParser/ShaderParser.h
    include/ShaderParser/ShaderFile.h
    include/ShaderParser/ShaderReflection.h
    include/ShaderParser/ShaderData.h
    include/ShaderParser/Imports/BuiltInImports.h
    include/ShaderParser/Imports/Lights_Core.h
    include/ShaderParser/Imports/MVP_Core.h
    include/ShaderParser/Shaders/Default_Shader.h
    include/ShaderParser/Shaders/Debug_Normal_Shader.h
    include/ShaderParser/Shaders/Debug_UV_Shader.h
)

source_group(
    TREE ${CMAKE_CURRENT_SOURCE_DIR}
    FILES ${SHADER_PARSER_SOURCES} ${SHADER_PARSER_HEADERS})

# ------------------------------
# Create shared library
# ------------------------------

add_library(ShaderParser SHARED
    ${SHADER_PARSER_SOURCES}
    ${SHADER_PARSER_HEADERS}
)

set_target_properties(ShaderParser PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    POSITION_INDEPENDENT_CODE ON
    FOLDER "Engine"
)

# ------------------------------
# Include directories
# ------------------------------

target_include_directories(ShaderParser PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

# ------------------------------
# Compile definitions
# ------------------------------

target_compile_definitions(ShaderParser PRIVATE
    SHADER_PARSER_BUILD
    $<$<CONFIG:Debug>:TWISTED_DEBUG>
    $<$<CONFIG:RelWithDebInfo>:TWISTED_RELEASE>
    $<$<CONFIG:Release>:TWISTED_DEPLOY>
)

if(MSVC)
    target_compile_definitions(ShaderParser PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

# ------------------------------
# Compiler options
# ------------------------------

if(MSVC)
    target_compile_options(ShaderParser PRIVATE
        /W4
        /wd4251
        /external:W0
        /experimental:external
        /analyze-
    )
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(ShaderParser PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
endif()
