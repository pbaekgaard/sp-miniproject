cmake_minimum_required(VERSION 3.12)

set(This Tests)

file(GLOB_RECURSE Sources "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_executable(${This} ${Sources})

target_include_directories(${This} PUBLIC "${PROJECT_SOURCE_DIR}/include")

target_link_libraries(${This} PUBLIC gtest_main)

if(LIB_EXISTS)
    target_link_libraries(${This} PUBLIC lib)
endif()

add_test(NAME ${This} COMMAND ${This})
