#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

//Entradas: En argv se debe ingresar -c <Cantidad de imagenes> -n <Valor del umbral de negrura>.
//Funcionamiento: Realiza la clasificacion. Primero, se leen las entradas del argv usando getopt. Luego, se realiza la clasificacion.
//Salida: --
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
    
    //Clasificacion
    //Por cada imagen
    for(int i = 0; i<cValue;i++){
        //pixelMatrix pixels;
        floatPixelMatrix floatPixels;
        read(STDIN_FILENO, &floatPixels, sizeof(floatPixelMatrix));
        int solution = 0;
        //Se define el valor especifico del umbral
        int size = floatPixels.m * floatPixels.n;
        int threshold = (int)((nValue/100.0)*size);
        //Se obtiene la cantidad de pixeles negros en la matriz de pixeles
        int count = 0;
        for(int i = 0; i < floatPixels.m; i++){
            for(int j = 0; j < floatPixels.n; j++){
                if(floatPixels.matrix[i][j] == 0.0){
                    count = count + 1;
                    //Si la cantidad de pixeles negros es igual al umbral se retorna 1 
                    if(count >= threshold){
                        solution = 1;
                        break;
                    }
                }
            }
        }
        //Se define el valor nearlyBlack de la imagen
        if(solution == 1){
            floatPixels.nearlyBlack = 1;
        }
        else{
            floatPixels.nearlyBlack = 0;
        }
        write(STDOUT_FILENO, &floatPixels, sizeof(floatPixelMatrix));
    }
    wait(NULL);
    return 0;
}