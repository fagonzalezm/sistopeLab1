#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

//Entradas: En argv se debe ingresar -c <Cantidad de imagenes>.
//Funcionamiento: Realiza la rectificacion. Primero, se leen las entradas del argv usando getopt. Luego, se realiza la rectificacion.
//Salida: --
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
    //Rectificacion
    //Para cada imagen
    for(int i= 0; i<cValue; i++){
        floatPixelMatrix floatPixels;
        read(STDIN_FILENO, &floatPixels, sizeof(floatPixelMatrix));
        //Se eliminan los valores negativos
        for(int i = 0; i<floatPixels.m; i++){
            for(int j = 0; j<floatPixels.n; j++){
                if((floatPixels.matrix)[i][j]<0.0){
                    (floatPixels.matrix)[i][j] = 0;
                }
            }
        }
        write(STDOUT_FILENO, &floatPixels, sizeof(floatPixelMatrix));
    }
    wait(NULL);
    return 0;
}