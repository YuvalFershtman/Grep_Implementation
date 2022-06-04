File Edit Options Buffers Tools C Help
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include "command_handler.h"

        command_arguments switch_cases_handler(char *argument, command_arguments arguments) {
    if (strcmp(argument, "-A") == 0) {
        arguments.type_A = true;
    }
    else if (strcmp(argument, "-b") == 0) {
        arguments.type_b = true;
    }
    else if (strcmp(argument, "-c") == 0) {
        arguments.type_c = true;
    }
    else if (strcmp(argument, "-i") == 0) {
        arguments.type_i = true;
    }
    else if (strcmp(argument, "-n") == 0) {
        arguments.type_n = true;
    }
    else if (strcmp(argument, "-v") == 0) {
        arguments.type_v = true;
    }
    else if (strcmp(argument, "-x") == 0) {
        arguments.type_x = true;
    }
    else if (strcmp(argument, "-E") == 0) {
        arguments.type_E = true;
    }
    else{
        if (arguments.expression == NULL) {
            arguments.expression = argument;
        }
        else if (!arguments.piped) {
            arguments.file_name = argument;
        }
    }
    return arguments;
}



command_arguments read_command(int argc, char *argv[], command_arguments arguments) {
    if (!isatty(STDIN_FILENO)){
        arguments.piped = true;
    }
    if (argc == 2) {  //only exprssion is given
        arguments.expression = argv[1];
        return arguments;
    }
    bool flag_A_is_on = false;
    int i;
    for (i=1 ; i < argc; i++) {
        arguments = switch_cases_handler(argv[i], arguments);
        if (arguments.type_A == true && flag_A_is_on==false) {
            flag_A_is_on = true;
            i++;
            int NUM = atoi(argv[i]);
            arguments.NUM_A = NUM;
        }
    }
    return arguments;
}
