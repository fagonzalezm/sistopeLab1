#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

int main(int argc, char **argv){
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

    
    if(bFlag == 1){
        printf("|   image   | nearly black |\n");
        printf("|-----------|--------------|\n");
        for(int i= 0; i<cValue; i++){
            floatPixelMatrix floatPixels;
            read(STDIN_FILENO, &floatPixels, sizeof(floatPixelMatrix));
            if(floatPixels.nearlyBlack==1){
                printf("|  imagen_%d |     yes      |\n",i+1);
            }
            else if(floatPixels.nearlyBlack==0){
                printf("|  imagen_%d |      no      |\n",i+1);
            }
        }
    }
    
    /*
    for(int i= 0; i<cValue; i++){
        //pixelMatrix pixels;
        floatPixelMatrix floatPixels;
        printf("read\n");
        read(STDIN_FILENO, &floatPixels, sizeof(floatPixelMatrix));



        printf("(m,n): (%d,%d)\n", floatPixels.m,floatPixels.n);
        for(int i = 0; i< floatPixels.m;i++){
            for(int j = 0;j<floatPixels.n;j++){
                printf("%f ", (floatPixels.matrix)[i][j]);
            }
            printf("\n");
        }
    }
    */
    
    return 0;
}