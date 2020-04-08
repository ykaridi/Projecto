cmake_minimum_required(VERSION 3.13)
project(ProjectoHW3 C)

set(CMAKE_C_FLAGS "-ansi -Wall -Wextra -Werror -pedantic-errors ${CMAKE_C_FLAGS}")

set(GUROBI_INCLUDE_DIRECTORY "/opt/gurobi901/linux64/include")
include_directories(${GUROBI_INCLUDE_DIRECTORY})

set(GUROBI_LIB_DIRECTORY "/opt/gurobi901/linux64/lib")
link_directories(${GUROBI_LIB_DIRECTORY})


add_executable(ProjectoHW3 main.c engine/sudoku_board.c engine/sudoku_board.h engine/solver/backtracking/sudoku_backtracking.c
        engine/solver/backtracking/sudoku_backtracking.h utils.c utils.h engine/commands/parser.c engine/commands/parser.h
        engine/commands/commands.c engine/commands/commands.h engine/solver/backtracking/backtracking_stack.c engine/solver/backtracking/backtracking_stack.h
        engine/sudoku_game.c engine/sudoku_game.h engine/rollback/operation_list.c engine/rollback/operation_list.h
        engine/rollback/sudoku_game_operation.c engine/rollback/sudoku_game_operation.h engine/commands/sudoku_commands.c engine/commands/sudoku_commands.h
        engine/fs/sudoku_fs.c engine/fs/sudoku_fs.h engine/user_interface.c engine/user_interface.h engine/solver/gurobi/sudoku_grb.c engine/solver/gurobi/sudoku_grb.h
        ${GUROBI_INCLUDE_DIRECTORY}/gurobi_c.h engine/solver/gurobi/sudoku_var_list.c engine/solver/gurobi/sudoku_var_list.h engine/random.c engine/random.h)

target_link_libraries(ProjectoHW3 gurobi90)