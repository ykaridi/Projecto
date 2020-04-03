#ifndef PROJECTOHW3_SUDOKU_FS_H
#define PROJECTOHW3_SUDOKU_FS_H

#include <stdio.h>

#include "../../utils.h"
#include "../sudoku_board.h"

int save_board(const char* pathname, const sudoku_board_t* board);

enum load_return_value {
    LOAD_SUCCESS = 0,
    LOAD_ENOFILE,
    LOAD_EBADFILE
};

enum load_return_value load_board(const char* pathname, sudoku_board_t** board);


#endif
