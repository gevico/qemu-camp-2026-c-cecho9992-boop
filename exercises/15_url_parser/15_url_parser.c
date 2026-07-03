#include <stdio.h>
#include <string.h>

/**
 * URL解析器（严格格式版）
 */

int parse_url(const char* url) {
    char *q = strstr(url, "?");

    if (!q) {
        printf("No query parameters\n");
        return 0;
    }

    char buf[512];
    strcpy(buf, q + 1);

    char *param = strtok(buf, "&");

    while (param) {
        char *eq = strchr(param, '=');

        if (!eq) {
            printf("key = %s, value = (empty)\n", param);
        } else {
            *eq = '\0';
            char *key = param;
            char *val = eq + 1;

            printf("key = %s, value = %s\n", key, val);
        }

        param = strtok(NULL, "&");
    }

    return 0;
}

int main() {
    const char* test_url =
        "https://cn.bing.com/search?name=John&age=30&city=New+York";

    printf("Parsing URL: %s\n", test_url);
    printf("Parameters:\n");

    parse_url(test_url);

    return 0;
}