cmake_minimum_required(VERSION 3.21)
project(SQL)
add_executable(SQL main.cpp)

add_subdirectory("C:\\Projects\\libpqxx-7.9.0" pqxx-build)
target_compile_features(SQL PRIVATE cxx_std_17)
target_link_libraries(SQL pqxx)