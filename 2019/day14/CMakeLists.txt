add_library(day14 OBJECT solution.cpp)
target_compile_features(day14 PUBLIC cxx_std_17)
target_link_libraries(day14 Catch2::Catch2)
