#define _CRT_SECURE_NO_WARNINGS
#include "print_handler.h"
#include "command_handler.h"
#include "line_handler.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

line_struct print_function(command_arguments arguments, line_struct line)
{
    line.print_flag = true;
    if (arguments.type_c == true) {
        line.print_flag = true;
        line.print_next = false;
        line.number_of_next_prints = 0;
        return line;
    }
    if (line.found_match == true) {
        if (line.print_previous == false && line.number_of_previous_prints > 1 && arguments.type_A == true) {
            printf("--\n");
        }
        if (arguments.type_n == true) {
            printf("%d:", line.line_number);
        }
        if (arguments.type_b == true) {
            printf("%d:", line.byts_count);
        }
        printf("%s", line.string);
    } else {
        if (arguments.type_E == true) {
            return line;
        }
        if (arguments.type_n == true) {
            printf("%d-", line.line_number);
        }
        if (arguments.type_b == true) {
            printf("%d-", line.byts_count);
        }
        printf("%s", line.string);
        line.number_of_next_prints = line.number_of_next_prints - 1;
    }

    return line;
}


