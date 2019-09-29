#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

int main(int argc, char **argv){
    //words palabra;
    printf("AAAA K RABIA\n");
    pixelMatrix pixels;
    read(STDIN_FILENO, &pixels, sizeof(pixelMatrix));





    for(int i=0;i<pixels.m;i++){
        for(int j=0;j<pixels.n;j++){
            printf("%d ",(pixels.matrix)[i][j]);
        }
        printf("\n");
    }
    return 0;
}