#include <stdio.h>
#include <stdlib.h>
#include "lab1.h"

int main(){
    struct parEnteros *punteroEstructura;
    punteroEstructura = malloc(sizeof(struct parEnteros));
    punteroEstructura -> a = 40;
    printf("hola: %d\n",punteroEstructura -> a);
    free(punteroEstructura);
    punteroEstructura = NULL;
    myPrintHelloMake();
    return 0;
}