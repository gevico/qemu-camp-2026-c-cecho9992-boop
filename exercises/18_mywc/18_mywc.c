#include "mywc.h"
#include <stdio.h>
#include <signal.h>

void simple_wc(const char *path)
{
    FILE *fp = fopen(path, "r");
    if (!fp)
    {
        perror("open file failed");
        return;
    }
    long lines = 0;
    long words = 0;
    long chars = 0;
    int in_word = 0;
    int ch;
    while ((ch = fgetc(fp)) != EOF)
    {
        chars++;
        if (ch == '\n')
            lines++;
        if (isspace(ch))
        {
            in_word = 0;
        }
        else if (!in_word)
        {
            in_word = 1;
            words++;
        }
    }
    fclose(fp);
    printf("%ld lines, %ld words, %ld characters\n", lines, words, chars);
}

int main(int argc, char *argv[]) {
    signal(SIGPIPE, SIG_IGN);
    const char* filepath = "./text.txt";
    simple_wc(filepath);
    int ret = process_file(filepath);
    if (ret != 0) {
        fprintf(stderr, "单词统计失败\n");
        return 1;
    }
    return 0;
}
