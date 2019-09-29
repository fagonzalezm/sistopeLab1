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
    for(int i = 0; i<pixels.m; i++){
        for(int j = 0; j<pixels.n; j++){
            if((pixels.matrix)[i][j]<0){
                (pixels.matrix)[i][j] = 0;
            }
        }
    }
    write(STDOUT_FILENO, &pixels, sizeof(pixelMatrix));

    wait(NULL);
    //Send Results
    return 0;
}