#ifndef PARSER_H
#define PARSER_H

#define MAX_COMMAND_SIZE 1024


typedef struct Command{
    char* name;
    char **args;
    int argc;
}Command;

// 사용자 입력 파싱하기
Command* parse_input(char *input, int* num_cmds);

// 메모리 공간 생성
char* createCharMem();

// cmd free
void freeCommands(Command* cmds, int num_cmds);
#endif