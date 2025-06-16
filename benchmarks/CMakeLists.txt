cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(This Benchmarks)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

file(GLOB Sources
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_executable(${This} ${Sources})

find_package(Qt6 COMPONENTS Core Gui Widgets Charts Svg REQUIRED)

target_include_directories(${This} PUBLIC "${PROJECT_SOURCE_DIR}/include")

# Link Qt6::Widgets explicitly for QApplication
target_link_libraries(${This} PUBLIC benchmark::benchmark_main Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Charts Qt6::Svg)

if(LIB_EXISTS)
    target_link_libraries(${This} PUBLIC lib)
endif()


add_test(NAME ${This} COMMAND ${This} --benchmark_out_format=json --benchmark_out=${PROJECT_SOURCE_DIR}/test_results.json --benchmark_format=console --benchmark_time_unit=s --benchmark_color=true)
