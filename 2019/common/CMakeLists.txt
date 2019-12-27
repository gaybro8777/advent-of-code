add_library(intcode OBJECT intcode.cpp)
target_include_directories(intcode PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(intcode PUBLIC aoc)

add_executable(driver main.cpp)
target_link_libraries(driver PRIVATE
    intcode
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
    day15
    day16
    day17
    day18
    day19
    day20
    day21
    day22
    day23
    day24
    day25)
