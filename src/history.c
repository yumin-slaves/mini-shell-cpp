#include <unistd.h>
#include <string.h>

#include "history.h"


history* create_history(){
    history* history_list = (history*)malloc(sizeof(history));
    
    history_list->head = (history_node*)malloc(sizeof(history_node));
    history_list->tail = (history_node*)malloc(sizeof(history_node));
    history_list->head->command = strdup("");
    history_list->tail->command = strdup("");
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