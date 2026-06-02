add_subdirectory(Core)
add_subdirectory(Window)
add_subdirectory(Renderer)
add_subdirectory(World)
add_subdirectory(Assets)
add_subdirectory(Physics)

add_library(TwistedEngine SHARED
    dllmain.cpp
)

set_target_properties(TwistedEngine PROPERTIES
    FOLDER "Engine"
)

target_include_directories(TwistedEngine PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/Core/include/"
    "${CMAKE_CURRENT_SOURCE_DIR}/Window/include/"
    "${CMAKE_CURRENT_SOURCE_DIR}/Assets/include/"
    "${CMAKE_CURRENT_SOURCE_DIR}/World/include/"
    "${CMAKE_CURRENT_SOURCE_DIR}/Renderer/include/"
    "${CMAKE_CURRENT_SOURCE_DIR}/Physics/include/"
    "${CMAKE_SOURCE_DIR}/ShaderParser/include"
)

target_link_libraries(TwistedEngine PRIVATE
    $<LINK_LIBRARY:WHOLE_ARCHIVE,Core>
    $<LINK_LIBRARY:WHOLE_ARCHIVE,Window>
    $<LINK_LIBRARY:WHOLE_ARCHIVE,Renderer>
    $<LINK_LIBRARY:WHOLE_ARCHIVE,World>
    $<LINK_LIBRARY:WHOLE_ARCHIVE,Assets>
    $<LINK_LIBRARY:WHOLE_ARCHIVE,Physics>
    $<LINK_LIBRARY:WHOLE_ARCHIVE,ShaderParser>
)

target_compile_definitions(TwistedEngine PRIVATE
    TWISTED_ENGINE
    $<$<CONFIG:Debug>:TWISTED_DEBUG>
    $<$<CONFIG:RelWithDebInfo>:TWISTED_RELEASE>
    $<$<CONFIG:Release>:TWISTED_DEPLOY>
)

target_compile_options(TwistedEngine PRIVATE
    -Wall
    -Wextra
    -Wno-c++98-compat
    -Wno-c++98-compat-pedantic
    -Wno-pre-c++14-compat
    -Wno-pre-c++17-compat
)

if(TWISTED_WERROR)
    target_compile_options(TwistedEngine PRIVATE -Werror)
endif()
