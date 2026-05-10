cmake_minimum_required(VERSION 3.28)

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


# ------------------------------
# Create static library
# ------------------------------

add_library(World STATIC 
    ${WORLD_SOURCES} 
    ${WORLD_HEADERS}
)

set_target_properties(World PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    POSITION_INDEPENDENT_CODE ON
)

target_include_directories(World PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_SOURCE_DIR}/Core/include/"
    "${CMAKE_SOURCE_DIR}/Assets/include/"
    "${CMAKE_SOURCE_DIR}/Renderer/include/"
)

find_package(EnTT CONFIG REQUIRED)

target_link_libraries(World PRIVATE
    EnTT::EnTT
    Core
    Renderer
)

# ------------------------------
# Compile definitions
# ------------------------------

target_compile_definitions(World PRIVATE
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
  target_compile_options(World PRIVATE
    /W4
    /wd4251
    /external:W0
    /experimental:external
    /analyze-
  )
  if(TWISTED_WERROR)
    target_compile_options(World PRIVATE /WX)
  endif()
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
  target_compile_options(World PRIVATE
    -Wall
    -Wextra
    -Wpedantic
  )
  if(TWISTED_WERROR)
    target_compile_options(World PRIVATE -Werror)
  endif()
endif()