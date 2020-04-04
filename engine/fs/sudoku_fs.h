#ifndef PROJECTOHW3_SUDOKU_FS_H
#define PROJECTOHW3_SUDOKU_FS_H

#include <stdio.h>

#include "../../utils.h"
#include "../sudoku_board.h"

enum load_file_status {
    LOAD_SUCCESS = 0,
    LOAD_ENOFILE,
    LOAD_EBADFILE
};

int save_board_to_file(const char* pathname, const sudoku_board_t* board);
enum load_file_status board_from_file(const char* pathname, sudoku_board_t** board_ptr);

#endif
