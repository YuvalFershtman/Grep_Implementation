#define _CRT_SECURE_NO_WARNINGS
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command_handler.h"
#include "line_handler.h"
#include "print_handler.h"
#include "E_case_handler.h"


bool  search_str_round(char *line, char *str , int i_line){
    int j_str = 0;
    while ( str[j_str]!= '\0' && line[i_line]!='\0'){
        if(line[i_line] == str[j_str]){
            i_line=i_line+1;
            j_str = j_str +1;
        }
        else {
            return false;
        }
    }
    return true;
}



void round_func( char *expression, int i, char *str1, char *str2) {
    int count_str1_len = 1;
    int num1=i;
    int j,k;
    while (expression[num1] != '|') {
        count_str1_len++;
        num1++;
    }
    int num2=i;
    for (j=0; j < count_str1_len; j++) {
        str1[j] = expression[num2];
        num2++;
    }
    str1[count_str1_len] = '\0';
    int num3 = num2 +1;
    int num4 = num2 +1;
    int count_str2_len = 1;
    while (expression[num3] != ')') {
        count_str2_len++;
        num3++;
    }
    for (k = 0; k < count_str2_len; k++) {
        str2[k] = expression[num4];
        num4++;
    }
    str2[count_str2_len] = '\0';
}




char_struct expression_mapping(char *expression, int j_expression) {
    int i = j_expression ;
    char backslash = 92;
    char_struct single_char={0,false,false,false,false,false,false,false};
    single_char.data=j_expression;
    if (expression[i]=='.') {
        single_char.dot = true;
    }
    else if ((expression[i]== ']') || (expression[i]== '[')) {
        single_char.square = true;
    }
    else if ((expression[i]== ')') || (expression[i]== '(')) {
        single_char.round = true;
    }
    else if ((expression[i]== '{') || (expression[i]== '}')) {
        single_char.bracket = true;
    }
    else if (expression[i]==backslash) {
        single_char.backslash = true;
    }
    else if (expression[i]== '|') {
        single_char.or = true;
    }
    else {
        single_char.regular = true;
    }
    return single_char;

}



bool E_handler(char *expression, char *line) {
    bool backslash_before = false;
    char_struct single_char;
    int i_line = 0, j_expression = 0;
    char backslash = 92;
    while ((line[i_line] != '\n') && (expression[j_expression] != '\0')) {
        single_char=expression_mapping(expression, j_expression);

        if (single_char.dot == true) {
            if (backslash_before == true) {
                if (line[i_line]== expression[j_expression]) {
                    j_expression = j_expression + 1;
                    i_line = i_line + 1;
                }
                else{
                    i_line = i_line + 1;
                }
                if(j_expression>0 && line[i_line] != expression[j_expression] && expression[j_expression]!='\0'){
                    j_expression = 0;
                }
            }
            else {
                j_expression = j_expression + 1;
                i_line = i_line + 1;
            }
            backslash_before = false;
        }
        else if (single_char.square == true) {
            if (backslash_before == true) {
                if (line[i_line]== expression[j_expression]) {
                    j_expression = j_expression + 1;
                    i_line = i_line + 1;
                }
                else{
                    i_line = i_line + 1;
                }
                if(j_expression>0 && line[i_line] != expression[j_expression] && expression[j_expression]!='\0'){
                    j_expression = 0;
                }
            }
            else {
                int first_char;
                int last_char;
                first_char = expression[j_expression + 1];
                last_char = expression[j_expression + 3];
                bool special_char_in_line = false;
                int current_char;
                for (current_char = first_char; current_char <= last_char; current_char++) {
                    if (line[i_line] == current_char) {
                        special_char_in_line = true;
                        break;
                    }
                }
                if (special_char_in_line == true) {
                    j_expression = j_expression + 5;
                    i_line = i_line + 1;
                }
                else {
                    j_expression = 0;
                }
            }
            backslash_before = false;
        }
        else if (single_char.round == true) {
            if (backslash_before == true) {
                if (line[i_line]== expression[j_expression]) {
                    j_expression = j_expression + 1;
                    i_line = i_line + 1;
                }
                else{
                    i_line = i_line + 1;
                }
                if(j_expression>0 && line[i_line] != expression[j_expression] && expression[j_expression]!='\0'){
                    j_expression = 0;
                }
            }
            else {
                char *str1, *str2;
                bool found_str1, found_str2;
                str1 = (char *)malloc(sizeof(char *) * (strlen(line) + 1));
                str2 = (char *)malloc(sizeof(char *) * (strlen(line) + 1));
                round_func(expression, j_expression, str1, str2);
                int len_str1 = strlen(str1);
                int len_str2 = strlen(str2);
                found_str1 = search_str_round(line, str1, i_line);
                found_str2 = search_str_round(line, str2, i_line);
                if (found_str1 == true) {
                    i_line = i_line + len_str1;
                    j_expression = j_expression + len_str1 + len_str2 + 3;
                }
                else if (found_str2 == true) {
                    i_line = i_line + len_str1;
                    j_expression = j_expression + len_str1 + len_str2 + 3;
                }
                else {
                    j_expression = 0;
                }
                free(str1);
                free(str2);
            }
            backslash_before = false;
        }
        else if (single_char.backslash == true) {
            if (backslash_before == true) {
                if (line[i_line]== expression[j_expression]) {
                    j_expression = j_expression + 1;
                    i_line = i_line + 1;
                }
                else {
                    j_expression = 0;
                }
            }
            else {
                backslash_before = true;
                j_expression = j_expression + 1;
            }
        }
        else if (single_char.or == true) {
            if (line[i_line]== expression[j_expression]) {
                j_expression = j_expression + 1;
                i_line = i_line + 1;
            }
            else{
                i_line = i_line + 1;
            }
            if(j_expression>0 && line[i_line] != expression[j_expression] && expression[j_expression]!='\0'){
                j_expression = 0;
            }
            backslash_before = false;
        }
        else if (single_char.bracket == true) {
            if (line[i_line]== expression[j_expression]) {
                j_expression = j_expression + 1;
                i_line = i_line + 1;
            }
            else{
                i_line = i_line + 1;
            }
            if(j_expression>0 && line[i_line] != expression[j_expression] && expression[j_expression]!='\0'){
                j_expression = 0;
            }
            backslash_before = false;
        }
        else {//regular label
            if (line[i_line]== expression[j_expression]) {
                j_expression = j_expression + 1;
                i_line = i_line + 1;
            }
            else{
                i_line = i_line + 1;
            }
            if(j_expression>0 && line[i_line] != expression[j_expression] && expression[j_expression]!='\0'){
                if( expression[j_expression]!=backslash){
                    j_expression = 0;
                }
            }
            backslash_before = false;
        }
    }
    if (expression[j_expression] == '\0'){
        return true;
    }
    else if (line[i_line] == '\n'){
        return false;
    }
    return false;
}
