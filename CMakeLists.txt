cmake_minimum_required(VERSION 3.0.0)
project(GA VERSION 0.1.0)

set (
   SOURCES
   ./src/main.c
   ./src/reader.c
   ./src/graph.c
   ./src/tsp.c
   ./src/interface.c
)

include_directories(inc)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g3 -Wall -Wextra -pedantic -Og -std=c99")

include(CTest)
enable_testing()

add_executable(GA ${SOURCES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
