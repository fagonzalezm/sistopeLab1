#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "main.h"

#include <png.h>
#include <errno.h>
#include <stdarg.h>

#define READ 0
#define WRITE 1



pixelMatrix pngRead(char * fileName){
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
    fp = fopen (fileName, "rb");
    if (! fp) {
		//printf("Error, el archivo no pudo ser abierto.\n");
    }
    //Se crea la estructura de lectura de PNG
    png_ptr = png_create_read_struct (PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (! png_ptr) {
		//printf("No se pudo crear la estructura PNG de lectura.\n");
    }
    //Se crea la estructura de informacion de PNG
    info_ptr = png_create_info_struct (png_ptr);
    if (! png_ptr) {
		//printf("No se pudo crear la estructura PNG de informacion.\n");
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
    int rowbytes;
    //Se obtiene la cantidad de bytes necesarios para contener una fila de una imagen.
    rowbytes = png_get_rowbytes (png_ptr, info_ptr);

    matrizPix.m = alto;
    matrizPix.n = ancho;
    //Se recorren los punteros de la matriz pixel a pixel y se guardan en una estructura.
    for (x = 0; x < alto; x++) {
		int y;
		png_bytep row;
		row = rows[x];
      for (y = 0; y < ancho; y++) {
        png_byte pixel;
        pixel = row[y];
        //printf("%d", pixel);
        (matrizPix.matrix)[x][y] = (int) pixel;
        
      }
		//printf ("\n");
    }
    pclose(fp);
   	return matrizPix;
}

void pipeline(int cValue, char * mValue, int nValue, int bFlag){
    printf("HOLI1\n");
    pid_t pid1, pid2, pid3, pid4, pid5;
    int  status1;

    char bFlagStr[2];
    sprintf(bFlagStr, "%d", bFlag);
    char nValueStr[3];
    sprintf(nValueStr, "%d", nValue);
    char cValueStr[3];
    sprintf(cValueStr, "%d", cValue);
    char mValueStr[100];
    strcpy(mValueStr, mValue);

    char * argvConvolution[] ={"convolution","-c",cValueStr, NULL};
    char * argvRectification[] ={"rectification","-c",cValueStr, NULL};
    char * argvPooling[] ={"pooling","-c",cValueStr, NULL};
    char * argvClassifier[] ={"classifier","-n", nValueStr,"-c",cValueStr, NULL};
    char * argvResultsWriter[] ={"resultsWriter","-b", bFlagStr,"-c",cValueStr, NULL};

    int * pipe1 = (int *)malloc(2*sizeof(int));
    int * pipe2 = (int *)malloc(2*sizeof(int));
    int * pipe3 = (int *)malloc(2*sizeof(int));
    int * pipe4 = (int *)malloc(2*sizeof(int));
    int * pipe5 = (int *)malloc(2*sizeof(int));
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
                    execvp("bin/pooling",argvPooling);
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

        //Ejemplo leyendo archivo
        /*
        FILE * fp;
        char aux[20];
        char index[14];
        char element[20];
        char newline;
        strcpy(aux, "imagen_");
        for(int i = 0; i<cValue; i++){
            int k = 0;
            int l = 0;
            int m = 0;
            int n = 0;

            sprintf(index,"%d",i+1);
            strcat(aux,index);
            fp = fopen(aux, "r");
            int saltoDeLinea = 0;
            while(!feof(fp)){
                fscanf(fp,"%s", element);
                (pixels.matrix)[k][l] = atoi(element);
                l = l + 1;
                newline = fgetc(fp);
                if(saltoDeLinea == 0){
                    n = n + 1;
                }
                if(newline == '\n'){
                    l = 0;
                    k = k + 1;
                    m = m + 1;  
                    saltoDeLinea = 1;
                }

            }

            pixels.m = m;
            pixels.n = n;
            for(int i = 0; i<pixels.m;i++){
                for(int j = 0; j<pixels.n;j++){
                }
            }
            
            strcpy(aux, "imagen_");
            fclose(fp);
            write(STDOUT_FILENO, &pixels, sizeof(pixelMatrix));
            pixels.m = 0;
            pixels.n = 0;
            
        }*/
        
        for(int i = 0; i<cValue; i++){
            pixelMatrix pixels;
            char fileName[20];
            char index[14];
            strcpy(fileName, "imagen_");
            sprintf(index,"%d",i+1);
            strcat(fileName,index);
            pixels = pngRead(fileName);
            write(STDOUT_FILENO, &pixels, sizeof(pixelMatrix));
        }
    }
    wait(&status1);
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
        printf("HOLI0\n");
        pipeline(cValue, mValue, nValue, bFlag);
    }
    return 0;
}
