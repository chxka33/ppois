cmake_minimum_required(VERSION 3.14)
project(SetProject CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(ENABLE_COVERAGE "Build with coverage instrumentation" OFF)

include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.5.2
)

FetchContent_MakeAvailable(Catch2)

# Исполняемый файл (Main)
add_executable(set_app
    main.cpp
    src/set.cpp
)
target_include_directories(set_app PRIVATE include)

# Тесты
add_executable(set_tests
    tests/test_set.cpp
    src/set.cpp
)
target_include_directories(set_tests PRIVATE include)
target_link_libraries(set_tests PRIVATE Catch2::Catch2WithMain)

include(Catch)
catch_discover_tests(set_tests)

# Коверидж (покрытие тестами)
if(ENABLE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    foreach(target set_app set_tests)
        target_compile_options(${target} PRIVATE --coverage -g -O0)
        target_link_options(${target} PRIVATE --coverage)
    endforeach()
endif()

enable_testing()
add_test(NAME set_unit_tests COMMAND set_tests)