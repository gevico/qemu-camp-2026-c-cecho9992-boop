#include "mywc.h"

// 创建哈希表
WordCount **create_hash_table() {
  WordCount **hash_table = calloc(HASH_SIZE, sizeof(WordCount *));
  if (!hash_table) return NULL;
  return hash_table;
}

// 简单的哈希函数
unsigned int hash(const char *word) {
  unsigned long hash = 5381;
  int c;
  while ((c = *word++))
    hash = ((hash << 5) + hash) + c; // hash * 33 + c
  return hash % HASH_SIZE;
}

// 检查字符是否构成单词的一部分
bool is_valid_word_char(char c) { return isalpha(c) || c == '\''; }

// 转换为小写
char to_lower(char c) { return tolower(c); }

// 添加单词到哈希表
void add_word(WordCount **hash_table, const char *word) {
    if (!hash_table || !word) return;
    unsigned int idx = hash(word);
    WordCount *cur = hash_table[idx];
    // 遍历链表，单词已存在则计数+1
    while (cur != NULL) {
        if (strcmp(cur->word, word) == 0) {
            cur->count++;
            return;
        }
        cur = cur->next;
    }
    // 新建节点插入链表头部
    WordCount *new_node = malloc(sizeof(WordCount));
    if (!new_node) return;
    strncpy(new_node->word, word, MAX_WORD_LEN);
    new_node->word[MAX_WORD_LEN - 1] = '\0';
    new_node->count = 1;
    new_node->next = hash_table[idx];
    hash_table[idx] = new_node;
}

// 打印单词统计结果
void print_word_counts(WordCount **hash_table) {
    printf("Word Count Statistics:\n");
    printf("======================\n");
    for (int i = 0; i < HASH_SIZE; i++) {
        WordCount *p = hash_table[i];
        while (p != NULL) {
            printf("%-20s %d\n", p->word, p->count);   // 改为宽度20
            p = p->next;
        }
    }
}

// 释放哈希表内存
void free_hash_table(WordCount **hash_table) {
    if (hash_table == NULL) return;
    for (int i = 0; i < HASH_SIZE; i++) {
        WordCount *cur = hash_table[i];
        while (cur != NULL) {
            WordCount *tmp = cur;
            cur = cur->next;
            free(tmp);
        }
    }
    free(hash_table);
}

// 处理文件并统计单词
int process_file(const char *filename) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    perror("Error opening file");
    return -1;
  }
  WordCount **hash_table = create_hash_table();
  if (!hash_table) {
      fclose(file);
      return -1;
  }
  char word[MAX_WORD_LEN];
  int word_pos = 0;
  int c;
  while ((c = fgetc(file)) != EOF) {
    if (is_valid_word_char(c)) {
      if (word_pos < MAX_WORD_LEN - 1) {
        word[word_pos++] = to_lower(c);
      }
    } else {
      if (word_pos > 0) {
        word[word_pos] = '\0';
        add_word(hash_table, word);
        word_pos = 0;
      }
    }
  }
  // 处理文件末尾的最后一个单词
  if (word_pos > 0) {
    word[word_pos] = '\0';
    add_word(hash_table, word);
  }
  fclose(file);
  print_word_counts(hash_table);
  free_hash_table(hash_table);
  return 0;
}
