#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "main.h"

void pipeline(){
    pid_t pid1, pid2, pid3, pid4, pid5;
    int  status1, status2, status3, status4, status5;

    char*argvConvolution[] ={"convolution",NULL};
    char*argvRectification[] ={"rectification",NULL};
    char*argvPooling[] ={"pooling",NULL};
    char*argvClassifier[] ={"classifier",NULL};
    char*argvResultsWriter[] ={"resultsWriter",NULL};

    if( (pid1=fork()) == 0 ){
        if( (pid2=fork()) == 0 ){
            if( (pid3=fork()) == 0 ){
                if( (pid4=fork()) == 0 ){
                    if( (pid5=fork()) == 0 ){
                        //Results Writer
                        execvp("bin/resultsWriter",argvResultsWriter);
                    }
                    else{
                        //Classifier
                        execvp("bin/classifier",argvClassifier);
                    }
                }
                else{
                    //Pooling
                    execvp("bin/pooling",argvConvolution);
                }
            }
            else{
                //Rectification
                execvp("bin/rectification",argvRectification);
            }
        }
        else{
            //Convolution
            execvp("bin/convolution",argvConvolution);
        }
    }
    else{
        //Main
        wait(&status1);
        printf("FIN\n");
    }
}

int main(int argc, char **argv){
    int cValue=-1;
    char *mValue = NULL;
    int nValue=-1;
    int bFlag = 0;

    opterr = 0;
    int flag;
    while ((flag = getopt(argc, argv, "c:m:n:b")) != -1){
        switch (flag){
            case 'c':
                cValue = atoi(optarg);
                if(cValue < 1){
                    fprintf(stderr, "La bandera -c debe tener de argumento un número entero positivo.\n");
                    abort();
                }
                break;
            case 'm':
                mValue = optarg;
                int mValueLength = strlen(mValue);
                if(mValue[mValueLength-1] != 't' || mValue[mValueLength-2] != 'x' || mValue[mValueLength-3] != 't' || mValue[mValueLength-4] != '.'){
                    fprintf(stderr, "La bandera -m debe tener de argumento un archivo .txt.\n");
                    abort();
                }
                break;
            case 'n':
                nValue = atoi(optarg);
                if(nValue < 0 || nValue > 100){
                    fprintf(stderr, "La bandera -n debe tener de argumento un número entero entre 0 y 100.\n");
                    abort();
                }
                break;
            case 'b':
                bFlag = 1;
                break;
            case '?':
                if(optopt == 'c'){
                    fprintf(stderr, "La bandera -%c requiere un argumento.\n", optopt);
                }
                else if (isprint(optopt)){
                    fprintf(stderr, "Bandera '-%c' desconocida.\n", optopt);
                }
                else{
                    fprintf (stderr, "Unknown option character '\\x%x'.\n", optopt);
                }
                return 1;
            default:
                abort();
        }
    }
    printf("cValue = %d, mValue = %s, nValue = %d, bFlag = %d\n", cValue, mValue, nValue, bFlag);
    int index;
    for (index = optind; index < argc; index++){
        printf("No hay bandera para el argumento %s\n", argv[index]);
    }
    FILE * file;
    if(cValue == -1){
        fprintf (stderr, "Se debe ingresar la cantidad de imágenes usando la bandera -c\n");
        abort();
    }
    else if(mValue == NULL){
        fprintf (stderr, "Se debe ingresar el nombre del archivo de la máscara del filtro usando la bandera -m\n");
        abort();
    }
    else if(nValue == -1){
        fprintf (stderr, "Se debe ingresar el umbral para la clasificación\n");
        abort();
    }
    else if( (file = fopen (mValue, "r")) == NULL){
        fprintf (stderr, "El archivo %s no existe\n", mValue);
        fclose(file);
        abort();
    }
    else{
        pipeline();
    }
    return 0;
}