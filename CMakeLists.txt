cmake_minimum_required(VERSION 3.10)
project(Laba5_OOP LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.15.0
)
FetchContent_MakeAvailable(googletest)

set(MAIN_SOURCE
    src/main.cpp
    src/custom_memory_resource.cpp
)

set(LIB_SOURCES
    src/custom_memory_resource.cpp
)

add_executable(Laba5_OOP ${MAIN_SOURCE} ${LIB_SOURCES})
target_include_directories(Laba5_OOP PRIVATE include)
target_compile_features(Laba5_OOP PRIVATE cxx_std_17)

set(TEST_SOURCES
    tests/test_queue.cpp
    tests/test_main.cpp
    ${LIB_SOURCES}
)

add_executable(run_tests ${TEST_SOURCES})
target_include_directories(run_tests PRIVATE include)
target_compile_features(run_tests PRIVATE cxx_std_17)
target_link_libraries(run_tests PRIVATE gtest_main)

enable_testing()
add_test(NAME QueueTests COMMAND run_tests)