cmake_minimum_required(VERSION 3.10)
set(CMAKE_C_STANDARD 23)
project(sudoku)
add_executable(sudoku
  sudoku.c
  board.c
  cli.c
  )

find_package(Curses REQUIRED)

include_directories(${CURSES_INCLUDE_DIRS})
target_link_libraries(sudoku ${CURSES_LIBRARY})
