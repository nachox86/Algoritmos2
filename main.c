#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tda_documentador.h"
#include "tda_nodo_simple.h"
#include "tda_nodo.h"
#include "tda_indice.h"
#include "list_tda.h"
#include "htmlParser.h"
#include "function_tools.c"
#include "logger.h"


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
    int checkForHTML = 0;

    Logger *log;
    TDA_Doc *docu;

    if(argc!=ARG_TWO && argc!=ARG_SEVEN)
    {
        showHelp();
        return RES_NOT_ENOUGH_ARGS;
    }

    for (i = 1; i < argc; i=i+2) {
        if ((strcmp(argv[i],ARG_HELP1) == 0) || (strcmp(argv[i],ARG_HELP2) == 0)) {
            showHelp();
            return RES_HELP;
        } else if (strcmp(argv[i],ARG_INPUT_FILE) == 0) {
            inputDir = malloc(sizeof(char) * strlen(argv[i+1]) + 1);
            strcpy(inputDir, argv[i+1]);
            nargs++;
        } else if (strcmp(argv[i],ARG_LOG_FILE) == 0) {
            logFile = malloc(sizeof(char) * strlen(argv[i+1]) + 1);
            strcpy(logfile, argv[i+1]);
            nargs++;
        } else if (strcmp(argv[i],ARG_OUPUT_FILE) == 0) {
            outputFile = malloc(sizeof(char) * strlen(argv[i+1]) + 1);
            strcpy(outputFile, argv[i+1]);
            nargs++;
        }
    }

    if (nargs == ARG_MAX) {
    	/* We got enough arguments to proceed */

    	createLog(&log, logFile);
    	createDoc(&docu, log);
    	extractDocumentation(docu, inputDir, outputFile);
    	/*armo el nombre del archivo de indice*/
        /*token = strtok(outputFile,HTML_EXT);
        if(!token)
            indexFile = sprintf("%s%s",INDEX_PREFFIX_NO_EXT,token);
        else
            indexFile = sprintf("%s%s",token,INDEX_PREFFIX);
    	 createIndex(TDA_Doc *docu, char *indexFile); */
    	destroyDoc(&docu);
    	closeLog(&log);
    }
    else
    {
        showHelp();
        return RES_WRONG_ARGS;
    }

    free(inputDir);
    free(outputFile);
    free(logfile);

    return RES_OK;
}
