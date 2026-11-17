cmake_minimum_required(VERSION 3.14)
project(Lab2 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.5.2
)
FetchContent_MakeAvailable(Catch2)
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)

enable_testing()
include(Catch)

option(ENABLE_COVERAGE "Build with coverage instrumentation" OFF)
    if(ENABLE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        add_compile_options(--coverage -g -O0)
        add_link_options(--coverage)
    endif()

file(GLOB_RECURSE CORE_SOURCES CONFIGURE_DEPENDS "src/*.cpp")
add_library(sports_club ${CORE_SOURCES})
target_include_directories(sports_club PUBLIC include)

add_executable(sports_club_cli main.cpp)
target_link_libraries(sports_club_cli PRIVATE sports_club)

file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS "tests/*.cpp")
add_executable(sports_club_test ${TEST_SOURCES})
target_link_libraries(sports_club_test PRIVATE sports_club Catch2::Catch2WithMain)

catch_discover_tests(sports_club_test)