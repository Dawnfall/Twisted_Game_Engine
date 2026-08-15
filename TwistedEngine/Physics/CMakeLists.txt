set(PHYSICS_SOURCES
    src/PhysicsInit.cpp
    src/PhysicsService.cpp
    src/PhysicsSystem.cpp
    src/Colliders/ColliderComponent.cpp
)

set(PHYSICS_HEADERS
    include/PhysicsInit.h
    include/PhysicsService.h
    include/PhysicsSystem.h
    include/Colliders/ColliderComponent.h
)

source_group(
    TREE ${CMAKE_CURRENT_SOURCE_DIR}
    FILES ${PHYSICS_SOURCES} ${PHYSICS_HEADERS}
)

add_library(Physics STATIC
    ${PHYSICS_SOURCES}
    ${PHYSICS_HEADERS}
)

set_target_properties(Physics PROPERTIES
    FOLDER "Engine"
)

target_include_directories(Physics PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

twisted_gen_target(
    TARGET       Physics
    INCLUDE_DIR  "${CMAKE_CURRENT_SOURCE_DIR}/include"
    GEN_DIR      "${CMAKE_BINARY_DIR}/Generated/Physics"
    INCLUDE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

find_package(Jolt CONFIG REQUIRED)

target_link_libraries(Physics PUBLIC
    Jolt::Jolt
    Core
    World
)

target_compile_definitions(Physics PRIVATE
    TWISTED_ENGINE
    $<$<CONFIG:Debug>:TWISTED_DEBUG>
    $<$<CONFIG:RelWithDebInfo>:TWISTED_RELEASE>
    $<$<CONFIG:Release>:TWISTED_DEPLOY>
)

target_compile_options(Physics PRIVATE
    -Wall
    -Wextra
    -Wno-c++98-compat
    -Wno-c++98-compat-pedantic
    -Wno-pre-c++14-compat
    -Wno-pre-c++17-compat
    -Wno-old-style-cast
    -Wno-sign-conversion
    -Wno-unsafe-buffer-usage
    -Wno-missing-field-initializers
    -Wno-shadow-field-in-constructor
    -Wno-double-promotion
    -Wno-float-equal
    -Wno-switch-default
    -Wno-global-constructors
    -Wno-exit-time-destructors
)

if(TWISTED_WERROR)
    target_compile_options(Physics PRIVATE -Werror)
endif()

