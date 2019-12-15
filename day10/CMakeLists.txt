add_library(day10 OBJECT solution.cpp)
target_compile_features(day10 PUBLIC cxx_std_17)
target_link_libraries(day10 Catch2::Catch2)
