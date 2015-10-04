#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tda_documentador.h"
#include "tda_nodo.h"
#include "list_tda.h"
#include "htmlParser.h"
#include "functions_tools.c"

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
    char *inputDir, *outputFile, *logfile;
    
    Logger *log;
    TDA_Doc *docu;

    if (argc < 7) {
    	printf("Not enough args\n"); /* TODO: Do something else with this */
        return -1; /* NOT ENOUGH ARGS */
    }

    for (i = 1; i < argc; i=i+2) {
        if ((strcmp(argv[i], "-h") == 0) || (strcmp(argv[i], "--help") == 0)) {
            showHelp();
            break;
        } else if (strcmp(argv[i], "-i") == 0) {
            inputDir = malloc(sizeof(char) * strlen(argv[i+1]) + 1);
            strcpy(inputDir, argv[i+1]);
            nargs++;
        } else if (strcmp(argv[i], "-l") == 0) {
            logfile = malloc(sizeof(char) * strlen(argv[i+1]) + 1);
            strcpy(logfile, argv[i+1]);
            nargs++;
        } else if (strcmp(argv[i], "-o") == 0) {
            outputFile = malloc(sizeof(char) * strlen(argv[i+1]) + 1);
            strcpy(outputFile, argv[i+1]);
            nargs++;
        }
    }

    if (nargs == 3) {
    	/* We got enough arguments to proceed */
    	
    	createLog(&log, logfile);
    	createDoc(&docu, log);
    	extraerDocumentation(docu, inputDir, outputFile);
    	/* Create Index? */
    	destroyDoc(&docu);
    	closeLog(&log);
    }

    free(inputDir);
    free(outputFile);
    free(logfile);

    return 0;
}
