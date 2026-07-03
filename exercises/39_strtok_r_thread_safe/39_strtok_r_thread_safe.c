#include <stdio.h>
#include <stdlib.h>

/*
 * 19 字符串分割器（类似 strtok_r）
 * 目标：实现线程安全的字符串分割函数，不依赖静态全局状态
 * 原型：char* strtok_r(char *str, const char *delim, char **saveptr)
 * 示例：输入 "hello,world test"，分隔符 ", " → 依次得到 hello、world、test
 */

/* 判断字符 c 是否在分隔符集合 delim 中 */
static int is_delim(char c, const char *delim) {
    // TODO: 在这里添加你的代码
    if (delim == NULL) {
        return 0; // 分隔符为空则不匹配
    }
    while (*delim != '\0') {
        if (c == *delim) {
            return 1; // 匹配到分隔符
        }
        delim++;
    }
    return 0; // 未匹配到
}

/* 线程安全版本：通过 saveptr 维护调用状态，不使用静态变量 */
char *strtok_r(char *str, const char *delim, char **saveptr) {
    // TODO: 在这里添加你的代码
    char *start; // 标记分割字段的起始位置
    char *curr;  // 遍历字符串的当前指针

    // 1. 处理首次调用（str非空）和后续调用（str为空，用saveptr延续）
    if (str == NULL) {
        str = *saveptr;
        if (str == NULL) {
            return NULL; // 无后续字符可分割
        }
    }

    // 2. 跳过开头的分隔符
    start = str;
    while (*start != '\0' && is_delim(*start, delim)) {
        start++;
    }
    if (*start == '\0') { // 全是分隔符，无有效字段
        *saveptr = NULL;
        return NULL;
    }

    // 3. 找到当前字段的结束位置（下一个分隔符/字符串末尾）
    curr = start;
    while (*curr != '\0' && !is_delim(*curr, delim)) {
        curr++;
    }

    // 4. 标记下一次调用的起始位置，并截断当前字段
    if (*curr != '\0') {
        *curr = '\0';       // 将分隔符替换为字符串结束符，截断当前字段
        *saveptr = curr + 1;// 保存下一次调用的起始位置
    } else {
        *saveptr = NULL;    // 已到字符串末尾，无后续
    }

    return start; // 返回当前分割出的字段起始地址
}

int main(void) {
    char buf[] = "hello,world test";
    const char *delim = ", ";
    char *save = NULL;

    char *tok = strtok_r(buf, delim, &save);
    while (tok) {
        printf("%s\n", tok);
        tok = strtok_r(NULL, delim, &save);
    }
    return 0;
}
