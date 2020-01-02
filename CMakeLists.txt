cmake_minimum_required(VERSION 3.13)
project(ProjectoHW3 C)

set(CMAKE_C_STANDARD 99)

add_executable(ProjectoHW3 main.c sudoku_game.c sudoku_game.h sudoku_solver.c sudoku_solver.h main_aux.c main_aux.h utils.c utils.h parser.c parser.h)