# CMake generated Testfile for 
# Source directory: C:/Tidligere_skrivebord/UNI_SOFTWARE/8_semester/STP/sp-miniproject
# Build directory: C:/Tidligere_skrivebord/UNI_SOFTWARE/8_semester/STP/sp-miniproject/cmake-build-debug
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(sp-miniproject "C:/Tidligere_skrivebord/UNI_SOFTWARE/8_semester/STP/sp-miniproject/cmake-build-debug/sp-miniproject.exe")
set_tests_properties(sp-miniproject PROPERTIES  _BACKTRACE_TRIPLES "C:/Tidligere_skrivebord/UNI_SOFTWARE/8_semester/STP/sp-miniproject/CMakeLists.txt;42;add_test;C:/Tidligere_skrivebord/UNI_SOFTWARE/8_semester/STP/sp-miniproject/CMakeLists.txt;0;")
subdirs("extern/googletest")
subdirs("tests")
