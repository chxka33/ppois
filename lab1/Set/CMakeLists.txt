add_library(set_lib src/set.cpp)
target_include_directories(set_lib PUBLIC include)

add_executable(set_app main.cpp)
target_link_libraries(set_app PRIVATE set_lib)

add_executable(set_tests tests/test_set.cpp)
target_link_libraries(set_tests PRIVATE set_lib Catch2::Catch2WithMain)
catch_discover_tests(set_tests)