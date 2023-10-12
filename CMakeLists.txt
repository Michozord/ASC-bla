cmake_minimum_required(VERSION 3.20)
project(ASC_bla)

set (CMAKE_CXX_STANDARD 17)

include_directories(src)

add_executable (test_matrix_2 tests/test_matrix_2.cc)
target_sources (test_matrix_2 PUBLIC src/vector.h src/expression.h src/matrix.h)