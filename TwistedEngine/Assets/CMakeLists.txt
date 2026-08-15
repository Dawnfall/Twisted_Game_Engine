set(ASSETS_SOURCES
    src/AssetEncoders.cpp
    src/AssetImporterRegistry.cpp
    src/ImportedAsset.cpp
    src/AssetUuid.cpp
    src/AssetsService.cpp
    src/FolderWatcher.cpp
    src/Project.cpp
    src/ProjectConfig.cpp
    src/Importers/MaterialImporter.cpp
    src/Importers/ModelImporter.cpp
    src/Importers/ShaderImporter.cpp
    src/Importers/TextureImporter.cpp
    src/Importers/WorldImporter.cpp
)

set(ASSETS_HEADERS
    include/AssetEncoder.h
    include/AssetImporter.h
    include/AssetImporterRegistry.h
    include/ImportedAsset.h
    include/ModelAsset.h
    include/AssetUuid.h
    include/AssetUuidYaml.h
    include/AssetsService.h
    include/FolderWatcher.h
    include/Project.h
    include/ProjectConfig.h
    include/Importers/MaterialImporter.h
    include/Importers/ModelImporter.h
    include/Importers/ShaderImporter.h
    include/Importers/TextureImporter.h
    include/Importers/WorldImporter.h
)

source_group(
    TREE ${CMAKE_CURRENT_SOURCE_DIR}
    FILES ${ASSETS_SOURCES} ${ASSETS_HEADERS}
)

add_library(Assets STATIC
    ${ASSETS_SOURCES}
    ${ASSETS_HEADERS}
)

set_target_properties(Assets PROPERTIES
    FOLDER "Engine"
)

target_include_directories(Assets PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

find_package(EnTT CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)

target_link_libraries(Assets PRIVATE
    EnTT::EnTT
    Core
    Window
    World
    Renderer
    ShaderParser
    assimp::assimp
)

target_compile_definitions(Assets PRIVATE
    TWISTED_ENGINE
    $<$<CONFIG:Debug>:TWISTED_DEBUG>
    $<$<CONFIG:RelWithDebInfo>:TWISTED_RELEASE>
    $<$<CONFIG:Release>:TWISTED_DEPLOY>
)

target_compile_options(Assets PRIVATE
    -Wall
    -Wextra
    -Wno-c++98-compat
    -Wno-c++98-compat-pedantic
    -Wno-pre-c++14-compat
    -Wno-pre-c++17-compat
    -Wno-exit-time-destructors
    -Wno-global-constructors
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

if(TWISTED_WERROR)
    target_compile_options(Assets PRIVATE -Werror)
endif()
