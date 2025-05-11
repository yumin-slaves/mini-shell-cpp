#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"

/**
 *  @brief 입력받은 문자열을 저장할 메모리 공간을 생성한다
 * 
 *  @param input 공간을 만들 대상 문자열
 *  @return 메모리 공간을 가리키는 포인터
 */
char* create_char_mem(char *input) {
    char* temp = (char*)malloc(sizeof(char) * (strlen(input) + 1));
    if (!temp) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    strcpy(temp, input);
    return temp;
}

/**
 *  @brief 입력받은 문자열을 파싱한다.
 * 
 *  @param input 파싱할 문자열
 *  @return 파싱 결과를 담은 Command
 */
Command parse_input(char *input) {
    // 초기 args 크기 설정
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
    cmd.input_file = NULL;
    cmd.output_file = NULL;
    cmd.is_background = false;

    char* ptr = strtok(input, " ");

    if (ptr == NULL) {
        fprintf(stderr,"명령이 제대로 입력되지 않았습니다");
        return cmd;
    }

    // 명령어 이름을 저장
    cmd.name = create_char_mem(ptr);
    cmd.args[cmd.argc++] = create_char_mem(cmd.name);

    while ((ptr = strtok(NULL, " ")) != NULL) {
        //args에 공간이 없을 경우
        if (cmd.argc == capacity) { 
            capacity *= 2;
            cmd.args = realloc(cmd.args, sizeof(char*) * capacity);
            if (!cmd.args) {
                fprintf(stderr, "메모리 재할당 실패\n");
                exit(EXIT_FAILURE);
            }
        }

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

    // NULL을 추가할 공간 생성
    if (cmd.argc >= capacity) { // args에 공간이 없을 경우
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

/**
 *  @brief Command 구조체 내부 동적 메모리 해제
 * 
 *  @param cmd 해제할 구조체의 포인터
 */
void free_command(Command* cmd) {
    free(cmd->name);
    for (int i = 0; i < cmd->argc; i++) {
        free(cmd->args[i]);
    }
    free(cmd->args);

    if (cmd->input_file) free(cmd->input_file);
    if (cmd->output_file) free(cmd->output_file);
}
