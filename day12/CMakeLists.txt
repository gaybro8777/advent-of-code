add_library(day12 OBJECT solution.cpp)
target_compile_features(day12 PUBLIC cxx_std_17)
target_link_libraries(day12 Catch2::Catch2)
