#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * 16 LRU 缓存淘汰算法（哈希表 + 双向链表）
 *  - put(k,v)、get(k) 均为 O(1)
 *  - 容量满时淘汰最久未使用（LRU）的元素
 */

typedef struct LRUNode {
    int key;
    int value;
    struct LRUNode* prev;
    struct LRUNode* next;
} LRUNode;

typedef struct HashEntry {
    int key;
    LRUNode* node;
    struct HashEntry* next;
} HashEntry;

typedef struct {
    int capacity;
    int size;
    LRUNode* head; /* 最近使用（MRU） */
    LRUNode* tail; /* 最久未使用（LRU） */
    /* 简单链式哈希表 */
    size_t bucket_count;
    HashEntry** buckets;
} LRUCache;

static unsigned hash_int(int key) {
    // TODO: 在这里添加你的代码
    unsigned ukey = (unsigned)key;
    return ukey % 17; // 选用质数作为基础桶数，减少冲突
}

static HashEntry* hash_find(LRUCache* c, int key, HashEntry*** pprev_next) {
    // TODO: 在这里添加你的代码
    if (!c || !c->buckets || pprev_next == NULL) return NULL;
    
    unsigned idx = hash_int(key) % c->bucket_count;
    HashEntry** curr_next = &c->buckets[idx];
    *pprev_next = curr_next;
    
    while (*curr_next) {
        HashEntry* entry = *curr_next;
        if (entry->key == key) {
            return entry;
        }
        *pprev_next = curr_next;
        curr_next = &entry->next;
    }
    return NULL;
}

static void list_add_to_head(LRUCache* c, LRUNode* node) {
    // TODO: 在这里添加你的代码
    if (!c || !node) return;
    
    // 初始化节点的前后指针
    node->prev = NULL;
    node->next = c->head;
    
    // 如果原头部不为空，更新原头部的前驱
    if (c->head) {
        c->head->prev = node;
    }
    // 更新缓存头部为当前节点
    c->head = node;
    
    // 如果链表为空（tail为NULL），tail也指向该节点
    if (!c->tail) {
        c->tail = node;
    }
}

static void list_remove(LRUCache* c, LRUNode* node) {
    // TODO: 在这里添加你的代码
    if (!c || !node) return;
    
    LRUNode* prev = node->prev;
    LRUNode* next = node->next;
    
    // 更新前驱节点的next
    if (prev) {
        prev->next = next;
    } else {
        // 无前驱，说明是头节点，更新head
        c->head = next;
    }
    
    // 更新后继节点的prev
    if (next) {
        next->prev = prev;
    } else {
        // 无后继，说明是尾节点，更新tail
        c->tail = prev;
    }
}

static void list_move_to_head(LRUCache* c, LRUNode* node) {
    // TODO: 在这里添加你的代码
    if (!c || !node || c->head == node) return; // 已经是头节点则无需操作
    
    list_remove(c, node);
    list_add_to_head(c, node);
}

static LRUNode* list_pop_tail(LRUCache* c) {
    // TODO: 在这里添加你的代码
    if (!c || !c->tail) return NULL;
    
    LRUNode* tail_node = c->tail;
    list_remove(c, tail_node);
    return tail_node;
}

/* LRU 接口实现 */
static LRUCache* lru_create(int capacity) {
    // TODO: 在这里添加你的代码
    if (capacity <= 0) return NULL;
    
    LRUCache* c = (LRUCache*)malloc(sizeof(LRUCache));
    if (!c) return NULL;
    
    // 初始化缓存基本属性
    c->capacity = capacity;
    c->size = 0;
    c->head = NULL;
    c->tail = NULL;
    
    // 初始化哈希表（桶数选用质数，减少冲突）
    c->bucket_count = 17;
    c->buckets = (HashEntry**)calloc(c->bucket_count, sizeof(HashEntry*));
    if (!c->buckets) {
        free(c);
        return NULL;
    }
    
    return c;
}

static void lru_free(LRUCache* c) {
    // TODO: 在这里添加你的代码
    if (!c) return;
    
    // 释放双向链表节点
    LRUNode* curr_node = c->head;
    while (curr_node) {
        LRUNode* next = curr_node->next;
        free(curr_node);
        curr_node = next;
    }
    
    // 释放哈希表
    for (size_t i = 0; i < c->bucket_count; i++) {
        HashEntry* curr_entry = c->buckets[i];
        while (curr_entry) {
            HashEntry* next = curr_entry->next;
            free(curr_entry);
            curr_entry = next;
        }
    }
    free(c->buckets);
    free(c);
}

static int lru_get(LRUCache* c, int key, int* out_value) {
    // TODO: 在这里添加你的代码
    if (!c || !out_value) return 0;
    
    HashEntry** pprev_next;
    HashEntry* entry = hash_find(c, key, &pprev_next);
    if (!entry) return 0;
    
    // 找到节点，更新为最近使用
    list_move_to_head(c, entry->node);
    *out_value = entry->node->value;
    return 1;
}

static void lru_put(LRUCache* c, int key, int value) {
    // TODO: 在这里添加你的代码
    if (!c) return;
    
    HashEntry** pprev_next;
    HashEntry* entry = hash_find(c, key, &pprev_next);
    
    if (entry) {
        // 已存在该key：更新value并移到头部
        entry->node->value = value;
        list_move_to_head(c, entry->node);
        return;
    }
    
    // 不存在该key：创建新节点
    LRUNode* new_node = (LRUNode*)malloc(sizeof(LRUNode));
    if (!new_node) return;
    new_node->key = key;
    new_node->value = value;
    new_node->prev = NULL;
    new_node->next = NULL;
    
    // 创建哈希表条目
    HashEntry* new_entry = (HashEntry*)malloc(sizeof(HashEntry));
    if (!new_entry) {
        free(new_node);
        return;
    }
    new_entry->key = key;
    new_entry->node = new_node;
    new_entry->next = NULL;
    
    // 检查容量是否已满
    if (c->size >= c->capacity) {
        // 淘汰最久未使用节点（尾部）
        LRUNode* tail_node = list_pop_tail(c);
        if (tail_node) {
            // 从哈希表中删除该节点对应的entry
            HashEntry** pdel_prev;
            HashEntry* del_entry = hash_find(c, tail_node->key, &pdel_prev);
            if (del_entry) {
                *pdel_prev = del_entry->next;
                free(del_entry);
            }
            // 释放链表节点
            free(tail_node);
            c->size--;
        }
    }
    
    // 将新节点添加到链表头部
    list_add_to_head(c, new_node);
    // 将新条目添加到哈希表
    unsigned idx = hash_int(key) % c->bucket_count;
    new_entry->next = c->buckets[idx];
    c->buckets[idx] = new_entry;
    c->size++;
}

/* 打印当前缓存内容（从头到尾） */
static void lru_print(LRUCache* c) {
    LRUNode* p = c->head;
    int first = 1;
    while (p) {
        if (!first) printf(", ");
        first = 0;
        printf("%d:%d", p->key, p->value);
        p = p->next;
    }
    printf("\n");
}

int main(void) {
    /* 容量 3：put(1,1), put(2,2), put(3,3), put(4,4), get(2), put(5,5) */
    LRUCache* c = lru_create(3);
    if (!c) {
        fprintf(stderr, "创建 LRU 失败\n");
        return 1;
    }

    lru_put(c, 1, 1); /* 缓存：1 */
    lru_put(c, 2, 2); /* 缓存：2,1 */
    lru_put(c, 3, 3); /* 缓存：3,2,1 (满) */
    lru_put(c, 4, 4); /* 淘汰 LRU(1)，缓存：4,3,2 */

    int val;
    if (lru_get(c, 2, &val)) {
        /* 访问 2：缓存：2,4,3 */
        (void)val; /* 演示无需使用 */
    }

    lru_put(c, 5, 5); /* 淘汰 LRU(3)，缓存：5,2,4 */

    /* 期望最终键集合：{2,4,5}，顺序无关。此处按最近->最久打印：5:5, 2:2, 4:4 */
    lru_print(c);

    lru_free(c);
    return 0;
}
