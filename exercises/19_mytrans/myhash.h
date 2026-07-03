#ifndef MYHASH_H
#define MYHASH_H

#include <stdint.h>

#define HASH_TABLE_SIZE 1048576  // 百万级哈希表大小（2^20）

// 哈希表节点
typedef struct HashNode {
    char *key;                // 单词（小写）
    char *value;              // 翻译内容
    struct HashNode *next;    // 链表解决冲突
} HashNode;

// 哈希表结构体
typedef struct {
    HashNode *buckets[HASH_TABLE_SIZE];
} HashTable;

// 哈希表操作函数
unsigned long hash_function(const char *str);
HashTable *create_hash_table();
void free_node(HashNode *node);
void free_hash_table(HashTable *table);
int hash_table_insert(HashTable *table, const char *key, const char *value);
const char *hash_table_lookup(HashTable *table, const char *key);

// 翻译器辅助函数
void trim(char *str);
int load_dictionary(const char *filename, HashTable *table, uint64_t *dict_count);

#endif
