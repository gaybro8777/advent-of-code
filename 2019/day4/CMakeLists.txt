add_library(day4 OBJECT solution.cpp)
target_compile_features(day4 PUBLIC cxx_std_17)
target_link_libraries(day4 Catch2::Catch2)
