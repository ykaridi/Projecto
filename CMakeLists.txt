cmake_minimum_required(VERSION 3.13)
project(ProjectoHW3 C)

set(CMAKE_C_STANDARD 90)

add_executable(ProjectoHW3 main.c sudoku_board.c sudoku_board.h sudoku_solver.c sudoku_solver.h main_aux.c main_aux.h utils.c utils.h parser.c parser.h sudoku_commands.c sudoku_commands.h)