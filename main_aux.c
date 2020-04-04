#include "main_aux.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

void print_parsing_error(const parsing_errors_t *error, const command_t *command) {
    switch(error->error_type) {
        case NO_COMMAND:
            printf("Invalid command!\n");
            break;
        case UNAVAILABLE_COMMAND:
            printf("Command [%s] is currently unavailable! ", command->command_name);
            printf("Command is available in modes [");
            if (command->modes_of_availability.init)
                printf(" init ");
            if (command->modes_of_availability.edit)
                printf(" edit ");
            if (command->modes_of_availability.solve)
                printf(" solve ");
            printf("]\n");
            break;
        case INCORRECT_NUM_PARAMS:
            printf("Command [%s] takes %d parameter(s)!\n", command->command_name, command->args.num_arguments);
            break;
        case INCORRECT_TYPE:
            printf("Command [%s] expects parameter [#%d] to be of type [",
                    command->command_name, error->param_index + 1);
            if (command->args.arguments[error->param_index]->argument_type == INTEGER)
                printf("INTEGER");
            else if (command->args.arguments[error->param_index]->argument_type == FLOAT)
                printf("FLOAT");
            else if (command->args.arguments[error->param_index]->argument_type == STRING)
                printf("STRING");
            else
                printf("OTHER");
            printf("]\n");
            break;
        case INCORRECT_RANGE:
            if (command->args.arguments[error->param_index]->argument_type == INTEGER)
                printf("Command [%s] expects parameter [#%d] to be a [INTEGER] in the range [%d, %d]!\n",
                    command->command_name, error->param_index + 1,
                    command->args.arguments[error->param_index]->lower_bound.int_value,
                    command->args.arguments[error->param_index]->upper_bound.int_value);
            else if (command->args.arguments[error->param_index]->argument_type == FLOAT)
                printf("Command [%s] expects parameter [#%d] to be a [FLOAT] in the range [%f, %f]!\n",
                       command->command_name, error->param_index + 1,
                       command->args.arguments[error->param_index]->lower_bound.float_value,
                       command->args.arguments[error->param_index]->upper_bound.float_value);
            break;
        default:
            printf("An unexpected error occurred!\n");
            break;
    }
}
