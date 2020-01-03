CC = gcc
EXEC = sudoku
COMP_FLAG = -ansi -Wall -Wextra -Werror -pedantic-errors -o $(EXEC)

all:
	$(CC) $(COMP_FLAG) utils.c sudoku_game.c sudoku_solver.c parser.c sudoku_commands.c main_aux.c main.c
clean:
	rm sudoku
