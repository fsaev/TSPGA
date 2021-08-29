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
   ./src/phenotype.c
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
   ./src/graph_tb.c
   ./src/reader_tb.c
)

include_directories(inc)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g3 -Wall -Wextra -pedantic -Og -std=c99")

add_executable(tspga ${BUILD_SOURCES})

add_executable(tspga_tb ${TEST_SOURCES})
target_compile_definitions(tspga_tb PUBLIC TESTBENCH)

include(CTest)
enable_testing()

add_test(csv_test tspga_tb "CSVREADER")
set_tests_properties(csv_test PROPERTIES PASS_REGULAR_EXPRESSION "SUCCESS")

add_test(graphlink_test tspga_tb "GRAPHLINK")
set_tests_properties(graphlink_test PROPERTIES PASS_REGULAR_EXPRESSION "SUCCESS")

add_test(graphtraverse_test tspga_tb "GRAPHTRAVERSE")
set_tests_properties(graphtraverse_test PROPERTIES PASS_REGULAR_EXPRESSION "SUCCESS")

add_test(permutegenome_test tspga_tb "PERMUTEGENOME")
set_tests_properties(permutegenome_test PROPERTIES PASS_REGULAR_EXPRESSION "SUCCESS")

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
