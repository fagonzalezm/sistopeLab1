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
    
    if(pixels.m%3==1){
        for(int i = 0; i<pixels.n; i++){
            (pixels.matrix)[pixels.m+1][i]=0;
            (pixels.matrix)[pixels.m+2][i]=0;
        }
        pixels.m = pixels.m + 2;
    }
    else if(pixels.m%3==2){
        for(int i = 0; i<pixels.n; i++){
            (pixels.matrix)[pixels.m+1][i]=0;
        }
        pixels.m = pixels.m + 1;
    }
    else if(pixels.n%3==1){
        for(int i = 0; i<pixels.m; i++){
            (pixels.matrix)[i][pixels.n+1]=0;
            (pixels.matrix)[i][pixels.n+2]=0;
        }
        pixels.n = pixels.n + 2;
    }
    else if(pixels.n%3==2){
        for(int i = 0; i<pixels.m; i++){
            (pixels.matrix)[i][pixels.n+1]=0;
        }
        pixels.n = pixels.n + 1;
    }
    for(int i = 0; i<(pixels.m/3); i++){
        for(int j = 0; j<(pixels.n/3); j++){
            int higher = (pixels.matrix)[3*i][3*j];
            if((pixels.matrix)[3*i+1][3*j] > higher){
                higher = (pixels.matrix)[3*i+1][3*j];
            }
            if((pixels.matrix)[3*i+2][3*j] > higher){
                higher = (pixels.matrix)[3*i+2][3*j];
            }
            if((pixels.matrix)[3*i][3*j+1] > higher){
                higher = (pixels.matrix)[3*i][3*j+1];
            }

            if((pixels.matrix)[3*i+1][3*j+1] > higher){
                higher = (pixels.matrix)[3*i+1][3*j+1];
            }

            if((pixels.matrix)[3*i+2][3*j+1] > higher){
                higher = (pixels.matrix)[3*i+2][3*j+1];
            }

            if((pixels.matrix)[3*i][3*j+2] > higher){
                higher = (pixels.matrix)[3*i][3*j+2];
            }
            if((pixels.matrix)[3*i+1][3*j+2] > higher){
                higher = (pixels.matrix)[3*i+1][3*j+2];
            }
            if((pixels.matrix)[3*i+2][3*j+2] > higher){
                higher = (pixels.matrix)[3*i+2][3*j+2];
            }
            //printf("%d %d %d\n",(pixels.matrix)[3*i][3*j], (pixels.matrix)[3*i+1][3*j],(pixels.matrix)[3*i+2][3*j]);
            //printf("%d %d %d\n",(pixels.matrix)[3*i][3*j+1], (pixels.matrix)[3*i+1][3*j+1], (pixels.matrix)[3*i+2][3*j+1]);
            //printf("%d %d %d\n",(pixels.matrix)[3*i][3*j+2], (pixels.matrix)[3*i+1][3*j+2],(pixels.matrix)[3*i+2][3*j+2]);

            //printf("higher: %d\n", higher);
            (pixels.matrix)[i][j]=higher;
        }
    }
    pixels.m = (pixels.m)/3;
    pixels.n = (pixels.n)/3;
    
    write(STDOUT_FILENO, &pixels, sizeof(pixelMatrix));

    wait(NULL);
    //Send Results
    return 0;
}