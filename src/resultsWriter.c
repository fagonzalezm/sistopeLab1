#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv){
    char word[6];
    read(STDIN_FILENO, word, 6);
    strcat(word,"6");
    printf("WORD final: %s\n", word);
    return 0;
}