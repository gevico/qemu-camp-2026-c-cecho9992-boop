#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "common.h"

#define MAX_INPUT 1024
#define MAX_ARGS 64

// ======================
// 内置命令
// ======================

void execute_cd(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "mybash: expected argument to cd\n");
    } else {
        if (chdir(args[1]) != 0) {
            perror("mybash");
        }
    }
}

void execute_exit() {
    exit(0);
}

int is_builtin_command(char **args) {
    if (args[0] == NULL) return 0;

    if (strcmp(args[0], "cd") == 0) {
        execute_cd(args);
        return 1;
    }

    if (strcmp(args[0], "exit") == 0) {
        execute_exit();
        return 1;
    }

    return 0;
}

// ======================
// 解析输入
// ======================

int parse_input(char *input, char **args) {
    int i = 0;
    int in_quotes = 0;
    char buf[MAX_INPUT];
    int idx = 0;

    memset(buf, 0, sizeof(buf));

    for (char *p = input; *p && i < MAX_ARGS - 1; p++) {

        char c = *p;

        if (c == '"') {
            in_quotes = !in_quotes;
            continue;
        }

        if (!in_quotes && (c == ' ' || c == '\t')) {
            if (idx > 0) {
                buf[idx] = '\0';
                args[i++] = strdup(buf);
                idx = 0;
                memset(buf, 0, sizeof(buf));
            }
            continue;
        }

        buf[idx++] = c;
    }

    if (idx > 0) {
        buf[idx] = '\0';
        args[i++] = strdup(buf);
    }

    args[i] = NULL;
    return i;
}

// ======================
// 命令表
// ======================

typedef int (*cmd0)(void);
typedef int (*cmd1)(const char*);
typedef int (*cmd2)(const char*, const char*);

typedef struct {
    const char *name;
    int argc_type; // 0 / 1 / 2
    union {
        cmd0 f0;
        cmd1 f1;
        cmd2 f2;
    } func;
} Command;

// 外部命令
Command commands[] = {
    {"myfile", 1, .func.f1 = __cmd_myfile},
    {"mysed",  2, .func.f2 = __cmd_mysed},
    {"mytrans", 1, .func.f1 = __cmd_mytrans},
    {"mywc", 1, .func.f1 = __cmd_mywc},
    {NULL, 0, .func.f0 = NULL}
};

// ======================
// 执行命令
// ======================

void execute_command(char **args, int argc) {
    const char *name = args[0];

    const char *arg1 = (argc >= 2) ? args[1] : NULL;
    const char *arg2 = (argc >= 3) ? args[2] : NULL;

    for (Command *c = commands; c->name; c++) {
        if (strcmp(c->name, name) == 0) {

            switch (c->argc_type) {
                case 0:
                    c->func.f0();
                    return;
                case 1:
                    c->func.f1(arg1);
                    return;
                case 2:
                    c->func.f2(arg1, arg2);
                    return;
            }
        }
    }

    fprintf(stderr, "mybash: command not found: %s\n", name);
}

// ======================
// main
// ======================

int main(int argc, char *argv[]) {
    char input[MAX_INPUT];
    char *args[MAX_ARGS];

    // ================= file mode =================
    if (argc > 1) {
        FILE *fp = fopen(argv[1], "r");
        if (!fp) {
            fprintf(stderr, "mybash: cannot open file %s\n", argv[1]);
            return 1;
        }

        while (fgets(input, sizeof(input), fp)) {
            input[strcspn(input, "\n")] = '\0';

            int c = parse_input(input, args);
            if (c == 0) continue;

            if (is_builtin_command(args)) continue;

            execute_command(args, c);
        }

        fclose(fp);
        return 0;
    }

    // ================= interactive mode =================
    while (1) {
        printf("mybash$ ");
        fflush(stdout);

        if (!fgets(input, sizeof(input), stdin)) {
            printf("\n");
            break;
        }

        input[strcspn(input, "\n")] = '\0';

        int c = parse_input(input, args);
        if (c == 0) continue;

        if (is_builtin_command(args)) continue;

        execute_command(args, c);
    }

    return 0;
}