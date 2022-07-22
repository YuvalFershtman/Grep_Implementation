
#define _CRT_SECURE_NO_WARNINGS
#include "E_case_handler.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SQUARE_CASE_FIND_LAST 3
#define SQUARE_CASE_MOVEMENT 5
#define ROUND_CASE_MOVEMENT 3
#define BACKSLASH 92

bool search_str_round(const char *line, const char *str, int i_line)
{
    int j_str = 0;
    while (str[j_str] != '\0') {
        if (line[i_line] == '\n') {
            return false;
        }
        if (line[i_line] == str[j_str]) {
            i_line = i_line + 1;
            j_str = j_str + 1;
        } else {
            return false;
        }
    }
    return true;
}

char *round_func(char *expression, int num_of_str, int num_of_calls)
{
    char *str2, *str1;
    int len_check = 0;
    if (expression[0] != '(') {
        str1 = strtok(expression, "(");
        len_check = len_check + strlen(str1);
        str1 = strtok(NULL, "|");
        len_check = len_check + strlen(str1);
    } else {
        str1 = strtok(expression, "|");
    }
    if (expression[len_check + 2] == ')') {
        str2 = "";
        if (num_of_str == 2) {
            return str2;
        }
    }
    if (num_of_calls > 1) {
        if (expression[0] != '(') {
            int i;
            for (i = 2; i < num_of_calls; i++) {
                str1 = strtok(NULL, "(");
            }
        } else {
            str1 = strtok(NULL, "(");
            int i;
            for (i = 1; i < num_of_calls; i++) {
                str1 = strtok(NULL, "(");
            }
        }
        str1 = strtok(str1, "|");
    }
    str2 = strtok(NULL, ")");
    if (num_of_str == 1) {
        return str1;
    }
    if (num_of_str == 2) {
        return str2;
    }
    return str1;
}

char_struct expression_mapping(const char *expression, int j_expression)
{
    int i = j_expression;
    char_struct single_char = {0, false, false, false, false, false, false, false};
    single_char.data = j_expression;
    if (expression[i] == '.') {
        single_char.dot = true;
    } else if ((expression[i] == ']') || (expression[i] == '[')) {
        single_char.square = true;
    } else if ((expression[i] == ')') || (expression[i] == '(')) {
        single_char.round = true;
    } else if ((expression[i] == '{') || (expression[i] == '}')) {
        single_char.bracket = true;
    } else if (expression[i] == BACKSLASH) {
        single_char.backslash = true;
    } else if (expression[i] == '|') {
        single_char.or = true;
    } else {
        single_char.regular = true;
    }
    return single_char;
}

indexes there_is_backslash(char_struct single_char, const char *line, const char *expression, indexes my_indexes)
{
    if (line[my_indexes.i_line] == expression[my_indexes.j_expression]) {
        my_indexes.j_expression = my_indexes.j_expression + 1;
        my_indexes.i_line = my_indexes.i_line + 1;
    } else {
        if (single_char.backslash != true) {
            my_indexes.i_line = my_indexes.i_line + 1;
        } else {
            my_indexes.j_expression = 0;
            return my_indexes;
        }
    }
    if (my_indexes.j_expression > 0 && line[my_indexes.i_line] != expression[my_indexes.j_expression] &&
        expression[my_indexes.j_expression] != '\0') {
        if (single_char.regular != true && expression[my_indexes.j_expression] != '(' &&
            expression[my_indexes.j_expression] != '.' && expression[my_indexes.j_expression] != '[' &&
            expression[my_indexes.j_expression] != BACKSLASH) {
            my_indexes.j_expression = 0;
        } else {
            if (expression[my_indexes.j_expression] != BACKSLASH && expression[my_indexes.j_expression] != '[' &&
                expression[my_indexes.j_expression] != '(' && expression[my_indexes.j_expression] != '.') {
                my_indexes.j_expression = 0;
            }
        }
    }
    return my_indexes;
}

indexes special_square(indexes my_indexes, const char *expression, const char *line)
{
    char first_char;
    char last_char;
    bool special_char_in_line = false;
    char current_char;
    first_char = expression[my_indexes.j_expression + 1];
    last_char = expression[my_indexes.j_expression + SQUARE_CASE_FIND_LAST];
    while (line[my_indexes.i_line] < first_char || line[my_indexes.i_line] > last_char) {
        if (line[my_indexes.i_line] == '\n') {
            break;
        }
        my_indexes.i_line++;
    }
    for (current_char = first_char; current_char <= last_char; current_char++) {
        if (line[my_indexes.i_line] == current_char) {
            special_char_in_line = true;
            break;
        }
    }
    if (special_char_in_line == true) {
        my_indexes.j_expression = my_indexes.j_expression + SQUARE_CASE_MOVEMENT;
        my_indexes.i_line = my_indexes.i_line + 1;
    } else {
        my_indexes.j_expression = 0;
    }
    return my_indexes;
}

indexes special_round(indexes my_indexes, char *expression_backup, int num_of_calls, char *expression, char *line)
{
    char *str1, *str2;
    bool found_str1, found_str2;
    my_indexes.change_in_expression_len = 0;
    strcpy(expression_backup, expression);
    str1 = round_func(expression_backup, 1, num_of_calls);
    strcpy(expression_backup, expression);
    str2 = round_func(expression_backup, 2, num_of_calls);
    if (expression[0] == '(' && num_of_calls == 1) {
        str1 = str1 + 1;
    }
    int len_str1 = strlen(str1);
    int len_str2 = strlen(str2);
    found_str1 = search_str_round(line, str1, my_indexes.i_line);
    found_str2 = search_str_round(line, str2, my_indexes.i_line);
    if (found_str1 == true) {
        my_indexes.change_in_expression_len = ROUND_CASE_MOVEMENT + len_str2;
        my_indexes.i_line = my_indexes.i_line + len_str1;
        my_indexes.j_expression = my_indexes.j_expression + len_str1 + len_str2 + ROUND_CASE_MOVEMENT;
    } else if (found_str2 == true) {
        my_indexes.change_in_expression_len = ROUND_CASE_MOVEMENT + len_str1;
        my_indexes.i_line = my_indexes.i_line + len_str2;
        my_indexes.j_expression = my_indexes.j_expression + len_str1 + len_str2 + ROUND_CASE_MOVEMENT;
    } else {
        my_indexes.j_expression = 0;
        my_indexes.i_line = my_indexes.index + 1;
        my_indexes.index = my_indexes.index + 1;
    }
    return my_indexes;
}

E_case_expression E_handler(char *expression, char *line)
{
    bool backslash_before = false;
    char_struct single_char;
    indexes my_indexes = {0, 0, 0, 0};
    int num_of_calls = 0, expression_len = strlen(expression);
    E_case_expression e_case_expression = {0, false};
    while ((line[my_indexes.i_line] != '\n') && (expression[my_indexes.j_expression] != '\0')) {
        single_char = expression_mapping(expression, my_indexes.j_expression);
        if (backslash_before == true) {
            if (single_char.dot == true || single_char.square == true || single_char.round == true ||
                single_char.backslash == true || single_char.or == true || single_char.bracket == true) {
                expression_len = expression_len - 1;
                my_indexes = there_is_backslash(single_char, line, expression, my_indexes);
            }
            backslash_before = false;
        } else if (single_char.dot == true) {
            my_indexes.j_expression = my_indexes.j_expression + 1;
            my_indexes.i_line = my_indexes.i_line + 1;
        } else if (single_char.square == true) {
            expression_len = expression_len - SQUARE_CASE_MOVEMENT + 1;
            my_indexes = special_square(my_indexes, expression, line);
        } else if (single_char.round == true) {
            char *expression_backup;
            num_of_calls = num_of_calls + 1;
            expression_backup = (char *)malloc(sizeof(char *) * (strlen(expression) + 1));
            my_indexes = special_round(my_indexes, expression_backup, num_of_calls, expression, line);
            expression_len = expression_len - my_indexes.change_in_expression_len;
            if (my_indexes.change_in_expression_len == 0) {
                num_of_calls = 0;
            }
            free(expression_backup);
        } else if (single_char.backslash == true) {
            backslash_before = true;
            my_indexes.j_expression = my_indexes.j_expression + 1;
        } else {  // regular label
            my_indexes = there_is_backslash(single_char, line, expression, my_indexes);
            backslash_before = false;
        }
    }
    e_case_expression.expression_len = expression_len;
    if (expression[my_indexes.j_expression] == '\0') {
        e_case_expression.found_expression = true;
        return e_case_expression;
    } else if (line[my_indexes.i_line] == '\n') {
        e_case_expression.found_expression = false;
        return e_case_expression;
    }
    e_case_expression.found_expression = false;
    return e_case_expression;
}
