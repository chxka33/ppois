# Multiset использует Set::Element, поэтому линкуется с set_lib из ../Set
add_library(multiset_lib src/multiset.cpp)
target_include_directories(multiset_lib PUBLIC include)
target_link_libraries(multiset_lib PUBLIC set_lib)

add_executable(multiset_app main.cpp)
target_link_libraries(multiset_app PRIVATE multiset_lib)

add_executable(multiset_tests tests/test_multiset.cpp)
target_link_libraries(multiset_tests PRIVATE multiset_lib Catch2::Catch2WithMain)
catch_discover_tests(multiset_tests)