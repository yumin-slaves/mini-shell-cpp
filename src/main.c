/* main.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "executor.h"

void print_prompt();
char* read_input();

int main() {
    while (1) {
        print_prompt();
        char *input = read_input();
        if (input == NULL) {
            break;
        }

        // Command cmd = parse_input(input);
        free(input);
        // if (strcmp(cmd.name, "exit") == 0) {
        //     break;
        // }
        // execute_command(cmd);
    }

    printf("\nBye!\n");
    return 0;
}

void print_prompt() {
    printf("mysh> ");
    fflush(stdout);
}

char* read_input() {
    int size = 128; // initial size
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