#ifndef PARSER_H
#define PARSER_H

#include "redirect.h"


typedef struct {
    char* name;
    char** args;
    int argc;
    int is_background;
    char* input_file;
    char* output_file;
    int is_piped;
    RedirectType redirect_type;
} Command;

// 사용자 입력 파싱하기
Command parse_input(char *input);

// 메모리 공간 생성
char* createCharMem();

// cmd free
void freeCommand(Command* cmd);
#endif