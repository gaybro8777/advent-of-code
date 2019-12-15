add_library(utils OBJECT intcode.cpp coord.cpp)
target_compile_features(utils PUBLIC cxx_std_17)
target_include_directories(utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(driver)
target_sources(driver PRIVATE main.cpp)
target_link_libraries(driver PRIVATE
    day2
    day3
    day4
    day5
    day6
    day7
    day8
    day9
    day10
    day11
    day12
    day13
    day14
    day15)
