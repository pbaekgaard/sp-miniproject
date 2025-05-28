cmake_minimum_required(VERSION 3.12)
project(sp-miniproject)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(CTest)
enable_testing()

add_subdirectory(extern/googletest)
add_subdirectory(tests)

# Collect sources
file(GLOB_RECURSE AllSources CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/src/*.cpp")
list(FILTER AllSources EXCLUDE REGEX ".*/main\\.cpp$")

add_library(lib STATIC ${AllSources})
target_include_directories(lib PUBLIC "${PROJECT_SOURCE_DIR}/include")

# Strict warnings (optional)
option(ENABLE_STRICT "Enable strict compilation warnings" ON)
if (ENABLE_STRICT AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(lib PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE lib)

add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
