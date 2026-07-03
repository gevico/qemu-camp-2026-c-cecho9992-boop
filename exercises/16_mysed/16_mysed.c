#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 1024

int parse_replace_command(const char* cmd, char** old_str, char** new_str) {
    if (cmd[0] != 's' || cmd[1] != '/') {
        return -1;
    }

    const char* p = cmd + 2;
    // 找第一个分隔符 /
    const char* first_slash = strchr(p, '/');
    if (!first_slash) return -1;
    size_t old_len = first_slash - p;

    // 找第二个分隔符 /
    const char* second_slash = strchr(first_slash + 1, '/');
    if (!second_slash) return -1;
    size_t new_len = second_slash - (first_slash + 1);

    // 分配内存存放旧字符串
    *old_str = malloc(old_len + 1);
    strncpy(*old_str, p, old_len);
    (*old_str)[old_len] = '\0';

    // 分配内存存放新字符串
    *new_str = malloc(new_len + 1);
    strncpy(*new_str, first_slash + 1, new_len);
    (*new_str)[new_len] = '\0';

    return 0;
}

// 替换 str 中第一个 old 子串为 new
void replace_first_occurrence(char* str, const char* old, const char* new) {
    if (str == NULL || old == NULL || new == NULL)
        return;

    char* pos = strstr(str,old);
    if (pos == NULL)
        return; // 无匹配直接返回

    size_t old_len = strlen(old);
    size_t new_len = strlen(new);

    // 新旧长度相等：直接覆盖
    if (old_len == new_len) {
        memcpy(pos, new, new_len);
        return;
    }

    // 新串更短：前移后面字符填补空隙
    if (new_len < old_len) {
        // 剩余字符串起始位置
        char* src = pos + old_len;
        char* dst = pos + new_len;
        while (*src != '\0') {
            *dst = *src;
            src++;
            dst++;
        }
        *dst = '\0';
        memcpy(pos, new, new_len);
        return;
    }

    // 新串更长：向后移动尾部字符腾出空间
    size_t tail_len = strlen(pos + old_len);
    char tail_buf[MAX_LINE_LENGTH];
    strcpy(tail_buf, pos + old_len);

    // 写入新字符串
    memcpy(pos, new, new_len);
    // 拼接尾部剩余文本
    strcpy(pos + new_len, tail_buf);
}

int main(int argc, char* argv[]) {
    const char* replcae_rules = "s/unix/linux/";

    char line[MAX_LINE_LENGTH] = {"unix is opensource. unix is free os."};

    char* old_str = NULL;
    char* new_str = NULL;
    
    if (parse_replace_command(replcae_rules, &old_str, &new_str) != 0) {
        fprintf(stderr, "Invalid replace command format. Use 's/old/new/'\n");
        return 1;
    }

    replace_first_occurrence(line, old_str, new_str);
    fputs(line, stdout);

    free(old_str);
    free(new_str);
    return 0;
}
