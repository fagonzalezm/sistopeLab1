#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

int main(int argc, char **argv){
    int cValue = 0;
    int nValue = 0;

    int flag;
    while( (flag = getopt(argc, argv, "c:n:")) != -1){
        switch (flag)        {
        case 'c':
            cValue = atoi(optarg);
            break;
        case 'n':
            nValue = atoi(optarg);
            break;
        default:
            abort();
        }
    }
    
    pixelMatrix pixels;

    for(int i = 0; i<cValue;i++){
        read(STDIN_FILENO, &pixels, sizeof(pixelMatrix));
        /*int solution = 0;
        int size = pixels.m * pixels.n;
        int threshold = (int)((nValue/100.0)*size);
        int count = 0;
        for(int i = 0; i < pixels.m; i++){
            for(int j = 0; j < pixels.n; j++){
                if(pixels.matrix[i][j] == 0){
                    count = count + 1;
                    if(count >= threshold){
                        solution = 1;
                        break;
                    }
                }
            }
        }
        if(solution == 1){
            write(STDOUT_FILENO, "yes", 3);
        }
        else{
            write(STDOUT_FILENO, "no", 3);
        }*/
        write(STDOUT_FILENO, &pixels, sizeof(pixelMatrix));
    }
    wait(NULL);
    return 0;
}