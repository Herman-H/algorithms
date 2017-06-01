cmake_minimum_required (VERSION 3.8)
project (tmp_test)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++17")

include_directories("../tmp")

add_executable(tmp_test 
    test.cpp
    test_tmp.cpp
    test_math.cpp
    test_types.cpp
    test_data.cpp
    test_function.cpp
    test_rational.cpp)