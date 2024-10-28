cmake_minimum_required(VERSION 3.26)
project(rollercoaster)

enable_testing()

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/release-1.12.1.zip
)
FetchContent_MakeAvailable(googletest)

set(CMAKE_EXE_LINKER_FLAGS "-static")
set(CMAKE_CXX_STANDARD 23)

add_executable(rollercoasterApp general.cpp)

add_executable(rollercoasterTest test.cpp general.cpp)
target_link_libraries(rollercoasterTest gtest gtest_main)
target_compile_definitions(rollercoasterTest PRIVATE TESTING)

add_test(NAME rollercoasterTest COMMAND rollercoasterTest)
