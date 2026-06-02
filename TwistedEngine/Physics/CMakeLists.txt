set(PHYSICS_SOURCES
    src/PhysicsInit.cpp
    src/PhysicsService.cpp
    src/PhysicsManager.cpp
)

set(PHYSICS_HEADERS
    include/PhysicsInit.h
    include/PhysicsService.h
    include/PhysicsManager.h
    include/Colliders/ColliderComponent.h
    include/Colliders/CSphereCollider.h
    include/Colliders/CBoxCollider.h
    include/Colliders/CCapsuleCollider.h
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
)

if(TWISTED_WERROR)
    target_compile_options(Physics PRIVATE -Werror)
endif()

