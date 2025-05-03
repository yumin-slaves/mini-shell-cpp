#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

// 1차원 char 생성하기
char* create_char_mem() {
    char* temp = (char*)malloc(sizeof(char) * MAX_COMMAND_SIZE);
    if (!temp) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    return temp;
}

// 사용자 입력 파싱하기
Command parse_input(char *input) {
    int capacity = 2;

    // cmd 초기화
    Command cmd;
    cmd.name = create_char_mem();
    cmd.args = (char**)malloc(sizeof(char*) * capacity);
    if (!cmd.args) {
        fprintf(stderr, "메모리 할당 실패\n");
        exit(EXIT_FAILURE);
    }
    cmd.argc = 0;
    cmd.is_background = 0;

    char* ptr = strtok(input, " ");

    if (ptr == NULL) {
        printf("명령이 제대로 입력되지 않았습니다");
        return cmd;
    }

    // 명령어 처리
    strcpy(cmd.name, ptr);
    cmd.args[cmd.argc] = create_char_mem();
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
        cmd.args[cmd.argc] = create_char_mem();
        strcpy(cmd.args[cmd.argc++], ptr);
    }

    /* 마지막 인자가 "&"일 시
    * cmd.is_background = true,
    * 파싱 결과에서 삭제
    */ 
    if (strcmp(cmd.args[cmd.argc - 1], "&") == 0) {
        cmd.is_background = true;
        cmd.argc -= 1;
    }

    // 마지막 인자는 NULL
    cmd.args[cmd.argc] = NULL;

    return cmd;
}

// Command 메모리 해제 함수
void freeCommand(Command* cmd) {
    free(cmd->name);
    for (int i = 0; i < cmd->argc; i++) {
        free(cmd->args[i]);
    }
    free(cmd->args);
}
