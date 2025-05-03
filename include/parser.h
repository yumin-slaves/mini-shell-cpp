#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>

#define MAX_COMMAND_SIZE 1024

typedef struct Command{
    char* name;
    char **args;
    int argc;

    bool is_background;
}Command;

// 사용자 입력 파싱하기
Command parse_input(char *input);

// 메모리 공간 생성
char* create_char_mem();

// cmd free
void freeCommand(Command* cmd);
#endif