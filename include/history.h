#ifndef HISTORY_H
#define HISTORY_H

#include <stdio.h>
#include <stdlib.h>

typedef struct History_node{

    char* command;
    struct History_node* prev;
    struct History_node* next;
}history_node;

typedef struct History{

    history_node* head;
    history_node* tail;
    history_node* curr;
}history;

history* create_history();
void add_history_node(history** history_list, char* commad);
void free_history_list(history* history_list);

#endif