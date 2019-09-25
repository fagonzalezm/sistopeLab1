#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

int main(int argc, char **argv){
    //Code
    words palabra;
    read(STDIN_FILENO, &palabra, sizeof(words));
    strcat(palabra.word,"3");
    write(STDOUT_FILENO, &palabra, sizeof(words));

    wait(NULL);
    //Send Results
    return 0;
}