File Edit Options Buffers Tools C Help
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include "command_handler.h"
#include "line_handler.h"
#include "print_handler.h"
#include "E_case_handler.h"


int main(int argc, char *argv[]) {
    command_arguments arguments = { false, 0, false, false, false, false,
                                    false, false, false, NULL, NULL,false };
    arguments = read_command(argc, argv, arguments);


    FILE *myfile;
    if (arguments.piped == true){
        myfile = fdopen(STDIN_FILENO, "r");
    }
    else{
        myfile = fopen(arguments.file_name, "r");
    }
    if (myfile == NULL) {
        exit(1);
    }
    int count_line_number = 0;
    int count_prints = 0;
    int byts_count_file = 0;
    int num_of_prints_next = 0;
    int num_of_prints_so_far = 0;
    bool print_pre = false;
    bool print_next = false;
    while (1) {
        size_t bytes_read;
        line_struct line = { NULL,0,count_line_number,byts_count_file,
                             num_of_prints_next,num_of_prints_so_far,false,false,print_next,print_pre};
        bytes_read = getline(&line.string, &line.line_len, myfile);
        int bytes_read_int = bytes_read;
        if (bytes_read_int == -1) {
            if (arguments.type_c == true)
            {
                printf("%d\n", count_prints);
            }
            fclose(myfile);
            free(line.string);
            return 0;
        }
        count_line_number++;
        line.line_number = count_line_number;
        line = execute(arguments, line);
        if (line.print_flag == true)
        {
            count_prints++;
            print_pre = true;
        }
        num_of_prints_so_far = count_prints;
        print_next = line.print_next;
        if (line.print_next==false){
            print_pre = false;
        }
        num_of_prints_next = line.number_of_next_prints;
        byts_count_file = byts_count_file + strlen(line.string);
        free(line.string);
    }
    fclose(myfile);
    return 0;
}
