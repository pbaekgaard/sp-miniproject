cmake_minimum_required(VERSION 3.12)
project(sp-miniproject)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# set(CMAKE_AUTORCC ON)
# set(CMAKE_AUTOMOC ON)
# set(CMAKE_AUTOUIC ON)

include(CTest)
include(cmake/benchmark.cmake)
include(cmake/gtest.cmake)
enable_testing()

# Collect source files except main.cpp
file(GLOB_RECURSE AllSources CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/src/*.cpp")
list(FILTER AllSources EXCLUDE REGEX ".*/main\\.cpp$")

find_package(Qt6 COMPONENTS Core Gui Widgets Charts Svg REQUIRED)

# Initialize LIB_EXISTS to false
set(LIB_EXISTS FALSE)

if(AllSources)
    add_library(lib STATIC ${AllSources})
    set_target_properties(lib PROPERTIES AUTOMOC ON AUTOUIC ON AUTORCC ON)

    target_include_directories(lib PUBLIC "${PROJECT_SOURCE_DIR}/include")

    # Link Qt to the library target too
    target_link_libraries(lib PUBLIC Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Charts Qt6::Svg )

    option(ENABLE_STRICT "Enable strict compilation warnings" OFF)
    if(ENABLE_STRICT AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(lib PRIVATE -Wall -Wextra -Wpedantic -Werror)
    endif()

    add_executable(${PROJECT_NAME} src/main.cpp)
    target_link_libraries(${PROJECT_NAME} PRIVATE lib)

    set(LIB_EXISTS TRUE)
else()
    add_executable(${PROJECT_NAME} src/main.cpp)
endif()

# Export LIB_EXISTS as a cache variable to pass to subdirectories
set(LIB_EXISTS ${LIB_EXISTS} CACHE INTERNAL "Does the lib target exist")


# add_subdirectory(extern/googletest)
add_subdirectory(tests)

add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})


set(COMPILE_COMMANDS_LINK "${CMAKE_SOURCE_DIR}/compile_commands.json")
set(COMPILE_COMMANDS_IN_BUILD "${CMAKE_BINARY_DIR}/compile_commands.json")

set_target_properties(${PROJECT_NAME} PROPERTIES AUTOMOC ON AUTOUIC ON AUTORCC ON)

add_custom_target(
        symlink_compile_commands ALL
        COMMAND ${CMAKE_COMMAND} -E remove -f "${COMPILE_COMMANDS_LINK}"
        COMMAND ${CMAKE_COMMAND} -E create_symlink "${COMPILE_COMMANDS_IN_BUILD}" "${COMPILE_COMMANDS_LINK}"
        COMMENT "Creating symlink for compile_commands.json in source root"
)
