# CMake generated Testfile for 
# Source directory: /home/pbk/school/SP/sp-miniproject
# Build directory: /home/pbk/school/SP/sp-miniproject/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(sp-miniproject "/home/pbk/school/SP/sp-miniproject/build/sp-miniproject")
set_tests_properties(sp-miniproject PROPERTIES  _BACKTRACE_TRIPLES "/home/pbk/school/SP/sp-miniproject/CMakeLists.txt;31;add_test;/home/pbk/school/SP/sp-miniproject/CMakeLists.txt;0;")
subdirs("extern/googletest")
subdirs("tests")
