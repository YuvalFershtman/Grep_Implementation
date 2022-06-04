#ifndef LINE_HANDLER
#define LINE_HANDLER

typedef struct line_struct
{
    char *string;
    size_t line_len;
    int line_number;
    int byts_count;
    int number_of_next_prints;
    int number_of_previous_prints;
    bool print_flag;
    bool found_match;
    bool print_next;
    bool print_previous;
}line_struct;

line_struct execute(command_arguments arguments, line_struct line);

#endif