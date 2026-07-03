// mytrans.c
#include "myhash.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 修剪字符串两端的空白字符（空格、制表符、换行符等）
void trim(char *str) {
  if (!str || *str == '\0') return;

  // 1. 去掉开头空白
  char *start = str;
  while (*start && isspace((unsigned char)*start)) {
    start++;
  }

  // 2. 去掉结尾空白
  char *end = str + strlen(str) - 1;
  while (end > start && isspace((unsigned char)*end)) {
    end--;
  }

  // 3. 重新构造字符串
  *(end + 1) = '\0';
  memmove(str, start, end - start + 2); // 包含终止符
}

int load_dictionary(const char *filename, HashTable *table,
                    uint64_t *dict_count) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("无法打开词典文件");
        return -1;
    }

    char line[1024];
    *dict_count = 0;
    char current_word[1024] = {0};  // 暂存当前单词

    while (fgets(line, sizeof(line), file) != NULL) {
        // 去掉换行符
        line[strcspn(line, "\n")] = '\0';
        trim(line);  // 去掉首尾空白

        if (strlen(line) == 0) continue;  // 空行跳过

        // 1) 处理以 '#' 开头的行 —— 提取单词
        if (line[0] == '#') {
            char *word = line + 1;          // 跳过 '#'
            trim(word);                     // 去除可能的前后空白
            // 转为小写
            for (char *p = word; *p; p++) {
                *p = tolower((unsigned char)*p);
            }
            strncpy(current_word, word, sizeof(current_word) - 1);
            current_word[sizeof(current_word) - 1] = '\0';
            continue;
        }

        // 2) 处理以 "Trans:" 开头的行 —— 提取翻译
        if (strncmp(line, "Trans:", 6) == 0) {
            char *trans = line + 6;         // 跳过 "Trans:"
            trim(trans);                    // 去除前后空白
            if (strlen(trans) == 0) continue;

            // 如果有等待的单词，则插入
            if (strlen(current_word) > 0) {
                if (hash_table_insert(table, current_word, trans)) {
                    (*dict_count)++;
                }
                // 清空单词，准备处理下一对
                current_word[0] = '\0';
            }
            // 如果之前没有单词，则忽略该翻译行（数据不完整）
            continue;
        }

        // 3) 其他行（如注释行、非标准行）忽略
        // 但为了容错，也可以尝试按照原有 '\t' 格式处理（可选）
        // 这里我们直接忽略，因为文件格式固定
    }

    fclose(file);
    return 0;
}
