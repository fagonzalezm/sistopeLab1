#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

//Entradas: En argv se debe ingresar -c <Cantidad de imagenes>.
//Funcionamiento: Realiza el Pooling. Primero, se leen las entradas del argv usando getopt. Luego, se realiza el pooling.
//Salida: --
int main(int argc, char **argv){
    int cValue = 0;
    int flag;
    while( (flag = getopt(argc, argv, "c:")) != -1){
        switch (flag)
        {
        case 'c':
            cValue = atoi(optarg);
            break;
        
        default:
            abort();
        }
    }

    //Pooling
    //Para cada imagen
    for(int i= 0; i<cValue; i++){
        floatPixelMatrix floatPixels;
        read(STDIN_FILENO, &floatPixels, sizeof(floatPixelMatrix));
        //Se adecua la matrix de pixeles agregando ceros abajo y a la derecha para que el tamaño de la matriz sea divisible por 3 
        if(floatPixels.m%3==1){
            for(int i = 0; i<floatPixels.n; i++){
                (floatPixels.matrix)[floatPixels.m+1][i]=0.0;
                (floatPixels.matrix)[floatPixels.m+2][i]=0.0;
            }
            floatPixels.m = floatPixels.m + 2;
        }
        else if(floatPixels.m%3==2){
            for(int i = 0; i<floatPixels.n; i++){
                (floatPixels.matrix)[floatPixels.m+1][i]=0.0;
            }
            floatPixels.m = floatPixels.m + 1;
        }
        else if(floatPixels.n%3==1){
            for(int i = 0; i<floatPixels.m; i++){
                (floatPixels.matrix)[i][floatPixels.n+1]=0.0;
                (floatPixels.matrix)[i][floatPixels.n+2]=0.0;
            }
            floatPixels.n = floatPixels.n + 2;
        }
        else if(floatPixels.n%3==2){
            for(int i = 0; i<floatPixels.m; i++){
                (floatPixels.matrix)[i][floatPixels.n+1]=0.0;
            }
            floatPixels.n = floatPixels.n + 1;
        }
        //Se recorre la matriz de pixeles de 9 en 9, definiendo el valor mayor dentro de ese conjunto
        for(int i = 0; i<(floatPixels.m/3); i++){
            for(int j = 0; j<(floatPixels.n/3); j++){
                float higher = (floatPixels.matrix)[3*i][3*j];
                if((floatPixels.matrix)[3*i+1][3*j] > higher){
                    higher = (floatPixels.matrix)[3*i+1][3*j];
                }
                if((floatPixels.matrix)[3*i+2][3*j] > higher){
                    higher = (floatPixels.matrix)[3*i+2][3*j];
                }
                if((floatPixels.matrix)[3*i][3*j+1] > higher){
                    higher = (floatPixels.matrix)[3*i][3*j+1];
                }

                if((floatPixels.matrix)[3*i+1][3*j+1] > higher){
                    higher = (floatPixels.matrix)[3*i+1][3*j+1];
                }

                if((floatPixels.matrix)[3*i+2][3*j+1] > higher){
                    higher = (floatPixels.matrix)[3*i+2][3*j+1];
                }

                if((floatPixels.matrix)[3*i][3*j+2] > higher){
                    higher = (floatPixels.matrix)[3*i][3*j+2];
                }
                if((floatPixels.matrix)[3*i+1][3*j+2] > higher){
                    higher = (floatPixels.matrix)[3*i+1][3*j+2];
                }
                if((floatPixels.matrix)[3*i+2][3*j+2] > higher){
                    higher = (floatPixels.matrix)[3*i+2][3*j+2];
                }
                //Se reemplaza el valor mayor en su lugar correspondiente
                (floatPixels.matrix)[i][j]=higher;
            }
        }
        //Se reduce el tamaño de la matriz
        floatPixels.m = (floatPixels.m)/3;
        floatPixels.n = (floatPixels.n)/3;
        write(STDOUT_FILENO, &floatPixels, sizeof(floatPixelMatrix));
    }
    wait(NULL);
    return 0;
}