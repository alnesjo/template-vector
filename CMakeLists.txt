cmake_minimum_required (VERSION 2.8.11)
project (TemplateVector)

include_directories (include)

SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -std=c++11 -pedantic -g")
SET (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lgtest_main")

find_package (GTest REQUIRED)
include_directories (${GTEST_INCLUDE_DIRS})

add_subdirectory (test)

enable_testing ()
add_test (NAME UnitTest COMMAND VectorTest)
