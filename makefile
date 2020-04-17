CC = gcc
EXEC = sudoku-console
COMP_FLAG = -ansi -Wall -Wextra -Werror -pedantic-errors

GUROBI_INCLUDE_DIRECTORY = /usr/local/lib/gurobi563/include
GUROBI_LIB_DIRECTORY = /usr/local/lib/gurobi563/lib

FILES = main.c engine/sudoku_board.c engine/sudoku_board.h engine/solver/backtracking/sudoku_backtracking.c engine/solver/backtracking/sudoku_backtracking.h utils.c utils.h engine/commands/parser.c engine/commands/parser.h engine/commands/commands.c engine/commands/commands.h engine/solver/backtracking/backtracking_stack.c engine/solver/backtracking/backtracking_stack.h engine/sudoku_game.c engine/sudoku_game.h engine/rollback/operation_list.c engine/rollback/operation_list.h engine/rollback/sudoku_game_operation.c engine/rollback/sudoku_game_operation.h engine/commands/sudoku_commands.c engine/commands/sudoku_commands.h engine/fs/sudoku_fs.c engine/fs/sudoku_fs.h engine/user_interface.c engine/user_interface.h engine/solver/gurobi/grb_processing.c engine/solver/gurobi/grb_processing.h ${GUROBI_INCLUDE_DIRECTORY}/gurobi_c.h engine/solver/gurobi/sudoku_var_list.c engine/solver/gurobi/sudoku_var_list.h engine/random/random.c engine/random/random.h engine/solver/gurobi/grb_commands.c engine/solver/gurobi/grb_commands.h engine/random/random_value_chooser.c engine/random/random_value_chooser.h engine/solver/gurobi/grb_setup.c engine/solver/gurobi/grb_setup.h engine/solver/gurobi/grb_utils.h
LINKING = -I$(GUROBI_INCLUDE_DIRECTORY) -L$(GUROBI_LIB_DIRECTORY) -lgurobi56

all:
	$(CC) $(COMP_FLAG) $(FILES) $(LINKING) -o $(EXEC)
clean:
	rm -f *.o sudoku-console