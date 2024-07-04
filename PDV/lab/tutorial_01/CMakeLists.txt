cmake_minimum_required(VERSION 3.15)
project(B4B36PDV_01)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE "Release")

if(MSVC)
	add_compile_options("/W4")
	add_compile_options("/O2")
else()
	add_compile_options("-Wall" "-Wextra")
	add_compile_options("-O3")
endif()

find_package(Threads REQUIRED)
find_package(OpenMP REQUIRED)

add_executable(hello.bin hello.cpp)
add_executable(memory.bin memory.cpp)
add_executable(matrix.bin matrix.cpp)
add_executable(false_sharing.bin false_sharing.cpp)
add_executable(magic.bin magic.cpp cpu_info.cpp)
add_executable(decrypt.bin decrypt.cpp PDVCrypt.cpp)

target_link_libraries(false_sharing.bin PUBLIC Threads::Threads)
target_link_libraries(hello.bin PUBLIC OpenMP::OpenMP_CXX)
target_link_libraries(magic.bin PUBLIC OpenMP::OpenMP_CXX)
target_link_libraries(decrypt.bin PUBLIC OpenMP::OpenMP_CXX)
