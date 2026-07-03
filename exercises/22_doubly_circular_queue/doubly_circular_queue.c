#include "doubly_circular_queue.h"

#include <stdlib.h>

// 头尾哨兵
static struct node tailsentinel;
static struct node headsentinel = {0, NULL, &tailsentinel};
static struct node tailsentinel = {0, &headsentinel, NULL};

static link head = &headsentinel;
static link tail = &tailsentinel;

link make_node(int data) {
    // TODO: 在这里添加你的代码
    link p = (link)malloc(sizeof(struct node));
    if (p == NULL) return NULL;
    p->data = data;
    p->prev = NULL;
    p->next = NULL;
    return p;
}

void free_node(link p) {
    // TODO: 在这里添加你的代码
    if (p != NULL) {
        free(p);
    }
}

link search(int key) {
    // TODO: 在这里添加你的代码
    link p = head->next;
    while (p != tail) {
        if (p->data == key) {
            return p;
        }
        p = p->next;
    }
    return NULL;
}

void insert(link p) {
    // TODO: 在这里添加你的代码
    if (p == NULL) return;
    // 调整指针：新节点的前驱/后继
    p->next = head->next;
    p->prev = head;
    // 原头后节点的前驱指向新节点
    head->next->prev = p;
    // 头哨兵的后继指向新节点
    head->next = p;
}

void delete(link p) {
    // TODO: 在这里添加你的代码
    if (p == NULL || p == head || p == tail) return;
    // 调整前后节点的指针
    p->prev->next = p->next;
    p->next->prev = p->prev;
    // 重置被删节点的指针（避免野指针）
    p->prev = NULL;
    p->next = NULL;
}

void traverse(void (*visit)(link)) {
    // TODO: 在这里添加你的代码
    if (visit == NULL) return;
    link p = head->next;
    while (p != tail) {
        visit(p);
        p = p->next;
    }
}

void destroy(void) {
    // TODO: 在这里添加你的代码
    link p = head->next;
    link q;
    while (p != tail) {
        q = p->next;
        free_node(p);
        p = q;
    }
    // 重置哨兵指针，恢复初始状态
    head->next = tail;
    tail->prev = head;
}
