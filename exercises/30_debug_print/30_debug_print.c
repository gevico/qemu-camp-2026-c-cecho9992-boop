#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>

#if defined(DEBUG_LEVEL)

#if DEBUG_LEVEL == 3
static void print_backtrace() {
    void *callstack[10];
    int frames = backtrace(callstack, 10);
    char **strs = backtrace_symbols(callstack, frames);
    if (strs != NULL) {
        printf("DEBUG: backtrace (frames: %d):\n", frames);
        for (int i = 0; i < frames; ++i) {
            printf("  %s\n", strs[i]);
        }
        free(strs);
    }
}
#endif
#if DEBUG_LEVEL == 1
#define DEBUG_PRINT(fmt, ...) do { \
    printf("DEBUG: func=%s, line=%d\n", __func__, __LINE__); \
} while (0)
#elif DEBUG_LEVEL == 2
#define DEBUG_PRINT(fmt, ...) do { \
    printf("DEBUG: func=%s, line=%d, ", __func__, __LINE__); \
    printf(fmt, ##__VA_ARGS__); \
    printf("\n"); \
} while (0)
#elif DEBUG_LEVEL == 3
#define DEBUG_PRINT(fmt, ...) do { \
    printf("DEBUG: func=%s, line=%d, ", __func__, __LINE__); \
    printf(fmt, ##__VA_ARGS__); \
    printf("\n"); \
    print_backtrace(); \
} while (0)
#else
#define DEBUG_PRINT(fmt, ...) do { \
    printf("DEBUG: func=%s, line=%d (invalid DEBUG_LEVEL)\n", __func__, __LINE__); \
} while (0)
#endif
#else
#define DEBUG_PRINT(fmt, ...) do {} while (0)
#endif
void test() {
    int x = 42;
    DEBUG_PRINT("x=%d", x);
}

int main() {
    test();
    return 0;
}