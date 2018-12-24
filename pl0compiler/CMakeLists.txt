cmake_minimum_required(VERSION 2.8)
project(pl0)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

add_subdirectory(lex)
add_subdirectory(common)
add_subdirectory(syntax)
add_subdirectory(error)

add_executable(PL0 test.c)
target_link_libraries(PL0 lex common tax error)
