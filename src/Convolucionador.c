#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <png.h>
#include <errno.h>
#include <stdarg.h>
//#include "main.h"
typedef struct pixelMatrix{
	int n;
	int m;
	int matrix[600][600];
}pixelMatrix;

int main(){
	//Esto era pa solicitar el nombre del archivo
    char folder[32];
	scanf("%s",folder);

	//definicion de variables
    png_structp	png_ptr;
    png_infop info_ptr;
    FILE * fp;
    png_uint_32 ancho;
    png_uint_32 alto;
    int profundidadBit;
    int tipoColor;
    int metEntrelaz;
    int metCompres;
    int metFiltro;
    int x;
    png_bytepp rows;
    pixelMatrix matrizPix;
    //apertura del archivo (imagen) para lectura en binario
    fp = fopen (folder, "rb");
    if (! fp) {
		printf("Error, el archivo no pudo ser abierto.\n");
    }
    //Se crea la estructura de lectura de PNG
    png_ptr = png_create_read_struct (PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (! png_ptr) {
		printf("No se pudo crear la estructura PNG de lectura.\n");
    }
    //Se crea la estructura de informacion de PNG
    info_ptr = png_create_info_struct (png_ptr);
    if (! png_ptr) {
		printf("No se pudo crear la estructura PNG de informacion.\n");
    }
    //Se inicializan las funciones de entrada y salida predeterminadas para PNG 
    png_init_io (png_ptr, fp);
    //Se lee toda la imagen en memoria
    png_read_png (png_ptr, info_ptr, 0, 0);
    //Se obtinene la informacion desde el fragmento IHDR de la imagen PNG
    png_get_IHDR (png_ptr, info_ptr, & ancho, & alto, & profundidadBit,
		  & tipoColor, & metEntrelaz, & metCompres,
		  & metFiltro);
    //Se obtiene una matriz e punteros que apuntan a los datos de pixeles para cada fila.
    rows = png_get_rows (png_ptr, info_ptr);
    printf ("ancho is %d, alto is %d\n", ancho, alto);
    int rowbytes;
    //Se obtiene la cantidad de bytes necesarios para contener una fila de una imagen.
    rowbytes = png_get_rowbytes (png_ptr, info_ptr);
    printf ("Row bytes = %d\n", rowbytes);

    matrizPix.m = alto;
    matrizPix.n = ancho;
    //Se recorren los punteros de la matriz pixel a pixel y se guardan en una estructura.
    for (x = 0; x < alto; x++) {
		int y;
		png_bytep row;
		row = rows[x];
		for (y = 0; y < rowbytes; y++) {
	    	png_byte pixel;
	    	pixel = row[y];
	    	printf("%d", pixel);
	    	matrizPix.matrix[x][y] = (int) pixel;
            //Puedes jugar con estos printeos para ver como se abre la imagen.
	    	if (pixel == 0) {
			//printf ("#");
	    	}
	    	else if (pixel == 255) {
			//printf ("*");
	    	}
	    	/*else if (pixel < 196) {
			printf (".");
	    	}*/
	    	else {
			//printf (" ");
	    	}
	    	printf(" ");

		}
		printf ("\n");
    }


	int kernel[3][3];
	int in;
	int jn;
	for(in=0;in<3;in++){
		for(jn=0;jn<3;jn++){
			kernel[in][jn]=3;
		}
	}

	//int xn;
	//int yn;
	/*int** matrizAux = (int**) malloc(sizeof(int*)* matrizPix.m );
	for(xn=0;xn<matrizPix.m;xn++){
		matrizAux[xn] = (int*) malloc(sizeof(int) * matrizPix.n);
	}*/
	float matrizAux[600][600]; 

	int fila;
	int columna;
	for(fila=0;fila<matrizPix.m;fila++){
		//printf("Fila: %d ", fila);
		for(columna=0;columna<matrizPix.n;columna++){
			//printf("Columna: %d\n", columna);
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
				s5 = (matrizPix.matrix[fila][columna] * kernel[1][1]);
				s6 = (matrizPix.matrix[fila][columna + 1] * kernel[1][2]);
				s8 = (matrizPix.matrix[fila + 1][columna] * kernel[2][1]);
				s9 = (matrizPix.matrix[fila +1][columna +1] * kernel[2][2]);
			}
			else if ((fila == (matrizPix.m - 1)) && (columna == 0)){
				s2 = (matrizPix.matrix[fila - 1][columna] * kernel[0][1]);
				s3 = (matrizPix.matrix[fila - 1][columna + 1] * kernel[0][2]);
				s5 = (matrizPix.matrix[fila][columna] * kernel[1][1]);
				s6 = (matrizPix.matrix[fila][columna + 1] * kernel[1][2]);
			}
			else if((fila == 0) && (columna == (matrizPix.n - 1))){
				s4 = (matrizPix.matrix[fila][columna - 1] * kernel[1][0]);
				s5 = (matrizPix.matrix[fila][columna] * kernel[1][1]);
				s7 = (matrizPix.matrix[fila + 1][columna - 1] * kernel[2][0]);
				s8 = (matrizPix.matrix[fila + 1][columna] * kernel[2][1]);
			}
			else if ((fila == (matrizPix.m - 1)) && (columna == (matrizPix.n - 1))){
				s1 = (matrizPix.matrix[fila - 1][columna - 1] * kernel[0][0]);
				s2 = (matrizPix.matrix[fila - 1][columna] * kernel[0][1]);
				s4 = (matrizPix.matrix[fila][columna - 1] * kernel[1][0]);
				s5 = (matrizPix.matrix[fila][columna] * kernel[1][1]);
			}
			else if (columna == 0){
				s2 = (matrizPix.matrix[fila - 1][columna] * kernel[0][1]);
				s3 = (matrizPix.matrix[fila - 1][columna + 1] * kernel[0][2]);
				s5 = (matrizPix.matrix[fila][columna] * kernel[1][1]);
				s6 = (matrizPix.matrix[fila][columna + 1] * kernel[1][2]);
				s8 = (matrizPix.matrix[fila + 1][columna] * kernel[2][1]);
				s9 = (matrizPix.matrix[fila + 1][columna + 1] * kernel[2][2]);
			}
			else if (fila == 0){
				s4 = (matrizPix.matrix[fila][columna - 1] * kernel[1][0]);
				s5 = (matrizPix.matrix[fila][columna] * kernel[1][1]);
				s6 = (matrizPix.matrix[fila][columna + 1] * kernel[1][2]);
				s7 = (matrizPix.matrix[fila + 1][columna - 1] * kernel[2][0]);
				s8 = (matrizPix.matrix[fila + 1][columna] * kernel[2][1]);
				s9 = (matrizPix.matrix[fila + 1][columna + 1] * kernel[2][2]);
			}
			else if (columna == (matrizPix.n - 1)){
				s1 = (matrizPix.matrix[fila - 1][columna - 1] * kernel[0][0]);
				s2 = (matrizPix.matrix[fila - 1][columna] * kernel[0][1]);
				s4 = (matrizPix.matrix[fila][columna - 1] * kernel[1][0]);
				s5 = (matrizPix.matrix[fila][columna] * kernel[1][1]);
				s7 = (matrizPix.matrix[fila + 1][columna - 1] * kernel[2][0]);
				s8 = (matrizPix.matrix[fila + 1][columna] * kernel[2][1]);
			}
			else if (fila == (matrizPix.m - 1)){
				s1 = (matrizPix.matrix[fila - 1][columna - 1] * kernel[0][0]);
				s2 = (matrizPix.matrix[fila - 1][columna] * kernel[0][1]);
				s3 = (matrizPix.matrix[fila - 1][columna + 1] * kernel[0][2]);
				s4 = (matrizPix.matrix[fila][columna - 1] * kernel[1][0]);
				s5 = (matrizPix.matrix[fila][columna] * kernel[1][1]);
				s6 = (matrizPix.matrix[fila][columna + 1] * kernel[1][2]);
			}
			else {
				s1 = (matrizPix.matrix[fila - 1][columna - 1] * kernel[0][0]);
				s2 = (matrizPix.matrix[fila - 1][columna] * kernel[0][1]);
				s3 = (matrizPix.matrix[fila - 1][columna + 1] * kernel[0][2]);
				s4 = (matrizPix.matrix[fila][columna - 1] * kernel[1][0]);
				s5 = (matrizPix.matrix[fila][columna] * kernel[1][1]);
				s6 = (matrizPix.matrix[fila][columna + 1] * kernel[1][2]);
				s7 = (matrizPix.matrix[fila + 1][columna - 1] * kernel[2][0]);
				s8 = (matrizPix.matrix[fila + 1][columna] * kernel[2][1]);
				s9 = (matrizPix.matrix[fila + 1][columna + 1] * kernel[2][2]);
			}
			resultado = (s1 + s2 +s3 +s4 +s5 +s6 +s7 +s8 +s9);
			matrizAux[fila][columna] = resultado / 9.0;
			printf("%f ", matrizAux[fila][columna]);
		}
		printf("\n");
	}

	return 1;
}