/* main.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#include "parser.h"
#include "executor.h"

/**
* @brief Reads a line of input from the user.
* @return A pointer to a string containing the input line, or NULL on EOF or error.
*/
char* read_input() {
    // 초기 사이즈를 128로 설정
    int size = 128;
    int len = 0;

    char *buffer = malloc(size);
    if (buffer == NULL) {
        perror("malloc");
        exit(1);
    }

    int ch;
    // 버퍼의 크기가 부족할 시 갱신
    while ((ch = getchar()) != EOF && ch != '\n') {
        buffer[len++] = ch;

        if (len >= size) {
            size *= 2;
            char *new_buffer = realloc(buffer, size);
            if (new_buffer == NULL) {
                perror("realloc");
                exit(1);
            }
            buffer = new_buffer;
        }
    }

    buffer[len] = '\0';

    // 입력 문자열이 EOF(Ctrl + D) 일때는 NULL 반환
    if (len == 0 && ch == EOF) {
        free(buffer);
        return NULL;
    }
    
    return buffer;
}

/**
 * @brief Print prompt with flush.
 */
void print_prompt() {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("mysh::%s> ", cwd);
    } else {
        printf("mysh::~> ");
    }
    fflush(stdout);
}

int main() {
    while (true) {
        print_prompt();
        char *input = read_input();
        if (input == NULL) {
            break;
        }

        int num_cmds;
        Command* cmds = parse_input(input, &num_cmds);
        free(input);

        execute_commands(cmds, num_cmds);
        free_commands(cmds, num_cmds);
    }

    printf("\nBye!\n");
    return 0;
}