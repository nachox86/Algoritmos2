#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tda_nodo.h"
#define MAX_STR 200
#define MAX 100

int main (int argc,char** argv)
{
    TDA_Nodo nodo;

    char** elemento;
    char** comment;

    int count = 2;
    int statusCreateNode;
    int statussetCommentsNode;
    int statussetNextNode;
    TDA_Nodo* siguienteNodo;
    int destruido;

    elemento = (char**) malloc(sizeof(char*)*MAX);

    elemento[0] = (char*) malloc(sizeof(char)*MAX_STR);
    elemento[1] = (char*) malloc(sizeof(char)*MAX_STR);

    elemento[0] = "primer elemento";
    elemento[1] = "segundo elemento";

    statusCreateNode = createNode(&nodo,count);
    printf("creado el nodo con cantidad:%i\n",nodo.commentsCount);

    statussetCommentsNode = setCommentsNode(&nodo,elemento);
    printf("seteado el comentario: %s,%s\n",nodo.comments[0],nodo.comments[1]);

    statussetNextNode = setNextNode(&nodo,&nodo);
    printf("la cantidad del next node es: %i\n",nodo.Siguiente->commentsCount);
    comment = getCommentsNode(nodo);
    printf("comentario 1: %s\n",comment[0]);
    printf("comentario 2: %s\n",comment[1]);

    siguienteNodo = getNextNode(nodo);
    printf("lso comentarios del siguiente nodo: %s %s\n",siguienteNodo->comments[0],siguienteNodo->comments[1]);

    destruido = destroyNode(&nodo);
    printf("la cantidad del nodo destruido: %i\n",nodo.commentsCount);

    free(elemento[0]);free(elemento[1]);
    free(elemento);
    return 0;
}
