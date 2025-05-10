#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"

char* create_char_mem(char *input) {
    char* temp = (char*)malloc(sizeof(char) * (strlen(input) + 1));
    if (!temp) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    strcpy(temp, input);
    return temp;
}

// 사용자 입력 파싱하기
Command parse_input(char *input) {
    int capacity = 2;

    // cmd 초기화
    Command cmd;
    cmd.redirect_type = NO_REDIRECT;
    cmd.args = (char**)malloc(sizeof(char*) * capacity);
    if (!cmd.args) {
        fprintf(stderr, "메모리 할당 실패\n");
        exit(EXIT_FAILURE);
    }
    cmd.argc = 0;
    cmd.is_background = false;

    char* ptr = strtok(input, " ");

    if (ptr == NULL) {
        printf("명령이 제대로 입력되지 않았습니다");
        return cmd;
    }

    // 명령어 처리
    cmd.name = create_char_mem(ptr);
    cmd.args[cmd.argc++] = create_char_mem(cmd.name);

    while ((ptr = strtok(NULL, " ")) != NULL) {
        if (cmd.argc == capacity) { // args is full
            capacity *= 2;
            cmd.args = realloc(cmd.args, sizeof(char*) * capacity);
            if (!cmd.args) {
                fprintf(stderr, "메모리 재할당 실패\n");
                exit(EXIT_FAILURE);
            }
        }

        // 파일명이 존재하지 않을 때 오류 처리하기
        if(!strcmp(ptr, ">")){
            ptr = strtok(NULL, " ");
            if(ptr){
                cmd.output_file = create_char_mem(ptr);
            }
            cmd.redirect_type = REDIRECT_OUTPUT;
            
        }
        else if(!strcmp(ptr, ">>")){
            ptr = strtok(NULL, " ");
            if(ptr){
                cmd.output_file = create_char_mem(ptr);
            }
            cmd.redirect_type = REDIRECT_APPEND;
        }
        else if(!strcmp(ptr, "<")){
            ptr = strtok(NULL, " ");
            if(ptr){
                cmd.input_file = create_char_mem(ptr);
            }
            cmd.redirect_type = REDIRECT_INPUT;
        }
        else{
            cmd.args[cmd.argc++] = create_char_mem(ptr);
        }
    }

    if (cmd.argc >= capacity) { // args is full
        cmd.args = realloc(cmd.args, sizeof(char*) * (capacity + 1));
        if (!cmd.args) {
            fprintf(stderr, "메모리 재할당 실패\n");
            exit(EXIT_FAILURE);
        }
    }

    // 마지막 인자가 "&"이면 파싱 결과에서 삭제
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
