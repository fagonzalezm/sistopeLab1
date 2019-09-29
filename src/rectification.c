#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

int main(int argc, char **argv){
    int cValue = 0;
    int flag;
    while( (flag = getopt(argc, argv, "c:")) != -1){
        switch (flag)        {
        case 'c':
            cValue = atoi(optarg);
            break;
        
        default:
            abort();
        }
    }

    pixelMatrix pixels;

    for(int i= 0; i<cValue; i++){
        read(STDIN_FILENO, &pixels, sizeof(pixelMatrix));
        for(int i = 0; i<pixels.m; i++){
            for(int j = 0; j<pixels.n; j++){
                if((pixels.matrix)[i][j]<0){
                    (pixels.matrix)[i][j] = 0;
                }
            }
        }
        write(STDOUT_FILENO, &pixels, sizeof(pixelMatrix));
    }
    wait(NULL);
    return 0;
}