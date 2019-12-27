add_library(aoc INTERFACE)
target_compile_features(aoc INTERFACE cxx_std_17)
target_include_directories(aoc INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
