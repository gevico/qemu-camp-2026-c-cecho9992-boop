#include <stdio.h>

int main() {
    char str[]="Don't ask what your country can do for you, but ask what you can do for your country.";
    int wordCount = 0;
    // TODO: 在这里添加你的代码
    int i = 0;
    if(str[0]!='\0' && str[0] != ' ')
    wordCount = 1;

    while (str[i]!='0')
    {
        if(str[i] == ' ' && str[i+1] != ' '){
            wordCount++;
        }
        i++;
    }
    
    
    printf("单词数量: %d\n", wordCount);
    
    return 0;
}