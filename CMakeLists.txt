cmake_minimum_required(VERSION 3.23)
project(tape)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include_directories(include)
file(GLOB SRC_FILES src/*.cpp)
add_executable(tape ${SRC_FILES})

enable_testing()

add_executable(
        unit_tests
        tests/test.cpp src/tape.cpp src/tape_sorter.cpp)

target_link_libraries(
        unit_tests
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(unit_tests)


