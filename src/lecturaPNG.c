#include <png.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "main.h"

int main ()
{
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
    PixelMatrix matrizPix;
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
   	return 0;
}