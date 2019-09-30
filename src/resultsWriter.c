#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

int main(int argc, char **argv){
    printf("AAAA K RABIA\n");

    int cValue = 0;
    int bFlag = 0;
    int flag;
    while( (flag = getopt(argc, argv, "c:b:")) != -1){
        switch (flag)        {
        case 'c':
            cValue = atoi(optarg);
            break;
        case 'b':
            bFlag = atoi(optarg);
            break;
        
        default:
            abort();
        }
    }

    pixelMatrix pixels;

    /*char nearlyblack[3];
    if(bFlag == 1){
        printf("|   image   | nearly black |\n");
        printf("|-----------|--------------|\n");
        for(int i= 0; i<cValue; i++){
            read(STDIN_FILENO, &nearlyblack, 3);
            printf("|  imagen_%d |     %s       |\n",i+1,nearlyblack);
        }
    }*/
    
    for(int i= 0; i<cValue; i++){
        printf("read\n");
        read(STDIN_FILENO, &pixels, sizeof(pixelMatrix));
        printf("(m,n): (%d,%d)\n", pixels.m,pixels.n);
        for(int i = 0; i< pixels.m;i++){
            for(int j = 0;j<pixels.n;j++){
                printf("%d ", (pixels.matrix)[i][j]);
            }
            printf("\n");
        }
    }
    printf("CHAO\n");
    
    return 0;
}