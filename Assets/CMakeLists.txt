cmake_minimum_required(VERSION 3.28)

set(ASSETS_SOURCES
    src/AssetEncoders.cpp
    src/AssetImporterRegistry.cpp
    src/ImportedAsset.cpp
    src/AssetUuid.cpp
    src/AssetsService.cpp
    src/Project.cpp
    src/ProjectConfig.cpp
    src/Importers/MaterialImpoter.cpp
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
    include/Project.h
    include/ProjectConfig.h
    include/Importers/MaterialImporter.h
    include/Importers/ModelImporter.h
    include/Importers/ShaderImporter.h
    include/Importers/TextureImporter.h
    include/Importers/WorldImporter.h
)

#source_group(
#    TREE ${CMAKE_CURRENT_SOURCE_DIR}
#    FILES ${ASSETS_SOURCES} ${ASSETS_HEADERS}
#)


# ------------------------------
# Create static library
# ------------------------------

add_library(Assets STATIC 
    ${ASSETS_SOURCES} 
    ${ASSETS_HEADERS}
)

set_target_properties(Assets PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    POSITION_INDEPENDENT_CODE ON
)

target_include_directories(Assets PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_SOURCE_DIR}/Core/include/"
    "${CMAKE_SOURCE_DIR}/World/include/"
    "${CMAKE_SOURCE_DIR}/Renderer/include/"
    "${CMAKE_SOURCE_DIR}/ShaderParser/include"
)

find_package(EnTT CONFIG REQUIRED)

target_link_libraries(Assets PRIVATE
    EnTT::EnTT
    Core
    World
    Renderer
    ShaderParser
    assimp::assimp
)

# ------------------------------
# Compile definitions
# ------------------------------

target_compile_definitions(Assets PRIVATE
    TWISTED_ENGINE
    $<$<CONFIG:Debug>:TWISTED_DEBUG>
    $<$<CONFIG:RelWithDebInfo>:TWISTED_RELEASE>
    $<$<CONFIG:Release>:TWISTED_DEPLOY>
)

#if(MSVC)
#  target_compile_definitions(TwistedEngine PRIVATE 
#    _CRT_SECURE_NO_WARNINGS)
#endif()

# ------------------------------
# Compiler options
# ------------------------------
option(TWISTED_WERROR "Treat warnings as errors" OFF)

if(MSVC)
  target_compile_options(Assets PRIVATE
    /W4
    /wd4251
    /external:W0
    /experimental:external
    /analyze-
  )
  if(TWISTED_WERROR)
    target_compile_options(Assets PRIVATE /WX)
  endif()
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
  target_compile_options(Assets PRIVATE
    -Wall
    -Wextra
    -Wpedantic
  )
  if(TWISTED_WERROR)
    target_compile_options(Assets PRIVATE -Werror)
  endif()
endif()