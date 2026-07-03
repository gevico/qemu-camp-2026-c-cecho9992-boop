#include <stdio.h>
#include <stdbool.h>

#define MAX_PEOPLE 50

typedef struct {
    int data[MAX_PEOPLE];
    int head;
    int tail;
    int count;
} Queue;

void InitQueue(Queue *q) {
    q->head = 0;
    q->tail = 0;
    q->count = 0;
}

bool EnQueue(Queue *q, int x) {
    if (q->count >= MAX_PEOPLE) return false;
    q->data[q->tail] = x;
    q->tail = (q->tail + 1) % MAX_PEOPLE;
    q->count++;
    return true;
}

bool DeQueue(Queue *q, int *x) {
    if (q->count <= 0) return false;
    *x = q->data[q->head];
    q->head = (q->head + 1) % MAX_PEOPLE;
    q->count--;
    return true;
}

int main() {
    Queue q;
    InitQueue(&q);

    int n = 50;
    int step = 5;

    for (int i = 1; i <= n; i++) {
        EnQueue(&q, i);
    }

    int count = 0;
    int x;
    int eliminated[50];
    int elim_cnt = 0;

    while (q.count > 1) {
        DeQueue(&q, &x);
        count++;

        if (count == step) {
            printf("淘汰: %d\n", x);   // ⭐测试必须要这一行格式
            eliminated[elim_cnt++] = x;
            count = 0;
        } else {
            EnQueue(&q, x);
        }
    }

    printf("最后剩下的人是: %d\n", q.data[q.head]);

    return 0;
}