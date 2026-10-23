# CMake generated Testfile for 
# Source directory: /Users/chxkq/Desktop/pois/ppois/lab1/Set
# Build directory: /Users/chxkq/Desktop/pois/ppois/lab1/Set/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
include("/Users/chxkq/Desktop/pois/ppois/lab1/Set/build/set_tests-b12d07c_include.cmake")
add_test(set_unit_tests "/Users/chxkq/Desktop/pois/ppois/lab1/Set/build/set_tests")
set_tests_properties(set_unit_tests PROPERTIES  _BACKTRACE_TRIPLES "/Users/chxkq/Desktop/pois/ppois/lab1/Set/CMakeLists.txt;46;add_test;/Users/chxkq/Desktop/pois/ppois/lab1/Set/CMakeLists.txt;0;")
subdirs("_deps/catch2-build")
