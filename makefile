CC = gcc
EXEC = sudoku-console
COMP_FLAG = -ansi -Wall -Wextra -Werror -pedantic-errors -o $(EXEC)

all:
	$(CC) $(COMP_FLAG) main.c engine/sudoku_board.c engine/sudoku_board.h engine/solver/sudoku_solver.c engine/solver/sudoku_solver.h utils.c utils.h engine/commands/parser.c engine/commands/parser.h engine/commands/commands.c engine/commands/commands.h engine/solver/stack.c engine/solver/stack.h engine/sudoku_game.c engine/sudoku_game.h engine/rollback/operation_list.c engine/rollback/operation_list.h engine/rollback/sudoku_game_operation.c engine/rollback/sudoku_game_operation.h engine/commands/sudoku_commands.c engine/commands/sudoku_commands.h engine/fs/sudoku_fs.c engine/fs/sudoku_fs.h engine/user_interface.c engine/user_interface.h
clean:
	rm sudoku
