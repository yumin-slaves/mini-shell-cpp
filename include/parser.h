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
    bool is_piped;
    RedirectType redirect_type;
} Command;

// 사용자 입력 파싱하기
Command parse_input(char *input);

// 메모리 공간 생성
char* create_char_mem();

// cmd free
void freeCommand(Command* cmd);
#endif