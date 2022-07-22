#ifndef COMMAND_HANDLER
#define COMMAND_HANDLER

#include <stdbool.h>

typedef struct command_arguments {
    bool type_A;
    int NUM_A;
    bool type_b;
    bool type_c;
    bool type_i;
    bool type_n;
    bool type_v;
    bool type_x;
    bool type_E;
    char *expression;
    char *file_name;
    bool piped;
} command_arguments;

command_arguments read_command(int argc, char *argv[], command_arguments arguments);

#endif
