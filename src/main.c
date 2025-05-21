/* main.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <termios.h>

#include "parser.h"
#include "executor.h"
#include "history.h"

// raw mode 실행
void enable_raw_mode(struct termios* orig_termios) {
    struct termios raw;
    tcgetattr(STDIN_FILENO, orig_termios);
    raw = *orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

// raw mode 종료
void disable_raw_mode(struct termios* orig_termios) {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, orig_termios);
}

history* create_history(){
    history* history_list = (history*)malloc(sizeof(history));
    
    history_list->head = (history_node*)malloc(sizeof(history_node));
    history_list->tail = (history_node*)malloc(sizeof(history_node));
    history_list->head->command = "";
    history_list->tail->command = "";
    history_list->head->prev = NULL;
    history_list->head->next = history_list->tail;
    history_list->tail->prev = history_list->head;
    history_list->tail->next = NULL;

    return history_list;
}

// history_list에 명령어 추가하기
void add_history_node(history** history_list, char* command) {
    history_node* x = (history_node*)malloc(sizeof(history_node));
    x->command = strdup(command);

    // tail 앞에 삽입
    x->prev = (*history_list)->tail->prev;
    x->next = (*history_list)->tail;

    (*history_list)->tail->prev->next = x;
    (*history_list)->tail->prev = x;

    // 새 명령어가 추가되었으므로 curr는 tail로 초기화
    (*history_list)->curr = (*history_list)->tail;
}

void free_history_list(history* history_list) {
    history_node* curr = history_list->head->next;

    while (curr != NULL && curr != history_list->tail) {
        history_node* next = curr->next;

        if (curr->command != NULL) {
            free(curr->command); 
        }

        free(curr);
        curr = next;
    }

    free(history_list);
}


/**
* @brief Reads a line of input from the user.
* @return A pointer to a string containing the input line, or NULL on EOF or error.
*/
char* read_input(history* history_list) {
    struct termios orig_termios;
    enable_raw_mode(&orig_termios);

    char* buffer = malloc(1024);
    int len = 0;
    char c;
    while (1) {
        read(STDIN_FILENO, &c, 1);
        if (c == '\n') {
            buffer[len] = '\0';
            if (strcmp(buffer, "")) {
                add_history_node(&history_list, buffer);
            }
            printf("\n");
            break;
        } else if (c == 127 || c == '\b') {
            if (len > 0) {
                len--;
                write(STDOUT_FILENO, "\b \b", 3);
            }
        } else if (c == 27) { 
            char seq[2];
            read(STDIN_FILENO, &seq[0], 1);
            read(STDIN_FILENO, &seq[1], 1);
            if (seq[0] == '[') {

                if (history_list->head->next == history_list->tail) {
                     continue;
                }

                if (seq[1] == 'A') {
                    if (history_list->curr->prev != history_list->head) {
                        // 기존 입력 지우기
                        for (int i = 0; i < strlen(history_list->curr->command); i++) {
                            write(STDOUT_FILENO, "\b \b", 3);
                        }
                        history_list->curr = history_list->curr->prev;
                        strcpy(buffer, history_list->curr->command);
                        len = strlen(buffer);
                        write(STDOUT_FILENO, buffer, len);
                    }
                } else if (seq[1] == 'B') { 

                    if (history_list->curr == history_list->tail) {
                        continue;;
                    }

                    if (history_list->curr->next != history_list->tail) {
                        // 기존 입력 지우기
                        for (int i = 0; i < strlen(history_list->curr->command); i++) {
                            write(STDOUT_FILENO, "\b \b", 3);
                        }
                        history_list->curr = history_list->curr->next;
                        strcpy(buffer, history_list->curr->command);
                        len = strlen(buffer);
                        write(STDOUT_FILENO, buffer, len);
                    } else {
                        for (int i = 0; i < strlen(history_list->curr->command); i++) {
                            write(STDOUT_FILENO, "\b \b", 3);
                        }
                        history_list->curr = history_list->curr->next;
                        strcpy(buffer, "");
                        len = strlen("");
                    }
                }
            }
        } else {
            buffer[len++] = c;
            write(STDOUT_FILENO, &c, 1);
        }
    }

    disable_raw_mode(&orig_termios);
    return buffer;
}

void save_history_to_file(history* history_list, const char* filename) {
    FILE* fp = fopen(filename, "w");
    if (!fp) return;

    history_node* curr = history_list->head->next;
    while (curr != history_list->tail) {
        fprintf(fp, "%s\n", curr->command);
        curr = curr->next;
    }

    fclose(fp);
}

void load_history_from_file(history** history_list, const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (!fp) return;

    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), fp)) {
        buffer[strcspn(buffer, "\n")] = 0;
        add_history_node(history_list, buffer);
    }

    fclose(fp);
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

    history* history_list = create_history();
    load_history_from_file(&history_list, "history.txt");

    while (true) {
        print_prompt();
        char *input = read_input(history_list);
        if (input == NULL) {
            break;
        }

        int num_cmds;
        Command* cmds = parse_input(input, &num_cmds);

        if (!strcmp(input, "exit")) {
            free(input);
            free_commands(cmds, num_cmds);
            save_history_to_file(history_list, "history.txt");
            free_history_list(history_list);
            printf("Bye!\n");
            break;
        }
        
        free(input);
        execute_commands(cmds, num_cmds);
        free_commands(cmds, num_cmds);
    }

    return 0;
}