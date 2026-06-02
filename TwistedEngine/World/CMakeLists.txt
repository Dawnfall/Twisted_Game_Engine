set(WORLD_SOURCES
    src/Components/CTransform.cpp
    src/Entity.cpp
    src/GameService.cpp
    src/Managers/CameraManager.cpp
    src/RenderConvert.cpp
    src/World.cpp
    src/WorldRegistry.cpp
)

set(WORLD_HEADERS
    include/AComponent.h
    include/Entity.h
    include/GameService.h
    include/ManagerBase.h
    include/SystemBase.h
    include/World.h
    include/WorldRegistry.h
    include/Components/CCamera.h
    include/Components/CName.h
    include/Components/CRenderer.h
    include/Components/CTransform.h
    include/Components/DirectionalLightComponent.h
    include/RenderConvert.h
    include/Components/PointLightComponent.h
    include/Components/SpotLightComponent.h
    include/Managers/CameraManager.h
    include/Managers/RootTransformManager.h
)

source_group(
    TREE ${CMAKE_CURRENT_SOURCE_DIR}
    FILES ${WORLD_SOURCES} ${WORLD_HEADERS}
)

add_library(World STATIC
    ${WORLD_SOURCES}
    ${WORLD_HEADERS}
)

set_target_properties(World PROPERTIES
    FOLDER "Engine"
)

target_include_directories(World PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Assets/include/"
)

twisted_gen_target(
    TARGET       World
    INCLUDE_DIR  "${CMAKE_CURRENT_SOURCE_DIR}/include"
    GEN_DIR      "${CMAKE_BINARY_DIR}/Generated/World"
    INCLUDE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

find_package(EnTT CONFIG REQUIRED)
find_package(magic_enum CONFIG REQUIRED)

target_link_libraries(World PRIVATE
    EnTT::EnTT
    Core
    Renderer
)

target_link_libraries(World PUBLIC
    magic_enum::magic_enum
)

target_compile_definitions(World PRIVATE
    TWISTED_ENGINE
    $<$<CONFIG:Debug>:TWISTED_DEBUG>
    $<$<CONFIG:RelWithDebInfo>:TWISTED_RELEASE>
    $<$<CONFIG:Release>:TWISTED_DEPLOY>
)

target_compile_options(World PRIVATE
    -Wall
    -Wextra
    -Wno-c++98-compat
    -Wno-c++98-compat-pedantic
    -Wno-pre-c++14-compat
    -Wno-pre-c++17-compat
    -Wno-unsafe-buffer-usage
    -Wno-sign-conversion
    -Wno-old-style-cast
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
    -Wno-global-constructors
    -Wno-exit-time-destructors
    -Wno-shadow-field
)

if(TWISTED_WERROR)
    target_compile_options(World PRIVATE -Werror)
endif()
