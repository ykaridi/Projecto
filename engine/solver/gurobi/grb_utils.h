#ifndef GRB_UTILS_H
#define GRB_UTILS_H

#define LOG_FILE "grb.log"
#define DEFAULT_MODEL_NAME "sudoku_model"

#define GURUBI_OUTPUT (0)

/**
 * An enum representing the type of programming we want to do.
 */
enum programming_type {
    ILP,
    LP
};

#endif /* GRB_UTILS_H */
