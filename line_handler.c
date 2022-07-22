File Edit Options Buffers Tools C Help
#define _CRT_SECURE_NO_WARNINGS
#include "line_handler.h"
#include "E_case_handler.h"
#include "command_handler.h"
#include "print_handler.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define DECIMAL_DIFF_UPPER_LOWER 32

char *to_lower(char *string)
{
    int i;
    for (i = 0; string[i] != '\0'; i++) {
        if (string[i] >= 'A' && string[i] <= 'Z') {
            string[i] = string[i] + DECIMAL_DIFF_UPPER_LOWER;
        }
    }
    return string;
}

bool search(const char *expression, const char *line, size_t line_len)
{
    int i = 0, j = 0, len_of_line = line_len;
    for (i = 0; i < len_of_line; i = i + 1) {
        if (line[i] == expression[j]) {
            j = j + 1;
            if (expression[j] == '\0') {
                return true;
            }
        }
        if (line[i] == '\0') {
            return false;
        }
        if (j > 0 && line[i + 1] != expression[j]) {
            j = 0;
        }
    }
    return false;
}

bool x_case_handler(command_arguments arguments, line_struct line, int E_expression_len)
{
    int line_len = strlen(line.string) - 1;
    int expression_len = strlen(arguments.expression);
    if (arguments.type_E == true) {
        if (line_len != E_expression_len) {
            line.found_match = false;
            if (arguments.type_v == false && arguments.type_A == false) {
                return true;
            }
        }
    } else if (line_len != expression_len) {
        line.found_match = false;
        if (arguments.type_v == false && arguments.type_A == false) {
            return true;
        }
    }
    return false;
}

line_struct execute(command_arguments arguments, line_struct line)
{
    char *line_backup;
    char *expression_backup;
    E_case_expression e_case_exprssion = {0, false};
    bool is_return_x_case = false;
    if (arguments.type_i == true) {
        line_backup = (char *)malloc(sizeof(char *) * (strlen(line.string) + 1));
        strcpy(line_backup, line.string);
        expression_backup = (char *)malloc(sizeof(char *) * (strlen(arguments.expression) + 1));
        strcpy(expression_backup, arguments.expression);
        expression_backup = to_lower(expression_backup);
        line_backup = to_lower(line_backup);
        if (arguments.type_E == false) {
            line.found_match = search(expression_backup, line_backup, line.line_len);
        } else {
            e_case_exprssion = E_handler(expression_backup, line_backup);
            line.found_match = e_case_exprssion.found_expression;
        }
        free(line_backup);
        free(expression_backup);
    } else {
        if (arguments.type_E == true) {
            e_case_exprssion = E_handler(arguments.expression, line.string);
            line.found_match = e_case_exprssion.found_expression;
        } else {
            line.found_match = search(arguments.expression, line.string, line.line_len);
        }
    }
    if (arguments.type_x == true) {
        is_return_x_case = x_case_handler(arguments, line, e_case_exprssion.expression_len);
        if (is_return_x_case == true) {
            return line;
        }
    }
    if (arguments.type_v == true) {
        line.found_match = !line.found_match;
    }
    if (line.found_match == true) {
        if (arguments.type_A == true) {
            line.print_next = true;
            line.number_of_next_prints = arguments.NUM_A;
        }
        line = print_function(arguments, line);
    } else if (line.number_of_next_prints > 0) {
        line = print_function(arguments, line);
    } else if (line.found_match == false && line.number_of_next_prints == 0) {
        line.print_next = false;
    }
    return line;
}
