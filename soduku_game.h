struct _soduku_board {
    int rows;
    int cols;
    int sub_board_size;

    int* board;
    char* cell_metadata;
} typedef soduku_board;

soduku_board* create_board(int rows, int cols);
void destruct_board(soduku_board *board);

int is_in_board(const soduku_board* board, int sub_board_i, int sub_board_j, int inner_i, int inner_j);
int is_in_board_flattened(const soduku_board *board, int i, int j);

int get_cell(const soduku_board *board, int sub_board_i, int sub_board_j, int inner_i, int inner_j);
int get_cell_flattened(const soduku_board *board, int i, int j);
int set_cell(soduku_board *board, int value, int sub_board_i, int sub_board_j, int inner_i, int inner_j);
int set_cell_flattened(soduku_board *board, int value, int i, int j);
char get_cell_metadata(const soduku_board *board, int sub_board_i, int sub_board_j, int inner_i, int inner_j);
char get_cell_metadata_flattened(const soduku_board *board, int i, int j);
int set_cell_metadata(soduku_board *board, char metadata, int sub_board_i, int sub_board_j, int inner_i, int inner_j);
int set_cell_metadata_flattened(soduku_board *board, char metadata, int i, int j);