add_library(day6 OBJECT solution.cpp)
target_compile_features(day6 PUBLIC cxx_std_17)
target_link_libraries(day6 Catch2::Catch2)
