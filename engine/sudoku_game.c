#include "sudoku_game.h"
#include "fs/sudoku_fs.h"

/**
 * Create a sudoku game object
 * @return Pointer to the game object
 */
sudoku_game_t* create_game() {
    sudoku_game_t *game = malloc(sizeof(sudoku_game_t));

    /* Check if malloc was successful */
    if (game == NULL) {
        EXIT_ON_ERROR("malloc");
    }

    game->mode = INIT;
    game->mark_errors = TRUE;
    game->board = NULL;
    game->temporary_board = NULL;
    game->last_operation = NULL;
    game->grb_env = NULL;

    return game;
}

/**
 * Load a new board into the sudoku game
 * @param game
 * @param board
 */
void load_board(sudoku_game_t *game, sudoku_board_t *board) {
    operation_node_t *operations = create_operation_node();

    if (operations == NULL) {
        EXIT_ON_ERROR("malloc");
    }

    /* Free game objects, if previously allocated */
    if (game->board != NULL)
        destruct_board(game->board);
    game->board = board;
    if (game->temporary_board != NULL)
        destruct_board(game->temporary_board);
    game->temporary_board = create_board(board->rows, board->cols);

    if (game->last_operation != NULL)
        destruct_operation_list(game->last_operation);
    operation_list_append(operations, create_head_game_operation());
    game->last_operation = operations;
}

/**
 * Destroy (destroy) the sudoku game object
 */
void destruct_game(sudoku_game_t *game) {
    if (game->board != NULL)
        destruct_board(game->board);
    if (game->temporary_board != NULL)
        destruct_board(game->temporary_board);

    if (game->last_operation != NULL)
        destruct_operation_list(game->last_operation);

    if (game->grb_env != NULL)
        GRBfreeenv(game->grb_env);

    free(game);
}
