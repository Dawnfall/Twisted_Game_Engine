cmake_minimum_required(VERSION 3.28)

# ------------------------------
# Source files
# ------------------------------

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

if(WIN32)
    list(APPEND CORE_SOURCES src/Utils/FolderWatcher.cpp)
endif()

# ------------------------------
# Header files
# ------------------------------

set(CORE_HEADERS
    include/Application/Application.h
    include/Application/Service.h
    include/Application/TimeService.h
    include/Application/ObjectID.h
    include/Application/TObject.h
    include/AppCore.h
    include/Constants.h
    include/Debug/Logger.h
    include/Utils/AlgebraUtils.h
    include/Utils/BoundedQueue.h
    include/Utils/Event.h
    include/Utils/FileUtils.h
    include/Utils/FolderWatcher.h
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

# ------------------------------
# Create static library
# ------------------------------

add_library(Core STATIC
    ${CORE_SOURCES}
    ${CORE_HEADERS}
)

set_target_properties(Core PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    POSITION_INDEPENDENT_CODE ON
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

# ------------------------------
# Compile definitions
# ------------------------------

target_compile_definitions(Core PUBLIC
    TWISTED_SHARED_LIB
    $<$<CONFIG:Debug>:TWISTED_DEBUG>
    $<$<CONFIG:RelWithDebInfo>:TWISTED_RELEASE>
    $<$<CONFIG:Release>:TWISTED_DEPLOY>
)

target_compile_definitions(Core PRIVATE
    TWISTED_ENGINE
)

if(MSVC)
  target_compile_definitions(Core PRIVATE
    _CRT_SECURE_NO_WARNINGS)
endif()

# ------------------------------
# Compiler options
# ------------------------------
option(TWISTED_WERROR "Treat warnings as errors" OFF)

if(MSVC)
  target_compile_options(Core PRIVATE
    /W4
    /wd4251
    /external:anglebrackets
    /external:W0
    /analyze-
  )
  if(TWISTED_WERROR)
    target_compile_options(Core PRIVATE /WX)
  endif()
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
  target_compile_options(Core PRIVATE
    -Wall
    -Wextra
    -Wpedantic
  )
  if(TWISTED_WERROR)
    target_compile_options(Core PRIVATE -Werror)
  endif()
endif()
