#ifndef E_CASE_HANDLER
#define E_CASE_HANDLER

typedef struct char_struct
{
    int data;
    bool regular;
    bool dot;
    bool square;
    bool round;
    bool backslash;
    bool or;
    bool bracket;
}char_struct;

bool E_handler(char *expression, char *line);


#endif
