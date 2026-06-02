set(SHADER_PARSER_SOURCES
    src/ShaderParser.cpp
    src/ShaderFile.cpp
    src/SPReflection.cpp
    src/ShaderData.cpp
)

set(SHADER_PARSER_HEADERS
    src/Utils.h
    include/ShaderParser/API.h
    include/ShaderParser/ShaderParser.h
    include/ShaderParser/ShaderFile.h
    include/ShaderParser/SPReflection.h
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
    FILES ${SHADER_PARSER_SOURCES} ${SHADER_PARSER_HEADERS}
)

add_library(ShaderParser STATIC
    ${SHADER_PARSER_SOURCES}
    ${SHADER_PARSER_HEADERS}
)

set_target_properties(ShaderParser PROPERTIES
    FOLDER "Engine"
)

target_include_directories(ShaderParser PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_compile_definitions(ShaderParser PRIVATE
    SHADER_PARSER_BUILD
    $<$<CONFIG:Debug>:TWISTED_DEBUG>
    $<$<CONFIG:RelWithDebInfo>:TWISTED_RELEASE>
    $<$<CONFIG:Release>:TWISTED_DEPLOY>
)

target_compile_options(ShaderParser PRIVATE
    -Wall
    -Wextra
    -Wno-c++98-compat
    -Wno-c++98-compat-pedantic
    -Wno-pre-c++14-compat
    -Wno-pre-c++17-compat
    -Wno-global-constructors
    -Wno-exit-time-destructors
    -Wno-sign-conversion
    -Wno-old-style-cast
    -Wno-unsafe-buffer-usage
    -Wno-float-equal
    -Wno-implicit-int-float-conversion
    -Wno-shadow-field-in-constructor
    -Wno-missing-field-initializers
    -Wno-cast-function-type-strict
    -Wno-nonportable-system-include-path
    -Wno-switch-default
    -Wno-double-promotion
    -Wno-deprecated-declarations
    -Wno-missing-braces
)
