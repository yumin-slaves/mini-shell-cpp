#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

// 1차원 char 생성하기
char* createCharMem() {
    char* temp = (char*)malloc(sizeof(char) * MAX_COMMAND_SIZE);
    if (!temp) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    return temp;
}

// 사용자 입력 파싱하기
Command parse_single_command(char *input) {
    int capacity = 2;

    // cmd 초기화
    Command cmd;
    cmd.name = createCharMem();
    cmd.args = (char**)malloc(sizeof(char*) * capacity);
    if (!cmd.args) {
        fprintf(stderr, "메모리 할당 실패\n");
        exit(EXIT_FAILURE);
    }
    cmd.argc = 0;

    char* ptr = strtok(input, " ");

    if (ptr == NULL) {
        printf("명령이 제대로 입력되지 않았습니다");
        return cmd;
    }

    // 명령어 처리
    strcpy(cmd.name, ptr);
    cmd.args[cmd.argc] = createCharMem();
    strcpy(cmd.args[cmd.argc++], cmd.name);

    while ((ptr = strtok(NULL, " ")) != NULL) {
        if (cmd.argc == capacity) { // args is full
            capacity *= 2;
            cmd.args = realloc(cmd.args, sizeof(char*) * capacity);
            if (!cmd.args) {
                fprintf(stderr, "메모리 재할당 실패\n");
                exit(EXIT_FAILURE);
            }
        }
        cmd.args[cmd.argc] = createCharMem();
        strcpy(cmd.args[cmd.argc++], ptr);
    }

    // 마지막 인자는 NULL
    cmd.args[cmd.argc] = NULL;

    return cmd;
}

// 전체 입력 파싱 (파이프 처리 포함)
Command* parse_input(char *input, int* num_cmds) {
    int capacity = 2;
    *num_cmds = 0;

    Command* cmds = malloc(sizeof(Command) * capacity);
    if (!cmds) {
        fprintf(stderr, "Command 배열 메모리 할당 실패\n");
        exit(EXIT_FAILURE);
    }

    char* saveptr;
    char* token = strtok_r(input, "|", &saveptr);
    while (token != NULL) {
        if (*num_cmds == capacity) {
            capacity *= 2;
            cmds = realloc(cmds, sizeof(Command) * capacity);
            if (!cmds) {
                fprintf(stderr, "Command 배열 메모리 재할당 실패\n");
                exit(EXIT_FAILURE);
            }
        }

        // 앞뒤 공백 제거
        while (*token == ' ') token++;
        char* end = token + strlen(token) - 1;
        while (end > token && (*end == ' ' || *end == '\n')) *end-- = '\0';

        cmds[*num_cmds] = parse_single_command(token);
        (*num_cmds)++;

        token = strtok_r(NULL, "|", &saveptr);
    }

    return cmds;
}

// Command 메모리 해제 함수
void freeCommand(Command* cmd) {
    free(cmd->name);
    for (int i = 0; i < cmd->argc; i++) {
        free(cmd->args[i]);
    }
    free(cmd->args);
}

void freeCommands(Command* cmds, int num_cmds) {
    for (int i = 0; i < num_cmds; i++) {
        freeCommand(&cmds[i]);
    }
    free(cmds);
}