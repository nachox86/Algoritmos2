#include "list_tda.h"
#include "tda_documentador.h"
#include <string.h>

#define RES_OK 0
#define RES_ERROR 1
#define RES_HELP 2
#define RES_NOT_ENOUGH_ARGS 3
#define RES_WRONG_ARGS 4
#define RES_CORRECT_ARGS 5

#define ARG_HELP1 -h
#define ARG_HELP2 --help
#define ARG_INPUT_FILE -i
#define ARG_LOG_FILE -l
#define ARG_OUPUT_FILE -o
#define ARG_INDEX ->

#define TWO 2
#define SEVEN 7

#define KW_TITLE       "@titulo"
#define KW_SUBTITLE    "@subtitulo"
#define KW_FUNCTION    "@funcion"
#define KW_DESCRIPTION "@descr"
#define KW_AUTHOR      "@autor"
#define KW_DATE        "@fecha"
#define KW_VERSION     "@version"
#define KW_PARAM       "@param"
#define KW_RETURN      "@return"
#define KW_PRE         "@pre"
#define KW_POST        "@pos"


/*
@funcion showHelp
@descr Muestra la ayuda del programa.
@autor Ignacio
@fecha 03/10/2015
@version "1.0"
@param ninguno
@pre ninguna condición necesaria
@pos devuelve en pantalla la ayuda del programa.
*/
void showHelp()
{
    printf("Este programa permite obtener en formato html, la documentación provista de un archivo de texto.\n");
    printf("Uso:\n");
    printf("Documentador.exe -i archivo_a_documentar.c -l archivo_log.txt -o archivo_documentado.html\n");
    printf("Documentador.exe -h\n");
    printf("Documentador.exe --help\n");
    printf("Descripción de los parámetros:\n");
    printf("-i: indica el nombre del archivo de entrada del que se toma la documentación.\n");
    printf("-l: indica el nombre del archivo de loggeo de eventos.\n");
    printf("-o: indica el nombre del archivo de salida de la documentación en formato html.\n");
    printf("-h o --help: muestra esta ayuda.\n");
}

/*
@funcion validateInput
@descr Valida que los argumentos de entrada sean válidos al igual que la cantidad.
@autor Ignacio
@fecha 30/09/2015
@version "1.0"
@param carg cantidad de argumentos
@param varg[] array con los argumentos de entrada
@pre ninguna condición necesaria
@pos Devolverá RES_NOT_ENOUGH_ARGS si no están la cantidad apropiada de argumentos.
     Devolverá RES_HELP si se pide la ayuda.
     Devolverá RES_WRONG_ARGS si se ingresaron mal los argumentos.
     Devolverá RES_CORRECT_ARGS si se ingresaron correctamente los argumentos.
*/

/*
int validateInput(int carg, char *varg[])
{
    if(carg!=TWO && carg!= SEVEN)
    {
        showHelp();
        return RES_NOT_ENOUGH_ARGS;
    }
    else
    {
        if(carg==TWO)
        {
            if(strcmp(varg[1],HELP1)==0||strcmp(varg[1],HELP2)==0)
            {
                showHelp();
                return RES_HELP;
            }
            else
                return RES_WRONG_ARGS;
        }
        else if(carg==SEVEN)
        {
            if((strcmp(varg[1],ARG_INPUT_FILE)==0&&strcmp(varg[3],ARG_LOG_FILE)==0&&strcmp(varg[5],ARG_OUTPUT_FILE)==0)||
               (strcmp(varg[1],ARG_INPUT_FILE)==0&&strcmp(varg[3],ARG_OUTPUT_FILE)==0&&strcmp(varg[5],ARG_LOG_FILE)==0)||
               (strcmp(varg[1],ARG_LOG_FILE)==0&&strcmp(varg[3],ARG_INPUT_FILE)==0&&strcmp(varg[5],ARG_OUTPUT_FILE)==0)||
               (strcmp(varg[1],ARG_LOG_FILE)==0&&strcmp(varg[3],ARG_OUTPUT_FILE)==0&&strcmp(varg[5],ARG_INPUT_FILE)==0)||
               (strcmp(varg[1],ARG_OUTPUT_FILE)==0&&strcmp(varg[3],ARG_INPUT_FILE)==0&&strcmp(varg[5],ARG_LOG_FILE)==0)||
               (strcmp(varg[1],ARG_OUTPUT_FILE)==0&&strcmp(varg[3],ARG_LOG_FILE)==0&&strcmp(varg[5],ARG_INPUT_FILE)==0)||)
               {
                   return RES_CORRECT_ARGS;
               }
               else
                return RES_WRONG_ARGS;
        }

    }
}
*/

/*
@funcion checkForKW
@descr Dado un string de caracteres se fija si se encuentra alguna de las palabras claves definidas.
@autor Ignacio
@fecha 01/10/2015
@version "1.0"
@param linea array de caracteres
@pre ninguna condición necesaria
@pos Devolverá RES_OK si encontró alguna palabra clave o RES_ERROR si no.
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
        /*encontró un parametro*/
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
@pos Devolverá la cantidad de nodos que encontró en la lista.
*/
int countFunctions(T_List* listed, TDA_Doc *docu)
{
    int i = 0;

    if(EmptyList(*listed)!=FALSE)
    {
        MoveC(listed,M_First);
        do{
            i++;
        }while(MoveC(docu->listado,M_Next)!=FALSE);
    }
    MoveC(listed,M_First);
    return i;
}
