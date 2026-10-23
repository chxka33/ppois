# CMake generated Testfile for 
# Source directory: /Users/chxkq/Desktop/pois/ppois/lab1/Multiset
# Build directory: /Users/chxkq/Desktop/pois/ppois/lab1/Multiset/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
include("/Users/chxkq/Desktop/pois/ppois/lab1/Multiset/build/multiset_tests-b12d07c_include.cmake")
add_test(multiset_unit_tests "/Users/chxkq/Desktop/pois/ppois/lab1/Multiset/build/multiset_tests")
set_tests_properties(multiset_unit_tests PROPERTIES  _BACKTRACE_TRIPLES "/Users/chxkq/Desktop/pois/ppois/lab1/Multiset/CMakeLists.txt;56;add_test;/Users/chxkq/Desktop/pois/ppois/lab1/Multiset/CMakeLists.txt;0;")
subdirs("_deps/catch2-build")
