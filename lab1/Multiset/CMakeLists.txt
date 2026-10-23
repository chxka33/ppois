cmake_minimum_required(VERSION 3.14)
project(MultisetProject CXX)

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
add_executable(multiset_app
    main.cpp
    src/multiset.cpp
    ../Set/src/set.cpp
)
# Добавляем пути: свой include и include соседнего проекта Set
target_include_directories(multiset_app PRIVATE 
    include 
    "${CMAKE_CURRENT_SOURCE_DIR}/../Set/include"
)

# Тесты
add_executable(multiset_tests
    tests/test_multiset.cpp
    src/multiset.cpp
    ../Set/src/set.cpp
)
# Аналогично добавляем пути для тестов
target_include_directories(multiset_tests PRIVATE 
    include 
    "${CMAKE_CURRENT_SOURCE_DIR}/../Set/include"
)
target_link_libraries(multiset_tests PRIVATE Catch2::Catch2WithMain)

include(Catch)
catch_discover_tests(multiset_tests)

# Коверидж (покрытие тестами)
if(ENABLE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    foreach(target multiset_app multiset_tests)
        target_compile_options(${target} PRIVATE --coverage -g -O0)
        target_link_options(${target} PRIVATE --coverage)
    endforeach()
endif()

enable_testing()
add_test(NAME multiset_unit_tests COMMAND multiset_tests)