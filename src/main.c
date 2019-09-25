#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "main.h"

#define READ 0
#define WRITE 1

void pipeline(){
    pid_t pid1, pid2, pid3, pid4, pid5;
    int  status1, status2, status3, status4, status5;

    char * argvConvolution[] ={"convolution",NULL};
    char * argvRectification[] ={"rectification",NULL};
    char * argvPooling[] ={"pooling",NULL};
    char * argvClassifier[] ={"classifier",NULL};
    char * argvResultsWriter[] ={"resultsWriter",NULL};

    int * pipe1 = (int *)malloc(2*sizeof(int));
    int * pipe2 = (int *)malloc(2*sizeof(int));
    int * pipe3 = (int *)malloc(2*sizeof(int));
    int * pipe4 = (int *)malloc(2*sizeof(int));
    int * pipe5 = (int *)malloc(2*sizeof(int));

    words palabra;

    pipe(pipe1);
    pipe(pipe2);
    pipe(pipe3);
    pipe(pipe4);
    pipe(pipe5);

    if( (pid1=fork()) == 0 ){
        if( (pid2=fork()) == 0 ){
            if( (pid3=fork()) == 0 ){
                if( (pid4=fork()) == 0 ){
                    if( (pid5=fork()) == 0 ){
                        dup2(pipe5[READ],STDIN_FILENO);

                        close(pipe1[READ]);
                        close(pipe1[WRITE]);
                        close(pipe2[READ]);
                        close(pipe2[WRITE]);
                        close(pipe3[READ]);
                        close(pipe3[WRITE]);
                        close(pipe4[READ]);
                        close(pipe4[WRITE]);
                        close(pipe5[WRITE]);

                        //Results Writer
                        execvp("bin/resultsWriter",argvResultsWriter);
                    }
                    else{
                        dup2(pipe4[READ],STDIN_FILENO);
                        dup2(pipe5[WRITE],STDOUT_FILENO);

                        close(pipe1[READ]);
                        close(pipe1[WRITE]);
                        close(pipe2[READ]);
                        close(pipe2[WRITE]);
                        close(pipe3[READ]);
                        close(pipe3[WRITE]);
                        close(pipe4[WRITE]);
                        close(pipe5[READ]);

                        //Classifier
                        execvp("bin/classifier",argvClassifier);
                    }
                }
                else{
                    dup2(pipe3[READ],STDIN_FILENO);
                    dup2(pipe4[WRITE],STDOUT_FILENO);

                    close(pipe1[READ]);
                    close(pipe1[WRITE]);
                    close(pipe2[READ]);
                    close(pipe2[WRITE]);
                    close(pipe3[WRITE]);
                    close(pipe4[READ]);
                    close(pipe5[READ]);
                    close(pipe5[WRITE]);
                    
                    //Pooling
                    execvp("bin/pooling",argvConvolution);
                }
            }
            else{
                dup2(pipe2[READ],STDIN_FILENO);
                dup2(pipe3[WRITE],STDOUT_FILENO);

                close(pipe1[READ]);
                close(pipe1[WRITE]);
                close(pipe2[WRITE]);
                close(pipe3[READ]);
                close(pipe4[READ]);
                close(pipe4[WRITE]);
                close(pipe5[READ]);
                close(pipe5[WRITE]);

                //Rectification
                execvp("bin/rectification",argvRectification);
            }
        }
        else{
            dup2(pipe1[READ],STDIN_FILENO);
            dup2(pipe2[WRITE],STDOUT_FILENO);

            close(pipe1[WRITE]);
            close(pipe2[READ]);
            close(pipe3[READ]);
            close(pipe3[WRITE]);
            close(pipe4[READ]);
            close(pipe4[WRITE]);
            close(pipe5[READ]);
            close(pipe5[WRITE]);
            //Convolution
            execvp("bin/convolution",argvConvolution);
        }
    }
    else{
        palabra.word[0]='1';
        palabra.word[1]='\0';     
        printf("WORD inicial: %s\n", palabra.word);
        dup2(pipe1[WRITE],STDOUT_FILENO);

        close(pipe1[READ]);
        close(pipe2[READ]);
        close(pipe2[WRITE]);
        close(pipe3[READ]);
        close(pipe3[WRITE]);
        close(pipe4[READ]);
        close(pipe4[WRITE]);
        close(pipe5[READ]);
        close(pipe5[WRITE]);

        //Main

        
        write(STDOUT_FILENO, &palabra, sizeof(words));
        wait(&status1);
    }
    free(pipe1);
    pipe1 = NULL;
    free(pipe2);
    pipe2 = NULL;
    free(pipe3);
    pipe3 = NULL;
    free(pipe4);
    pipe4 = NULL;
    free(pipe5);
    pipe5 = NULL;
    //free(palabra);
    //palabra = NULL;

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