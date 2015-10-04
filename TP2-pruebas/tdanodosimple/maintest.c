#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tda_nodo_simple.h"
#define MAX_STR 200
#define MAX 100

int main (int argc,char** argv)
{
    TDA_Nodo_Simple nodo;
    TDA_Nodo_Simple* nodo2;

    char nombre[MAX_STR] = "funcion de prueba";
    char archsalida[MAX_STR] = "zarasa";
    char* getNombre;
    char* getSalida;
    int creado;
    int setNodo;
    int setNombre;
    int setSalida;

    printf("%s",nombre);

    creado = createSimpleNode(&nodo);
    printf("Creado: %i\n",creado);

    setNodo = setNextSNode(&nodo,&nodo);
    printf("Set nodo: %i\n",setNodo);

    setNombre = setNameSNode(&nodo,nombre);
    printf("Set nombre: %s\n",nodo.tname);

    setSalida = setOutputFileSNode(&nodo,archsalida);
    printf("Set salida: %s\n",nodo.outputFile);

    getNombre = getNameSNode(&nodo);
    printf("Get nombre: %s\n",getNombre);

    getSalida = getOutputFileSNode(&nodo);
    printf("Get salida: %s\n",getSalida);

    nodo2 = getNextSNode(&nodo);
    printf("Get salida del next node: %s\n",nodo2->outputFile);

    destroySNode(&nodo);


    return 0;
}
