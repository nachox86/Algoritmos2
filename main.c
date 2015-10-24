#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tda_documentador.h"
#include "tda_nodo.h"
#include "htmlParser.h"
#include "logger_tda.h"

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
@funcion straight_list_order_insert
@descr Esta función realiza la inserción ordenada de un dato, en la posición correcta en una lista. Realiza la comparación entre nombres de funciones para ordernar.
@autor Ignacio
@fecha 14/10/2015
@version "1.0"
@param lp referencia a la lista
@param data referencia al dato a guardar en la lista
@pre la lista debe estar creada
@pos se guardará en la posición que deba de acuerda al criterio de ordenamiento.
*/
int iListOrderInsert(T_List *lp, void* data)
{
    int insert;
    T_Move mov = straight_list_first;
	search_site(lp,data,&mov);
	insert = InsertE(&lp,mov,data);
	return insert;
}


int search_site(T_List *lp, const void* data, T_Move *mov)
{
	void *current_data; /*referencia de donde se va a guardar el dato/elemento del corriente de la lista*/
	char **cdata, **rdata;

	if(EmptyList(lp))
		return FALSE;

	GetC(lp,current_data);

	cdata = ((char**)current_data);
	rdata = ((char**)data);
	/*validar que pudo hacer el get porque usa la función de copy*/
    /*data: sería un char**, con data[0]="nombre de la funcion", data[1]="nombre del archivo"*/

   	if (strcmp( cdata[0], rdata[0]) > 0)
	{
		MoveC(&lp,M_First);
		GetC(lp,current_data);
	}
    while(strcmp(data,cdata[0])>0 && MoveC(&lp,M_Next))
	{
		GetC(lp,current_data);
	}
	if(strcmp(rdata[0],cdata[0])<0)
	{
		*mov = M_Prev;
	}
	else
		*mov = M_Next;

	return TRUE;
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

    Logger *log;
    TDA_Doc *docu;

    if( (argc != ARGS_TWO) && (argc != ARGS_SEVEN))
    {
        showHelp();
        return RES_WRONG_ARGS;
    }

    for (i = 1; i < argc; i=i+2) {
        if ((strcmp(argv[i], ARGS_HELP1) == 0) || (strcmp(argv[i], ARGS_HELP2) == 0)) {
            showHelp();
            /*free (InputDir);
            free (logFile);
            free (outputFile);*/   /*ver si esto es asi*/
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

    	logLightInit(log, logFile, fatal);
    	createDoc(&docu, log);
    	extractDocumentation(docu, inputDir, outputFile);

    	/*armo el nombre del archivo de indice*/
        token = strtok(outputFile,HTML_EXT);

        if(!token) {
            indexFile = malloc(strlen(INDEX_PREFFIX_NO_EXT) + strlen(token));
            sprintf(indexFile, "%s%s",INDEX_PREFFIX_NO_EXT,token);
        } else {
            indexFile = malloc(strlen(INDEX_PREFFIX) + strlen(token));
            sprintf(indexFile, "%s%s",token,INDEX_PREFFIX);
        }
        createIndex(docu, indexFile);
		logClose(log);
    	destroyDoc(&docu);
    }
    else
    {
        showHelp();
        /*free(inputDir);
    	free(outputFile);
    	free(logFile);*/              /*ver aca tambien si hay que liberar memoria*/
        return RES_WRONG_ARGS;
    }

    free(inputDir);
    free(outputFile);
    free(logFile);

    return RES_OK;
}
