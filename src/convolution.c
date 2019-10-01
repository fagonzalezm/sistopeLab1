#include <sys/types.h>
#include <ctype.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

//Entradas:	kernelMatrix kernel:Representa el filtro
//			pixelMatrix pixels: Respresenta una imagen 
//Funcionamiento: Se recorre la matriz de los valores de los pixeles aplicando el concepto de convolucion haciendo uso de una matriz de numeros (kernel) para generar una nueva matriz normalizada.
//Salida: floatPixelMatrix que representa una matriz de flotantes que contiene los valores normalizados.
floatPixelMatrix convolution(kernelMatrix kernel, pixelMatrix pixels){

	floatPixelMatrix floatPixels;
	floatPixels.m = pixels.m;
	floatPixels.n = pixels.n;

	int fila;
	int columna;
	for(fila=0;fila<pixels.m;fila++){
		for(columna=0;columna<pixels.n;columna++){
			float resultado = 0;
			int s1 = 0;
			int s2 = 0;
			int s3 = 0;
			int s4 = 0;
			int s5 = 0;
			int s6 = 0;
			int s7 = 0;
			int s8 = 0;
			int s9 = 0;
			if((fila == 0) && (columna==0)){
				s5 = (pixels.matrix[fila][columna] * (kernel.matrix)[1][1]);
				s6 = (pixels.matrix[fila][columna + 1] * (kernel.matrix)[1][2]);
				s8 = (pixels.matrix[fila + 1][columna] * (kernel.matrix)[2][1]);
				s9 = (pixels.matrix[fila +1][columna +1] * (kernel.matrix)[2][2]);
			}
			else if ((fila == (pixels.m - 1)) && (columna == 0)){
				s2 = (pixels.matrix[fila - 1][columna] * (kernel.matrix)[0][1]);
				s3 = (pixels.matrix[fila - 1][columna + 1] * (kernel.matrix)[0][2]);
				s5 = (pixels.matrix[fila][columna] * (kernel.matrix)[1][1]);
				s6 = (pixels.matrix[fila][columna + 1] * (kernel.matrix)[1][2]);
			}
			else if((fila == 0) && (columna == (pixels.n - 1))){
				s4 = (pixels.matrix[fila][columna - 1] * (kernel.matrix)[1][0]);
				s5 = (pixels.matrix[fila][columna] * (kernel.matrix)[1][1]);
				s7 = (pixels.matrix[fila + 1][columna - 1] * (kernel.matrix)[2][0]);
				s8 = (pixels.matrix[fila + 1][columna] * (kernel.matrix)[2][1]);
			}
			else if ((fila == (pixels.m - 1)) && (columna == (pixels.n - 1))){
				s1 = (pixels.matrix[fila - 1][columna - 1] * (kernel.matrix)[0][0]);
				s2 = (pixels.matrix[fila - 1][columna] * (kernel.matrix)[0][1]);
				s4 = (pixels.matrix[fila][columna - 1] * (kernel.matrix)[1][0]);
				s5 = (pixels.matrix[fila][columna] * (kernel.matrix)[1][1]);
			}
			else if (columna == 0){
				s2 = (pixels.matrix[fila - 1][columna] * (kernel.matrix)[0][1]);
				s3 = (pixels.matrix[fila - 1][columna + 1] * (kernel.matrix)[0][2]);
				s5 = (pixels.matrix[fila][columna] * (kernel.matrix)[1][1]);
				s6 = (pixels.matrix[fila][columna + 1] * (kernel.matrix)[1][2]);
				s8 = (pixels.matrix[fila + 1][columna] * (kernel.matrix)[2][1]);
				s9 = (pixels.matrix[fila + 1][columna + 1] * (kernel.matrix)[2][2]);
			}
			else if (fila == 0){
				s4 = (pixels.matrix[fila][columna - 1] * (kernel.matrix)[1][0]);
				s5 = (pixels.matrix[fila][columna] * (kernel.matrix)[1][1]);
				s6 = (pixels.matrix[fila][columna + 1] * (kernel.matrix)[1][2]);
				s7 = (pixels.matrix[fila + 1][columna - 1] * (kernel.matrix)[2][0]);
				s8 = (pixels.matrix[fila + 1][columna] * (kernel.matrix)[2][1]);
				s9 = (pixels.matrix[fila + 1][columna + 1] * (kernel.matrix)[2][2]);
			}
			else if (columna == (pixels.n - 1)){
				s1 = (pixels.matrix[fila - 1][columna - 1] * (kernel.matrix)[0][0]);
				s2 = (pixels.matrix[fila - 1][columna] * (kernel.matrix)[0][1]);
				s4 = (pixels.matrix[fila][columna - 1] * (kernel.matrix)[1][0]);
				s5 = (pixels.matrix[fila][columna] * (kernel.matrix)[1][1]);
				s7 = (pixels.matrix[fila + 1][columna - 1] * (kernel.matrix)[2][0]);
				s8 = (pixels.matrix[fila + 1][columna] * (kernel.matrix)[2][1]);
			}
			else if (fila == (pixels.m - 1)){
				s1 = (pixels.matrix[fila - 1][columna - 1] * (kernel.matrix)[0][0]);
				s2 = (pixels.matrix[fila - 1][columna] * (kernel.matrix)[0][1]);
				s3 = (pixels.matrix[fila - 1][columna + 1] * (kernel.matrix)[0][2]);
				s4 = (pixels.matrix[fila][columna - 1] * (kernel.matrix)[1][0]);
				s5 = (pixels.matrix[fila][columna] * (kernel.matrix)[1][1]);
				s6 = (pixels.matrix[fila][columna + 1] * (kernel.matrix)[1][2]);
			}
			else {
				s1 = (pixels.matrix[fila - 1][columna - 1] * (kernel.matrix)[0][0]);
				s2 = (pixels.matrix[fila - 1][columna] * (kernel.matrix)[0][1]);
				s3 = (pixels.matrix[fila - 1][columna + 1] * (kernel.matrix)[0][2]);
				s4 = (pixels.matrix[fila][columna - 1] * (kernel.matrix)[1][0]);
				s5 = (pixels.matrix[fila][columna] * (kernel.matrix)[1][1]);
				s6 = (pixels.matrix[fila][columna + 1] * (kernel.matrix)[1][2]);
				s7 = (pixels.matrix[fila + 1][columna - 1] * (kernel.matrix)[2][0]);
				s8 = (pixels.matrix[fila + 1][columna] * (kernel.matrix)[2][1]);
				s9 = (pixels.matrix[fila + 1][columna + 1] * (kernel.matrix)[2][2]);
			}
			resultado = (s1 + s2 +s3 +s4 +s5 +s6 +s7 +s8 +s9);
			(floatPixels.matrix)[fila][columna] = resultado / 9.0;
		}
	}

	return floatPixels;
}

//Entradas: En argv se debe ingresar -c <Cantidad de imagenes> -o <Valor de la posicion (1,1) del filtro> -p <Valor de la posicion (1,2) del filtro> -1 <Valor de la posicion (1,3) del filtro> -r <Valor de la posicion (2,1) del filtro> .s <Valor de la posicion (2,2) del filtro> -t <Valor de la posicion (2,3) del filtro> -u <Valor de la posicion (3,1) del filtro> -v <Valor de la posicion (3,2) del filtro> -w <Valor de la posicion (3,3) del filtro>
//Funcionamiento: Primero, se leen las entradas del argv usando getopt. Luego, se realiza la convolucion.
//Salida: --
int main(int argc, char **argv){
    int cValue = 0;
    int flag;
	kernelMatrix kernel; 
    while( (flag = getopt(argc, argv, "c:o:p:q:r:s:t:u:v:w:")) != -1){
        switch (flag){
        case 'c':
            cValue = atoi(optarg);
            break;
        case 'o':
            (kernel.matrix)[0][0] = atoi(optarg);
            break;
		case 'p':
            (kernel.matrix)[0][1] = atoi(optarg);
            break;
		case 'q':
            (kernel.matrix)[0][2] = atoi(optarg);
            break;
		case 'r':
            (kernel.matrix)[1][0] = atoi(optarg);
            break;
		case 's':
            (kernel.matrix)[1][1] = atoi(optarg);
            break;
		case 't':
            (kernel.matrix)[1][2] = atoi(optarg);
            break;
		case 'u':
            (kernel.matrix)[2][0] = atoi(optarg);
            break;
		case 'v':
            (kernel.matrix)[2][1] = atoi(optarg);
            break;
		case 'w':
            (kernel.matrix)[2][2] = atoi(optarg);
            break;
        default:
            abort();
        }
    }
	//Para cada imagen
    for(int i = 0; i<cValue;i++){
        pixelMatrix pixels;
        read(STDIN_FILENO, &pixels, sizeof(pixelMatrix));
		//Se realiza la convolucion
        floatPixelMatrix floatPixels = convolution(kernel,pixels);

        write(STDOUT_FILENO, &floatPixels, sizeof(floatPixelMatrix));
        
    }
    
    wait(NULL);
    return 0;
}