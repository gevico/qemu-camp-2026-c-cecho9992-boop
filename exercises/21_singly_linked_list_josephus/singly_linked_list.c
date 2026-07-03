#include "singly_linked_list.h"

#include <stdio.h>
#include <stdlib.h>

// 全局头指针
static link head = NULL;

// 创建新节点
link make_node(unsigned char item) {
    // TODO: 在这里添加你的代码
    link p = (link)malloc(sizeof(struct node));
    if (p == NULL) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
    p->item = item;
    p->next = NULL;
    return p;
}

// 释放节点
void free_node(link p) { free(p); }

// 查找节点
link search(unsigned char key) {
    // TODO: 在这里添加你的代码
    link p = head;
    while (p != NULL && p->item != key) {
        p = p->next;
    }
    return p;
}

// 在链表头部插入节点
void insert(link p) {
    // TODO: 在这里添加你的代码
    p->next = head;
    head = p;
}

// 删除指定节点
void delete(link p) {
    // TODO: 在这里添加你的代码
    if (head == NULL || p == NULL) return;

    // 若删除的是头节点
    if (head == p) {
        head = p->next;
        free_node(p);
        return;
    }

    // 查找前驱节点
    link prev = head;
    while (prev != NULL && prev->next != p) {
        prev = prev->next;
    }

    // 找到前驱节点则删除
    if (prev != NULL) {
        prev->next = p->next;
        free_node(p);
    }
}

// 遍历链表
void traverse(void (*visit)(link)) {
    // TODO: 在这里添加你的代码
    link p = head;
    while (p != NULL) {
        visit(p);
        p = p->next;
    }
}

// 销毁整个链表
void destroy(void) {
    // TODO: 在这里添加你的代码
    link p = head;
    while (p != NULL) {
        link temp = p;
        p = p->next;
        free_node(temp);
    }
    head = NULL;
}

// 在链表头部推入节点
void push(link p) { 
    // TODO: 在这里添加你的代码
    insert(p);
 }

// 从链表头部弹出节点
link pop(void) {
    // TODO: 在这里添加你的代码
    if (head == NULL) return NULL;

    link temp = head;
    head = head->next;
    temp->next = NULL;  // 断开指针
    return temp;
}

// 释放链表内存
void free_list(link list_head) {
    // TODO: 在这里添加你的代码
    link p = list_head;
    while (p != NULL) {
        link temp = p;
        p = p->next;
        free_node(temp);
    }
}
