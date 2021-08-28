cmake_minimum_required(VERSION 3.0.0)
project(GA VERSION 0.1.0)

include_directories(inc)

set (
   SOURCES
   ./src/reader.c
   ./src/graph.c
   ./src/tsp.c
   ./src/interface.c
   ./src/bruteforce.c
   ./src/permute.c
   ./src/ga.c
)

set (
   BUILD_SOURCES
   ${SOURCES}
   ./src/main.c
)

set (
   TEST_SOURCES
   ${SOURCES}
   ./src/testbench.c
)

include_directories(inc)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g3 -Wall -Wextra -pedantic -Og -std=c99")

add_executable(tspga ${BUILD_SOURCES})

add_executable(tspga_tb ${TEST_SOURCES})
target_compile_definitions(tspga_tb PUBLIC TESTBENCH)

include(CTest)
enable_testing()
add_test(graph_test tspga_tb "GRAPH")
set_tests_properties(graph_test PROPERTIES PASS_REGULAR_EXPRESSION "SUCCESS")

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
