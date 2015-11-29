#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tda_documentador.h"
#include "htmlParser.h"
#include "logger_tda.h"

#define INDEX_PREFFIX "index preffix"        /*escribir q va en estas ctes*/
#define INDEX_PREFFIX_NO_EXT "no ext"
#define RES_WRONG_ARGS -6
#define ARGS_HELP1 "-h"
#define ARGS_HELP2 "--help"
#define ARGS_INPUT_FILE "-i"
#define ARGS_LOG_FILE "-l"
#define ARGS_OUTPUT_FILE "-o"
#define ARGS_TWO 2
#define ARGS_SEVEN 7
#define ARGS_REQ 3


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
    printf("Descripcion de los parámetros:\n");
    printf("-i: indica el nombre del archivo de entrada del que se toma la documentación.\n");
    printf("-l: indica el nombre del archivo de loggeo de eventos.\n");
    printf("-o: indica el nombre del archivo de salida de la documentación en formato html.\n");
    printf("-h o --help: muestra esta ayuda.\n");
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
int countFunctions(straight_list_t* listed)
{
    int i = 0;

    if(straight_list_is_empty(listed)!=FALSE)
    {
        straight_list_move(listed,straight_list_first);
        do{
            i++;
        }while(straight_list_move(listed,straight_list_first)!=FALSE);
    }
    straight_list_move(listed,straight_list_first);
    return i;
}

/*
@funcion main
@descr Función principal del programa. Dado los argumentos de entrada toma los comentarios de funciones de un archivo de entrada y los vuelca en un archivo de salida en formato html. También genera un índice en html y un Log de eventos.
@fecha 03/10/2015
@version "2.0"
@param argc cantidad de argumentos de entrada
@param argv[] array de argumentos de entrada
@pre Se debe ejecutar el modo de ayuda antes de usarlo para saber qué argumentos pasarle: tp2.exe -h o tp2.exe --help
@pos Devolverá en un archivo de salida, el índice con todas las funciones documentadas, en otro la documentación de esas funciones.
*/
int main(int argc, char *argv[]) {
	int i, nargs = 0;
    char *inputDir, *outputFile, *logFile, *indexFile;
    char* token;

    Logger *log = malloc(sizeof(Logger));
    TDA_Doc *docu;

    if( (argc != ARGS_TWO) && (argc != ARGS_SEVEN))
    {
        showHelp();
        return RES_WRONG_ARGS;
    }

    for (i = 1; i < argc; i=i+2) {
        if ((strcmp(argv[i], ARGS_HELP1) == 0) || (strcmp(argv[i], ARGS_HELP2) == 0)) {
            showHelp();
            free (inputDir);
            free (logFile);
            free (outputFile);
            return RES_OK;
        } else if (strcmp(argv[i], ARGS_INPUT_FILE) == 0) {
            inputDir = malloc(sizeof(char) * strlen(argv[i+1]) + 1);
            strcpy(inputDir, argv[i+1]);
            nargs++;
        } else if (strcmp(argv[i], ARGS_LOG_FILE) == 0) {
            logFile = malloc(sizeof(char) * strlen(argv[i+1]) + 1);
            strcpy(logFile, argv[i+1]);
            nargs++;
        } else if (strcmp(argv[i], ARGS_OUTPUT_FILE) == 0) {
            outputFile = malloc(sizeof(char) * strlen(argv[i+1]) + 1);
            strcpy(outputFile, argv[i+1]);
            nargs++;
        }
    }

    if (nargs == ARGS_REQ) {
    	/* We got enough arguments to proceed */

    	logLightInit(log, logFile, trace);
    	createDoc(&docu, log);
    	extractDocumentation(docu, inputDir, outputFile);

    	/*armo el nombre del archivo de indice*/
        token = strtok(outputFile,"html");

        if(!token) {
            indexFile = malloc(strlen(INDEX_PREFFIX_NO_EXT) + strlen(token));
            sprintf(indexFile, "%s%s",INDEX_PREFFIX_NO_EXT,token);
        } else {
            indexFile = malloc(strlen(INDEX_PREFFIX) + strlen(token));
            sprintf(indexFile, "%s%s",token,INDEX_PREFFIX);
        }
        /*
        createIndex(docu, indexFile);
        */
		logClose(log);
    	destroyDoc(&docu);
    }
    else
    {
        showHelp();
        free(inputDir);
    	free(outputFile);
    	free(logFile);
        return RES_WRONG_ARGS;
    }

    free(inputDir);
    free(outputFile);
    free(logFile);

    return RES_OK;
}
