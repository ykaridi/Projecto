cmake_minimum_required(VERSION 3.13)
project(ProjectoHW3 C)

set(CMAKE_C_STANDARD 90)

add_executable(ProjectoHW3 main.c engine/sudoku_board.c engine/sudoku_board.h engine/sudoku_solver.c engine/sudoku_solver.h main_aux.c main_aux.h utils.c utils.h parser.c parser.h engine/sudoku_commands.c engine/sudoku_commands.h engine/data_structures/stack.c engine/data_structures/stack.h engine/sudoku_game.c engine/sudoku_game.h engine/data_structures/operation_list.c engine/data_structures/operation_list.h engine/sudoku_game_operation.c engine/sudoku_game_operation.h)