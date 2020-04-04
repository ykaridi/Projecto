#include "sudoku_game.h"
#include "../utils.h"
#include "fs/sudoku_fs.h"

sudoku_game_t* create_game() {
    sudoku_game_t *game = malloc(sizeof(sudoku_game_t));

    if (game == NULL) {
        EXIT_ON_ERROR("malloc");
    }

    game->mode = INIT;
    game->mark_errors = TRUE;
    game->board = NULL;
    game->temporary_board = NULL;
    game->last_operation = NULL;

    return game;
}
void load_board(sudoku_game_t *game, sudoku_board_t *board) {
    operation_node_t *operations = create_operation_list();

    if (operations == NULL) {
        EXIT_ON_ERROR("malloc");
    }

    if (game->board != NULL)
        destruct_board(game->board);
    game->board = board;
    if (game->temporary_board != NULL)
        destruct_board(game->temporary_board);
    game->temporary_board = create_board(board->rows, board->cols);

    if (game->last_operation != NULL)
        destruct_operation_list(game->last_operation);
    game->last_operation = operations;
}

void destruct_game(sudoku_game_t *game) {
    if (game->board != NULL)
        destruct_board(game->board);
    if (game->temporary_board != NULL)
        destruct_board(game->temporary_board);

    if (game->last_operation != NULL)
        destruct_operation_list(game->last_operation);

    free(game);
}
