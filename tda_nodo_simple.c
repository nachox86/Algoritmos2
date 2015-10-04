#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tda_nodo_simple.h"
#define MAX_STR 200
#define RES_MEM_ERROR 2
#define RES_OK 0


int createSimpleNode(TDA_Nodo_Simple* Node)
{
    printf("entre a create\n");
    Node->Siguiente = NULL;
    /*Node->tname = NULL;
    Node->outputFile = NULL;*/
    return RES_OK;
}

int setNextSNode(TDA_Nodo_Simple* Node,TDA_Nodo_Simple* nextNode)
{
    Node->Siguiente = nextNode;
    return RES_OK;
}

int setNameSNode(TDA_Nodo_Simple* Node,char* namet)
{
    Node->tname = (char*)malloc((sizeof(char)*strlen(namet))+1);
    if(!Node->tname)
        return RES_MEM_ERROR;
    Node->tname = namet;
    return RES_OK;
}

int setOutputFileSNode(TDA_Nodo_Simple* Node,char* output)
{
    Node->outputFile = (char*)malloc(sizeof(char)*strlen(output)+1);
    if(!Node->outputFile)
        return RES_MEM_ERROR;
    Node->outputFile = output;
    return RES_OK;
}

char* getNameSNode(TDA_Nodo_Simple* Node)
{
    return Node->tname;
}

char* getOutputFileSNode(TDA_Nodo_Simple* Node)
{
    return Node->outputFile;
}

TDA_Nodo_Simple* getNextSNode(TDA_Nodo_Simple* Node)
{
    return Node->Siguiente;
}

int destroySNode(TDA_Nodo_Simple* Node)
{
    Node->Siguiente = NULL;
    free(Node->tname);
    free(Node->outputFile);
    return RES_OK;
}



