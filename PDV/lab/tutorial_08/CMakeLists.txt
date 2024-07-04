cmake_minimum_required(VERSION 3.15)
project(pdv_lab08)

set(CMAKE_CXX_STANDARD 17)

# force release build
set(CMAKE_BUILD_TYPE "Release")
# enable more warnings
if(MSVC)
	add_compile_options("/W4")
else()
	add_compile_options("-Wall" "-Wextra" "-Wpedantic")
endif()

# OpenMP 4.0 is required for user defined reductions
find_package(OpenMP 4.0 REQUIRED)

add_executable(sparse_multiplication src/main.cpp src/multiply.cpp)
target_link_libraries(sparse_multiplication PUBLIC OpenMP::OpenMP_CXX)