set(EDITOR_SOURCES
    main.cpp
    include/EditorConstants.h
    include/SceneInstantiator.h
    include/EditorApp/EditorRegistry.h
    include/EditorApp/EditorService.h
    include/EditorData/EditorConfig.h
    include/EditorData/Selection.h
    include/EditorWorld/EditorCameraSystem.h
    include/Test/PlayerComponent.h
    include/Test/PlayerSystem.h
    include/Test/SunComponent.h
    include/Test/SunSystem.h
    include/UI/ComponentPainter.h
    include/UI/EditorPanel.h
    include/UI/GenericComponentPainter.h
    include/UI/ImguiExtensions.h
    include/UI/UIWindow.h
    include/UI/Components/CCameraPainter.h
    include/UI/Components/CNamePainter.h
    include/UI/Components/CRendererPainter.h
    include/UI/Components/CTransformPainter.h
    include/UI/Components/ColliderPainter.h
    include/UI/Details/EntityPainter.h
    include/UI/Details/MaterialPainter.h
    include/UI/Details/ShaderPainter.h
    include/UI/Panels/AssetsPanel.h
    include/UI/Panels/DetailsPanel.h
    include/UI/Panels/EditorSettingsPanel.h
    include/UI/Panels/GameViewPanel.h
    include/UI/Panels/ProjectSettingsPanel.h
    include/UI/Panels/TreeViewPanel.h
    include/UI/Panels/WorldSystemPanel.h
    include/UI/Panels/WorldViewPanel.h
    src/EditorApp/EditorService.cpp
    src/EditorData/Selection.cpp
    src/EditorWorld/EditorCameraSystem.cpp
    src/Test/PlayerSystem.cpp
    src/Test/SunSystem.cpp
    src/UI/ImguiExtensions.cpp
    src/UI/UIWindow.cpp
    src/UI/Components/CCameraPainter.cpp
    src/UI/Components/CNamePainter.cpp
    src/UI/Components/CRendererPainter.cpp
    src/UI/Components/CTransformPainter.cpp
    src/UI/Components/ColliderPainter.cpp
    src/UI/GenericComponentPainter.cpp
    src/UI/Details/EntityPainter.cpp
    src/UI/Details/MaterialPainter.cpp
    src/UI/Details/ShaderPainter.cpp
    src/UI/Panels/AssetsPanel.cpp
    src/UI/Panels/DetailsPanel.cpp
    src/UI/Panels/EditorSettingsPanel.cpp
    src/UI/Panels/GameViewPanel.cpp
    src/UI/Panels/ProjectSettingsPanel.cpp
    src/UI/Panels/TreeViewPanel.cpp
    src/UI/Panels/WorldSystemPanel.cpp
    src/UI/Panels/WorldViewPanel.cpp
)

list(APPEND EDITOR_SOURCES src/UI/ImguiExtensions_GLFW.cpp src/UI/ImguiExtensions_Vulkan.cpp)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${EDITOR_SOURCES})

add_executable(Editor ${EDITOR_SOURCES})

set_target_properties(Editor PROPERTIES
    FOLDER "Editor"
)

target_include_directories(Editor PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

twisted_gen_target(
    TARGET       Editor
    INCLUDE_DIR  "${CMAKE_CURRENT_SOURCE_DIR}/include"
    GEN_DIR      "${CMAKE_BINARY_DIR}/Generated/Editor"
    INCLUDE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

find_package(glm CONFIG REQUIRED)
find_package(yaml-cpp CONFIG REQUIRED)

target_link_libraries(Editor PRIVATE
    TwistedEngine
    glm::glm
    imgui
    imguizmo
    yaml-cpp::yaml-cpp
)

find_package(Vulkan REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(Editor PRIVATE Vulkan::Vulkan glfw)

target_compile_definitions(Editor PRIVATE
    PROJECT_ROOT_PATH="${CMAKE_SOURCE_DIR}"
    TWISTED_SHARED_LIB
    $<$<CONFIG:Debug>:TWISTED_DEBUG>
    $<$<CONFIG:RelWithDebInfo>:TWISTED_RELEASE>
    $<$<CONFIG:Release>:TWISTED_DEPLOY>
)

target_compile_options(Editor PRIVATE
    -Wall
    -Wextra
    -Wno-c++98-compat
    -Wno-c++98-compat-pedantic
    -Wno-pre-c++14-compat
    -Wno-pre-c++17-compat
    -Wno-float-equal
    -Wno-global-constructors
    -Wno-exit-time-destructors
    -Wno-unsafe-buffer-usage
    -Wno-old-style-cast
    -Wno-deprecated-declarations
    -Wno-implicit-int-float-conversion
    -Wno-sign-conversion
    -Wno-shadow-field-in-constructor
    -Wno-missing-field-initializers
    -Wno-cast-function-type-strict
    -Wno-nonportable-system-include-path
    -Wno-switch-default
    -Wno-switch-enum
    -Wno-double-promotion
    -Wno-missing-braces
    -Wno-shadow-field
    -Wno-newline-eof
    -Wno-unused-private-field
    -Wno-missing-variable-declarations
    -Wno-missing-prototypes
    -Wno-zero-as-null-pointer-constant
)

if(TWISTED_WERROR)
    target_compile_options(Editor PRIVATE -Werror)
endif()
