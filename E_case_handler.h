#ifndef E_CASE_HANDLER
#define E_CASE_HANDLER
#include <stdbool.h>

typedef struct indexes {
    int i_line;
    int j_expression;
    int index;
    int change_in_expression_len;
} indexes;

typedef struct char_struct {
    int data;
    bool regular;
    bool dot;
    bool square;
    bool round;
    bool backslash;
    bool or ;
    bool bracket;
} char_struct;

typedef struct E_case_expression {
    int expression_len;
    bool found_expression;
} E_case_expression;

E_case_expression E_handler(char *expression, char *line);

#endif

