#define _CRT_SECURE_NO_WARNINGS
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command_handler.h"
#include "line_handler.h"
#include "print_handler.h"
#include "E_case_handler.h"


char* to_lower(char *string) {
    int i;
    for (i = 0; string[i] != '\0'; i++) {
        if (string[i] >= 'A' && string[i] <= 'Z') {
            string[i] = string[i] + 32;
        }
    }
    return string;
}

bool search(char *expression, char *line, size_t line_len) {
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


line_struct execute(command_arguments arguments, line_struct line) {
    char *line_backup;
    char *expression_backup;
    if (arguments.type_i == true) {
        line_backup = (char *)malloc(sizeof(char *) * (strlen(line.string) + 1));
        if (line_backup == NULL) {
            printf("Allocation failure!!\n");
            exit(1);
        }
        strcpy(line_backup, line.string);

        expression_backup = (char *)malloc(sizeof(char *) * (strlen(arguments.expression) + 1));
        if (expression_backup == NULL) {
            printf("Allocation failure!!\n");
            free(line_backup);
            exit(1);
        }
        strcpy(expression_backup, arguments.expression);

        expression_backup = to_lower(expression_backup);
        line_backup = to_lower(line_backup);
        if (arguments.type_E == false){
            line.found_match = search(expression_backup, line_backup, line.line_len);
        }
        else{
            line.found_match = E_handler(expression_backup, line_backup);
        }
        free(line_backup);
        free(expression_backup);
    }
    else
    {
        line.found_match = search(arguments.expression, line.string, line.line_len);
    }
    if (arguments.type_x == true) {
        if ((strlen(line.string) - 1) != strlen(arguments.expression))
        {
            line.found_match = false;
            if (arguments.type_v == false && arguments.type_A == false) {
                return line;
            }
        }
    }
    if (arguments.type_v == true) {
        line.found_match = !line.found_match;
    }
    if (line.found_match == true) {
        if (arguments.type_A == true)
        {
            line.print_next = true;
            line.number_of_next_prints = arguments.NUM_A;
        }
        line = print_function(arguments, line);
    }
    else if (line.number_of_next_prints>0)
    {
        line = print_function(arguments, line);
    }
    else if (line.found_match == false && line.number_of_next_prints==0 ){
        line.print_next=false;
    }
    else if (arguments.type_E == true){
        line.found_match = E_handler( arguments.expression, line.string);
        line = print_function(arguments, line);
    }
    return line;
}
