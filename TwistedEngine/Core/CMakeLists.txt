set(CORE_SOURCES
    src/Application/Application.cpp
    src/Application/TObject.cpp
    src/Debug/Logger.cpp
    src/Serialization/BinSerializer.cpp
    src/Utils/FileUtils.cpp
    src/Utils/GlmUtils.cpp
    src/Utils/Utils.cpp
    src/Utils/YamlUtils.cpp
)


set(CORE_HEADERS
    include/Application/Application.h
    include/Application/Service.h
    include/Application/TimeService.h
    include/Application/ObjectID.h
    include/Application/TObject.h
    include/AppCore.h
    include/Constants.h
    include/Debug/Logger.h
        include/Utils/BoundedQueue.h
    include/Utils/Event.h
    include/Utils/FileUtils.h
    include/Utils/GlmUtils.h
    include/Utils/ThreadSafeQueue.h
    include/Utils/Utils.h
    include/Utils/WPtr.h
    include/Utils/WPtrBase.h
    include/Utils/YamlUtils.h
    include/Serialization/BinSerializer.h
    include/Serialization/SerializedBuffer.h
)

source_group(
    TREE ${CMAKE_CURRENT_SOURCE_DIR}
    FILES ${CORE_SOURCES} ${CORE_HEADERS}
)

add_library(Core STATIC
    ${CORE_SOURCES}
    ${CORE_HEADERS}
)

set_target_properties(Core PROPERTIES
    FOLDER "Engine"
)

target_include_directories(Core PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

find_package(glm CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(yaml-cpp CONFIG REQUIRED)

target_link_libraries(Core
    PUBLIC
        glm::glm
        yaml-cpp::yaml-cpp
    PRIVATE
        spdlog::spdlog
)

target_compile_definitions(Core PUBLIC
    TWISTED_SHARED_LIB
    $<$<CONFIG:Debug>:TWISTED_DEBUG>
    $<$<CONFIG:RelWithDebInfo>:TWISTED_RELEASE>
    $<$<CONFIG:Release>:TWISTED_DEPLOY>
)

target_compile_definitions(Core PRIVATE
    TWISTED_ENGINE
)

target_compile_options(Core PRIVATE
    -Wall
    -Wextra
    -Wno-c++98-compat
    -Wno-c++98-compat-pedantic
    -Wno-pre-c++14-compat
    -Wno-pre-c++17-compat
    -Wno-global-constructors
    -Wno-exit-time-destructors
    -Wno-unsafe-buffer-usage
    -Wno-deprecated-declarations
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
    -Wno-missing-braces
)

if(TWISTED_WERROR)
    target_compile_options(Core PRIVATE -Werror)
endif()
