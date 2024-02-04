cmake_minimum_required(VERSION 3.15)
project(PackageTest CXX)

find_package(ufm_client CONFIG REQUIRED)

add_executable(example src/example.cpp)
target_link_libraries(example ufm_client::ufm_client)
