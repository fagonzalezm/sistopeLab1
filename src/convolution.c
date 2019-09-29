#include <sys/types.h>
#include <ctype.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

int main(int argc, char **argv){
    int cValue = 0;
    char *mValue = NULL;
    int flag;
    while( (flag = getopt(argc, argv, "c:")) != -1){
        switch (flag){
        case 'c':
            cValue = atoi(optarg);
            break;
        //case 'm':
        //    mValue = optarg;
        //    break;
        default:
            abort();
        }
    }

    pixelMatrix pixels;

    for(int i = 0; i<cValue;i++){
        read(STDIN_FILENO, &pixels, sizeof(pixelMatrix));


        write(STDOUT_FILENO, &pixels, sizeof(pixelMatrix));
        
    }
    
    wait(NULL);
    return 0;
}