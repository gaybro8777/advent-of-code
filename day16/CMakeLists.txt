add_library(day16 OBJECT solution.cpp)
target_compile_features(day16 PUBLIC cxx_std_17)
target_link_libraries(day16 Catch2::Catch2)
