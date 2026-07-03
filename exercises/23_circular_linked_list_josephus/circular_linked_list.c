#include "circular_linked_list.h"

#include <stdio.h>
#include <stdlib.h>

Node* create_circular_list(int n) {
    // TODO: 在这里添加你的代码
     if (n <= 0) return NULL;
    Node* head = NULL;
    Node* tail = NULL;
    for (int i = 1; i <= n; i++) {
        Node* new_node = (Node*)malloc(sizeof(Node));
        new_node->id = i;
        new_node->next = NULL;
        if (head == NULL) {
            head = new_node;
            tail = new_node;
        } else {
            tail->next = new_node;
            tail = new_node;
        }
    }
    // 尾节点指向头，形成环
    tail->next = head;
    return head;
}

void free_list(Node* head) {
    // TODO: 在这里添加你的代码
    if (head == NULL) return;
    Node* cur = head;
    Node* next;
    do {
        next = cur->next;
        free(cur);
        cur = next;
    } while (cur != head);
}
