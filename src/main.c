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


//Entradas: char* filename: Nombre de una imagen en formato .png en escala de grises
//Funcionamiento: Lee una imagen y la guarda el largo, ancho y valor de sus pixeles en una matriz 
//Salida: estructura pixelMatrix que contiene largo, ancho y matriz de pixeles de la imagen
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
    fclose(fp);

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
   	return matrizPix;
}

//Entradas: int cValue: Entero positivo que indica la cantidad de imágenes a procesar
//          char* mValue: Nombre del archivo que contiene el filtro de la convolucion
//          int nValue: Entero positivo entre 0 y 100 que representa el umbral para definir si una imagen es nearlyblack o no
//          int bFlag: 0 o 1 que determina si se muestra en el terminal los resultados de la evaluación nearlyblack (bFlag = 1: muestra los resultados) 
//Funcionamiento: Primero prepara las entradas (argv) de requiere cada proceso. Luego, se crean los pipes y procesos, relacionandolos según corresponda. Finalmente, se libera memoria. A este proceso se le delega la lectura de imágenes
//Salida: --
void pipeline(int cValue, char * mValue, int nValue, int bFlag){
    pid_t pid1, pid2, pid3, pid4, pid5;
    int  status1;

    //Preparación entrada (argv) de cada proceso
    char convolutionFilterValueUpLeft[10];
    char convolutionFilterValueUp[10];
    char convolutionFilterValueUpRight[10];
    char convolutionFilterValueLeft[10];
    char convolutionFilterValueMiddle[10];
    char convolutionFilterValueRight[10];
    char convolutionFilterValueDownLeft[10];
    char convolutionFilterValueDown[10];
    char convolutionFilterValueDownRight[10];

    FILE * file = fopen(mValue, "r");
    fscanf(file,"%s %s %s %s %s %s %s %s %s",convolutionFilterValueUpLeft,convolutionFilterValueUp,convolutionFilterValueUpRight,convolutionFilterValueLeft,convolutionFilterValueMiddle,convolutionFilterValueRight,convolutionFilterValueDownLeft,convolutionFilterValueDown,convolutionFilterValueDownRight);
    fclose(file);

    char bFlagStr[2];
    sprintf(bFlagStr, "%d", bFlag);
    char nValueStr[3];
    sprintf(nValueStr, "%d", nValue);
    char cValueStr[3];
    sprintf(cValueStr, "%d", cValue);
    char mValueStr[100];
    strcpy(mValueStr, mValue);

    char * argvConvolution[] ={"convolution","-c",cValueStr,"-o",convolutionFilterValueUpLeft,"-p",convolutionFilterValueUp,"-q",convolutionFilterValueUpRight,"-r",convolutionFilterValueLeft,"-s",convolutionFilterValueMiddle,"-t",convolutionFilterValueRight,"-u",convolutionFilterValueDownLeft,"-v",convolutionFilterValueDown,"-w",convolutionFilterValueDownRight,NULL};
    char * argvRectification[] ={"rectification","-c",cValueStr, NULL};
    char * argvPooling[] ={"pooling","-c",cValueStr, NULL};
    char * argvClassifier[] ={"classifier","-n", nValueStr,"-c",cValueStr, NULL};
    char * argvResultsWriter[] ={"resultsWriter","-b", bFlagStr,"-c",cValueStr, NULL};

    //Creacion de los pipes y procesos
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

        
        //Main (lectura de imagenes)
        //Para cada imagaen
        for(int i = 0; i<cValue; i++){
            pixelMatrix pixels;
            //Se define el nombre del archivo para la lectura
            char fileName[20];
            char index[14];
            strcpy(fileName, "imagen_");
            sprintf(index,"%d",i+1);
            strcat(fileName,index);
            //Se lee el archivo de lectura
            pixels = pngRead(fileName);
            write(STDOUT_FILENO, &pixels, sizeof(pixelMatrix));
        }
    }
    //Se espera a que finalice la etapa de Convolcion
    wait(&status1);
    //Se libera memoria
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


//Entradas: En argv se debe ingresar -c <Cantidad de imagenes> -m <Nombre del archivo con el filtro> -n <Umbral para determinar nearlyblack> <-b si se requiere mostrar la evaluación de nearlyblack en el terminal>.
//Funcionamiento: Primero se leen los argumentos del main usando getopt. Luego, se evaluan si los parametros son adecuados. Finalmente, se ejecuta el pipeline.
//Salida: --
int main(int argc, char **argv){
    //Lectura de  las entradas del main
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
    //
    //printf("cValue = %d, mValue = %s, nValue = %d, bFlag = %d\n", cValue, mValue, nValue, bFlag);
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
        pipeline(cValue, mValue, nValue, bFlag);
    }
    return 0;
}
