file(GLOB sources CONFIGURE_DEPENDS "*.cpp")
add_executable(aoc2020 ${sources})
target_link_libraries(aoc2020 PUBLIC aoc Catch2::Catch2)
