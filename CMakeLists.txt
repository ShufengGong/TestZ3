################################################################################
# Example C++ project
################################################################################
project(Z3_C_EXAMPLE CXX)
cmake_minimum_required(VERSION 3.4)
#set(Z3_DIR "./")
find_package(Z3 REQUIRED)

################################################################################
# Z3 C++ API bindings require C++11
################################################################################
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message(STATUS "Z3_FOUND: ${Z3_FOUND}")
message(STATUS "Found Z3 ${Z3_VERSION_STRING}")
message(STATUS "Z3_DIR: ${Z3_DIR}")

add_executable(cpp_example example.cpp)
target_include_directories(cpp_example PRIVATE ${Z3_CXX_INCLUDE_DIRS})
target_link_libraries(cpp_example PRIVATE ${Z3_LIBRARIES})
