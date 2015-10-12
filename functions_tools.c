#include "list_tda.h"
#include "tda_nodo.h"
#include "tda_documentador.h"
#include <string.h>
#include "functions_tools.h"

/*
@funcion showHelp
@descr Muestra la ayuda del programa.
@autor Ignacio
@fecha 03/10/2015
@version "1.0"
@param ninguno
@pre ninguna condici�n necesaria
@pos devuelve en pantalla la ayuda del programa.
*/
void showHelp()
{
    printf("Este programa permite obtener en formato html, la documentaci�n provista de un archivo de texto.\n");
    printf("Uso:\n");
    printf("Documentador.exe -i archivo_a_documentar.c -l archivo_log.txt -o archivo_documentado.html\n");
    printf("Documentador.exe -h\n");
    printf("Documentador.exe --help\n");
    printf("Descripcion de los par�metros:\n");
    printf("-i: indica el nombre del archivo de entrada del que se toma la documentaci�n.\n");
    printf("-l: indica el nombre del archivo de loggeo de eventos.\n");
    printf("-o: indica el nombre del archivo de salida de la documentaci�n en formato html.\n");
    printf("-h o --help: muestra esta ayuda.\n");
}

/*
@funcion checkForKW
@descr Dado un string de caracteres se fija si se encuentra alguna de las palabras claves definidas.
@autor Ignacio
@fecha 01/10/2015
@version "1.0"
@param linea array de caracteres
@pre ninguna condici�n necesaria
@pos Devolver� RES_OK si encontr� alguna palabra clave o RES_ERROR si no.
*/
int checkForKW(char* linea)
{
    if(strstr(linea,KW_TITLE)||
       strstr(linea,KW_SUBTITLE)||
       strstr(linea,KW_FUNCTION)||
       strstr(linea,KW_DESCRIPTION)||
       strstr(linea,KW_AUTHOR)||
       strstr(linea,KW_DATE)||
       strstr(linea,KW_VERSION)||
       strstr(linea,KW_PARAM)||
       strstr(linea,KW_RETURN)||
       strstr(linea,KW_PRE)||
       strstr(linea,KW_POST))
        /*encontr� un parametro*/
    {
        return RES_OK;
    }
    return RES_ERROR;
}

/*
@funcion countFunctions
@descr Cuenta la cantidad de nodos en un lista. El corriente de la lista puede estar en cualquier lado de la lista.
@autor Ignacio
@fecha 01/10/2015
@version "1.0"
@param listed referencia a la lista
@pre la lista debe estar creada
@pos Devolver� la cantidad de nodos que encontr� en la lista.
*/
int countFunctions(T_List* listed)
{
    int i = 0;

    if(EmptyList(listed)!=FALSE)
    {
        MoveC(&listed,M_First);
        do{
            i++;
        }while(MoveC(&listed,M_Next)!=FALSE);
    }
    MoveC(&listed,M_First);
    return i;
}

/*
@funcion getCommentsCount
@descr Esta es una funci�n que devuelve la cantidad de comentarios almacenados en un dato/elemento que se le pasa (se usa para el elemento del nodo).
@autor Ignacio
@fecha 04/10/2015
@version "1.0"
@param Node nodo
@pre Node debe estar creado
@pos Devolver� la cantidad de comentarios almacenados.
*/
int getCommentsCount(char** dato)
{
    int count = 0;

    while(dato[count]!='\0')
        count++;

    return count;
}

