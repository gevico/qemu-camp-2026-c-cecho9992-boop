#include "simple_tree.h"

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

Queue* create_queue() {
    Queue *q = (Queue*)malloc(sizeof(Queue));
    q->front = q->rear = NULL;
    return q;
}

void enqueue(Queue *q, TreeNode *tree_node) {
    // TODO: 在这里添加你的代码
    QueueNode *new_node = (QueueNode*)malloc(sizeof(QueueNode));
    new_node->tree_node = tree_node;
    new_node->next = NULL;

    if (q->rear == NULL) { // 队列为空时，头尾都指向新节点
        q->front = q->rear = new_node;
        return;
    }

    // 队列非空，尾节点指向新节点，更新尾指针
    q->rear->next = new_node;
    q->rear = new_node;
}

TreeNode* dequeue(Queue *q) {
    // TODO: 在这里添加你的代码
    if (is_empty(q)) { // 队列为空返回NULL
        return NULL;
    }

    // 保存头部节点，更新头指针
    QueueNode *temp = q->front;
    TreeNode *tree_node = temp->tree_node;
    q->front = q->front->next;

    // 若出队后队列为空，尾指针置空
    if (q->front == NULL) {
        q->rear = NULL;
    }

    free(temp); // 释放队列节点内存
    return tree_node;
}

bool is_empty(Queue *q) {
    return q->front == NULL;
}

void free_queue(Queue *q) {
    while (!is_empty(q)) {
        dequeue(q);
    }
    free(q);
}

TreeNode* build_tree_by_level(int *level_order, int size) {
    // TODO: 在这里添加你的代码
    if (size == 0 || level_order[0] == INT_MIN) {
        return NULL;
    }

    // 创建根节点
    TreeNode *root = (TreeNode*)malloc(sizeof(TreeNode));
    root->val = level_order[0];
    root->left = root->right = NULL;

    // 队列存储待处理的父节点
    Queue *q = create_queue();
    enqueue(q, root);

    int i = 1; // 从数组第二个元素开始处理
    while (!is_empty(q) && i < size) {
        TreeNode *curr = dequeue(q);

        // 处理左孩子
        if (i < size && level_order[i] != INT_MIN) {
            curr->left = (TreeNode*)malloc(sizeof(TreeNode));
            curr->left->val = level_order[i];
            curr->left->left = curr->left->right = NULL;
            enqueue(q, curr->left);
        }
        i++;

        // 处理右孩子
        if (i < size && level_order[i] != INT_MIN) {
            curr->right = (TreeNode*)malloc(sizeof(TreeNode));
            curr->right->val = level_order[i];
            curr->right->left = curr->right->right = NULL;
            enqueue(q, curr->right);
        }
        i++;
    }

    free_queue(q); // 释放队列
    return root;
}

void preorder_traversal(TreeNode *root) {
    // TODO: 在这里添加你的代码
    if (root == NULL) {
        return;
    }
    // 访问根节点
    printf("%d ", root->val);
    // 递归遍历左子树
    preorder_traversal(root->left);
    // 递归遍历右子树
    preorder_traversal(root->right);
}

void preorder_traversal_iterative(TreeNode *root) {
    // TODO: 在这里添加你的代码
    if (root == NULL) {
        return;
    }

    // 用数组模拟栈（简化实现，也可用链表）
    TreeNode *stack[100];
    int top = -1; // 栈顶指针，初始为-1（空栈）
    stack[++top] = root; // 根节点入栈

    while (top >= 0) {
        // 出栈并访问节点
        TreeNode *curr = stack[top--];
        printf("%d ", curr->val);

        // 先压右孩子（栈后进先出，保证左孩子先处理）
        if (curr->right != NULL) {
            stack[++top] = curr->right;
        }
        // 再压左孩子
        if (curr->left != NULL) {
            stack[++top] = curr->left;
        }
    }
}

void free_tree(TreeNode *root) {
    if (root == NULL) {
        return;
    }
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}
