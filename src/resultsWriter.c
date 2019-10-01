#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <png.h>
#include <errno.h>
#include <stdarg.h>
#include "main.h"

//Entrada: Estructura bitmap_t que contiene los pixeles, ancho y alto de la imagen/ posicion en columnas/ posicion en filas.
//Funcionamiento: Extrae el pixel ubicado en la posicion (y,x).
//Salida: pixel.
static pixel_t * pixel_at (bitmap_t * bitmap, int x, int y)
{
    return bitmap->pixels + bitmap->width * y + x;
}

//Entrada: Estructura bitmap_t que contiene los pixeles, ancho y alto de la imagen/ string que contiene el nombre del archivo (imagen) de salida.
//Funcionamiento: Inicializa todas las estructuras presentes en un archivo PNG para posteriormente ser llenado con los valores 
//      presentes en la estructura bitmap_t.
//Salida: un entero que indica el estado de finalizacion de la función.  
static int save_png_to_file (bitmap_t *bitmap, const char *path)
{
    //Definicion de variables.
    FILE * fp;
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    size_t x, y;
    png_byte ** row_pointers = NULL;
   
    int status = -1;
    
    int pixel_size = 3;
    int depth = 8;
    //Apertura del archivo
    fp = fopen (path, "wb");
    if (! fp) {
        goto fopen_failed;
    }
    //Inicializacion de las estructuras requeridas
    png_ptr = png_create_write_struct (PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL) {
        goto png_create_write_struct_failed;
    }
    
    info_ptr = png_create_info_struct (png_ptr);
    if (info_ptr == NULL) {
        goto png_create_info_struct_failed;
    }
    

    if (setjmp (png_jmpbuf (png_ptr))) {
        goto png_failure;
    }
    
    //Seteo del header del archivo PNG
    png_set_IHDR (png_ptr,
                  info_ptr,
                  bitmap->width,
                  bitmap->height,
                  depth,
                  PNG_COLOR_TYPE_GRAY,
                  PNG_INTERLACE_NONE,
                  PNG_COMPRESSION_TYPE_DEFAULT,
                  PNG_FILTER_TYPE_DEFAULT);
    
    //Recorrido y llenado de la matriz de pixeles con los datos de bitmap
    row_pointers = png_malloc (png_ptr, bitmap->height * sizeof (png_byte *));
    for (y = 0; y < bitmap->height; y++) {
        png_byte *row = 
            png_malloc (png_ptr, sizeof (u_int8_t) * bitmap->width);
        row_pointers[y] = row;
        for (x = 0; x < bitmap->width; x++) {
            pixel_t * pixel = pixel_at (bitmap, x, y);
            *row++ = pixel->color;
        }
    }
    
    //Inicializacion, configuracion y escritura de la imagen.
    png_init_io (png_ptr, fp);
    png_set_rows (png_ptr, info_ptr, row_pointers);
    png_write_png (png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

    

    status = 0;
    //liberacion de la memoria utilizada en el proceso.
    for (y = 0; y < bitmap->height; y++) {
        png_free (png_ptr, row_pointers[y]);
    }
    png_free (png_ptr, row_pointers);
    
 png_failure:
 png_create_info_struct_failed:
    png_destroy_write_struct (&png_ptr, &info_ptr);
 png_create_write_struct_failed:
    fclose (fp);
 fopen_failed:
    return status;
}

//Entrada: Matriz de flotantes que contienen los valores de los pixeles / String que indica el nombre de salida.
//Funcionamiento: Se prepara la estructura necesaria (bitmap_t) con los datos de la matriz de flotantes y se escribe la imagen.
//Salida: entero que indica el estado del resultado de la función.
int writeImage(floatPixelMatrix matrizPix, char * fileOut){
    bitmap_t pngOut;
    int xg;
    int yg;
    int status;
    status = 0;

    /* Se crean las struct */
    
    pngOut.width = matrizPix.n;
    pngOut.height = matrizPix.m;
    pngOut.pixels = calloc (pngOut.width * pngOut.height, sizeof (pixel_t));
    if (! pngOut.pixels) {
    return -1;
    }
    //Se recorre y crea una matriz con pixeles
    for (yg = 0; yg < pngOut.height; yg++) {
        for (xg = 0; xg < pngOut.width; xg++) {
            pixel_t * pixel = pixel_at (& pngOut, xg, yg);
            pixel->color = (int) matrizPix.matrix[yg][xg];
        }
    }

    /* Se escribe la imagen */

    if (save_png_to_file (& pngOut, fileOut)) {
        fprintf (stderr, "Error escribiendo archivo.\n");
        status = -1;
    }

    free (pngOut.pixels);
    return status;
}
//Entradas: En argv se debe ingresar -c <Cantidad de imagenes> -b <1 o 0 dependiendo si se quiere mostrar la evaluacion nearlyblack en el terminal>.
//Funcionamiento: Realiza la escritura de resultados. Primero, se leen las entradas del argv usando getopt. Luego, se realiza la escritura de resultados.
//Salida: Escribe el resultado del pipeline en el archivo de salida. El formato del archivo de salida es out_n, donde n es el numero de la imagen.
int main(int argc, char **argv){
    int cValue = 0;
    int bFlag = 0;
    int flag;
    while( (flag = getopt(argc, argv, "c:b:")) != -1){
        switch (flag)        {
        case 'c':
            cValue = atoi(optarg);
            break;
        case 'b':
            bFlag = atoi(optarg);
            break;
        
        default:
            abort();
        }
    }
    //Escritura de resultados
    //Si se requiere mostrar la evaluacion de nearlyblack
    if(bFlag == 1){
        printf("|   image   | nearly black |\n");
        printf("|-----------|--------------|\n");
    }
    //Por cada imagen
    for(int i= 0; i<cValue; i++){
        //Se define el valor del nombre del archivo de salida
        char fileName[20];
        char index[14];
        strcpy(fileName, "out_");
        sprintf(index,"%d",i+1);
        strcat(fileName,index);

        floatPixelMatrix floatPixels;
        read(STDIN_FILENO, &floatPixels, sizeof(floatPixelMatrix));
        //Si se requiere mostrar la evaluacion de nearlyblack se muestra el resultado
        if(bFlag == 1){
            if(floatPixels.nearlyBlack==1){
                printf("|  imagen_%d |     yes      |\n",i+1);
            }
            else if(floatPixels.nearlyBlack==0){
                printf("|  imagen_%d |      no      |\n",i+1);
            }
        }
        //Se escribe el resultado del pipeline
        int status = writeImage(floatPixels, fileName);

    }
    return 0;
}