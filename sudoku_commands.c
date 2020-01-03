#include "sudoku_commands.h"

///TODO
command_output_t set(command_args_t args) {
    command_output_t output = {
            .success = 0
    };

    return output;
}

///TODO
command_output_t hint(command_args_t args) {
    command_output_t output = {
            .success = 0
    };

    return output;
}

///TODO
command_output_t validate(command_args_t args) {
    command_output_t output = {
            .success = 0
    };

    return output;
}

///TODO
command_output_t restart(command_args_t args) {
    command_output_t output = {
            .success = 0
    };

    return output;
}

///TODO
command_output_t exit_sudoku(command_args_t args) {
    command_output_t output = {
            .success = 0
    };

    return output;
}


command_t commands[] = {
        {
                .function =     set,
                .command_name = "set"
        },
        {
                .function =     hint,
                .command_name = "hint"
        },
        {
                .function =     validate,
                .command_name = "validate"
        },
        {
                .function =     restart,
                .command_name = "restart"
        },
        {
                .function =     exit_sudoku,
                .command_name = "exit"
        }
};

const int num_commands = sizeof(commands) / sizeof(command_t);