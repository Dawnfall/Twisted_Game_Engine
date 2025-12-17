set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

include(FetchContent)

FetchContent_Declare(
    googletests
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

FetchContent_MakeAvailable(googletests)

add_executable(ShaderProcTests
    src/test.cpp
)

target_link_libraries(ShaderProcTests
    PRIVATE
    ShaderProc
    GTest::gtest
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(ShaderProcTests)
