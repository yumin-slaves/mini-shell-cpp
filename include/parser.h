#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>
#include "redirect.h"

#define MAX_COMMAND_SIZE 1024

typedef struct {
    char* name;
    char** args;
    int argc;

    char* input_file;
    char* output_file;
    bool is_background;
    RedirectType redirect_type;
} Command;

// 사용자 입력 파싱하기
Command parse_single_command(char *input);

// 메모리 공간 생성
char* create_char_mem();

Command* parse_input(char *input, int* num_cmds);

// cmd free
void free_command(Command* cmd);
void free_commands(Command* cmds, int num_cmds);
#endif