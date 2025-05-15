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
Command parse_single_command(char *input) {
    int capacity = 2;

    Command cmd;
    cmd.name = create_char_mem();
    cmd.args = (char**)malloc(sizeof(char*) * capacity);
    cmd.argc = 0;
    cmd.input_file = NULL;
    cmd.output_file = NULL;
    cmd.is_background = false;
    cmd.redirect_type = NO_REDIRECT;

    int len = strlen(input);
    int i = 0;
    while (i < len) {
        while (i < len && (input[i] == ' ' || input[i] == '\t')) i++;
        if (i >= len) break;

        // 리디렉션 기호 확인
        if (input[i] == '<' || input[i] == '>') {
            int redirect_type = 0;
            if (input[i] == '<') {
                redirect_type = REDIRECT_INPUT;
                i++;
            } else if (input[i] == '>') {
                i++;
                if (input[i] == '>') {
                    redirect_type = REDIRECT_APPEND;
                    i++;
                } else {
                    redirect_type = REDIRECT_OUTPUT;
                }
            }

            while (i < len && (input[i] == ' ' || input[i] == '\t')) i++;

            char* filename = create_char_mem();
            int file_index = 0;
            while (i < len && input[i] != ' ' && input[i] != '\t') {
                filename[file_index++] = input[i++];
            }
            filename[file_index] = '\0';

            if (redirect_type == REDIRECT_INPUT) {
                cmd.input_file = filename;
            } else {
                cmd.output_file = filename;
            }
            cmd.redirect_type = redirect_type;
            continue;
        }

        // 일반 인자 처리
        char* arg = create_char_mem();
        int arg_index = 0;
        char quote = 0;

        if (input[i] == '"' || input[i] == '\'') {
            quote = input[i++];
            while (i < len && input[i] != quote) {
                arg[arg_index++] = input[i++];
            }
            if (i < len && input[i] == quote) {
                i++;
            }
        } else {
            while (i < len && input[i] != ' ' && input[i] != '\t' && input[i] != '>' && input[i] != '<') {
                arg[arg_index++] = input[i++];
            }
        }

        arg[arg_index] = '\0';

        if (cmd.argc == capacity) {
            capacity *= 2;
            cmd.args = realloc(cmd.args, sizeof(char*) * capacity);
        }

        cmd.args[cmd.argc] = arg;
        if (cmd.argc == 0) strcpy(cmd.name, arg);
        cmd.argc++;
    }

    if (strcmp(cmd.args[cmd.argc - 1], "&") == 0) {
        cmd.is_background = true;
        cmd.argc -= 1;
    }

    cmd.args = realloc(cmd.args, sizeof(char*) * (cmd.argc + 1));
    cmd.args[cmd.argc] = NULL;

    return cmd;
}



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
        // 앞뒤 공백 제거
        while (*token == ' ') token++;
        char* end = token + strlen(token) - 1;
        while (end >= token && (*end == ' ' || *end == '\n')) *end-- = '\0';

        // 토큰이 완전히 비어 있는 경우 무시
        if (strlen(token) == 0) {
            token = strtok_r(NULL, "|", &saveptr);
            continue;
        }

        if (*num_cmds == capacity) {
            capacity *= 2;
            cmds = realloc(cmds, sizeof(Command) * capacity);
            if (!cmds) {
                fprintf(stderr, "Command 배열 메모리 재할당 실패\n");
                exit(EXIT_FAILURE);
            }
        }

        cmds[*num_cmds] = parse_single_command(token);
        (*num_cmds)++;

        token = strtok_r(NULL, "|", &saveptr);
    }

    return cmds;
}

// Command 메모리 해제 함수
void free_command(Command* cmd) {
    free(cmd->name);
    for (int i = 0; i < cmd->argc; i++) {
        free(cmd->args[i]);
    }
    free(cmd->args);
    free(cmd->input_file);
    free(cmd->output_file);
}

void free_commands(Command* cmds, int num_cmds) {
    for (int i = 0; i < num_cmds; i++) {
        free_command(&cmds[i]);
    }
    free(cmds);
}