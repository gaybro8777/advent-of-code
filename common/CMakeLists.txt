add_library(aoc OBJECT)
target_sources(aoc PRIVATE aoc_intcode.cpp)
target_compile_features(aoc PUBLIC cxx_std_20)
target_include_directories(aoc PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
