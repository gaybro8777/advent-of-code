add_library(day8 OBJECT solution.cpp)
target_compile_features(day8 PUBLIC cxx_std_17)
target_link_libraries(day8 Catch2::Catch2)
