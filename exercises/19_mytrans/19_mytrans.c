#include "myhash.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

void to_lowercase(char *str) {
    for (; *str; ++str) {
        *str = tolower((unsigned char)*str);
    }
}

int main() {
    HashTable *table = create_hash_table();
    if (!table) {
        fprintf(stderr, "无法创建哈希表\n");
        return 1;
    }

    uint64_t dict_count = 0;
    if (load_dictionary("dict.txt", table, &dict_count) != 0) {
        fprintf(stderr, "加载词典失败\n");
        free_hash_table(table);
        return 1;
    }

    FILE *file = fopen("text.txt", "r");
    if (!file) {
        fprintf(stderr, "无法打开 text.txt\n");
        free_hash_table(table);
        return 1;
    }

    char line[256];

    while (fgets(line, sizeof(line), file)) {

        line[strcspn(line, "\n")] = '\0';

        if (strlen(line) == 0) {
            printf("\n");
            continue;
        }

        // ⚠️ 必须用 strtok 处理
        char *word = strtok(line, " \t");

        while (word) {

            char clean[100] = {0};
            int idx = 0;

            for (char *p = word; *p && idx < 99; p++) {
                if (isalpha((unsigned char)*p) || *p == '\'') {
                    clean[idx++] = *p;
                }
            }

            to_lowercase(clean);

            const char *trans = hash_table_lookup(table, clean);

            if (trans) {
                // ✅ 注意：格式必须完全匹配测试字符串
                printf("原文: %s\t翻译: %s\n", clean, trans);
            } else {
                printf("原文: %s\t未找到该单词的翻译。\n", clean);
            }

            word = strtok(NULL, " \t");
        }
    }

    fclose(file);
    free_hash_table(table);
    return 0;
}