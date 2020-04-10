#ifndef GRB_COMMANDS_H
#define GRB_COMMANDS_H

#include <stdbool.h>
#include <gurobi_c.h>

#include "../../sudoku_board.h"

/**
 * An enum representing the output of the command.
 */
enum grb_output {
    GRB_SUCCESS = 0,
    GRB_ERROR = -1
};

/**
 * An enum representing the kind of gurubi command we want to preform.
 */
enum grb_command {
    GRB_SOLVE,
    GRB_VALIDATE,
    GRB_GUESS,
    GRB_HINT,
    GRB_GUESS_HINT
};


/**
 * An input to the guess function is just a float.
 */
typedef float grb_guess_input_t;

/**
 * A struct representing a input to the hint function.
 */
typedef struct _grb_hint_input {
    int row;
    int col;
    int* value;
} grb_hint_input_t ;

/**
 * A struct representing a input to the guess_hint function.
 */
typedef struct _grb_guess_hint_input {
    int row;
    int col;
    int* values;
    int* len;
} grb_guess_hint_input_t;

/**
 * A struct representing an input to the grb_command function.
 */
typedef struct _grb_input {
    /* The command we want to perform. */
    enum grb_command command;

    /* Written here will be if the board is solvable. */
    bool* solvable_ptr;

    /* Additional input, depending on the specific command performed. */
    union {
        grb_guess_input_t       guess_value;
        grb_hint_input_t        hint_value;
        grb_guess_hint_input_t  guess_hint_value;
    } input;
} grb_args_t;

void create_grb_solve(grb_args_t* args, bool* solvable_ptr);
void create_grb_validate(grb_args_t* args, bool* solvable_ptr);
void create_grb_guess(grb_args_t* args, bool* solvable_ptr, float threshold);
void create_grb_hint(grb_args_t* args, bool* solvable_ptr, int row, int col, int* value);
void create_grb_guess_hint(grb_args_t* args, bool* solvable_ptr, int row, int col, int* values, int* len);

enum grb_output grb_command(GRBenv **env, sudoku_board_t *board, const grb_args_t *args);

void print_gurobi_error(GRBenv* env);


#endif /* GRB_COMMANDS_H */
