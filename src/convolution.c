#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

int main(int argc, char **argv){
    //Code
    //words palabra;
    pixelMatrix pixels;
    read(STDIN_FILENO, &pixels, sizeof(pixelMatrix));
    write(STDOUT_FILENO, &pixels, sizeof(pixelMatrix));
    wait(NULL);
    //Send Results
    return 0;
}