#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

int main(int argc, char **argv){
    words palabra;
    read(STDIN_FILENO, &palabra, sizeof(words));
    strcat(palabra.word,"6");
    printf("WORD final: %s\n", palabra.word);
    return 0;
}