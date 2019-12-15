add_library(day3 OBJECT solution.cpp)
target_compile_features(day3 PUBLIC cxx_std_17)
target_link_libraries(day3 Catch2::Catch2)
