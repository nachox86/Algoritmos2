#include <string.h>
#include "tda_documentador.h"
#include "logger.h"
#include "list_tda.h"
#include "tda_nodo.h"
#include "htmlParser.h"
#include "functions_tools.h"
#include "straight_list.h"

#include <sys/types.h>
#include <dirent.h>

#define MAX_LINE 300
#define MAX_KW 11
#define KW_INIT        "/*"
#define KW_END         "*/"
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

#define HTML_INDEX_HEADER "<h1>Indice</h1>\n<hl/>\n<ul>"
#define HTML_INDEX_FOOTER "</ul>"

#define MSG_ERROR_IN_FILE "Ocurrió un error al abrir el archivo de entrada de datos."
#define MSG_ERROR_INDEX_FILE "Ocurrió un error al abrir el archivo de índices."
#define MSG_ERROR_MEMORY "Error al pedir recursos para variable auxiliar."
#define MSG_ERROR_OUT_FILE "Ocurrió un error al abrir el archivo de salida de datos."

/* TODO: Check this declaration (done in situ for debug purposes */
#define RES_MEM_ERROR   -1

int extractDocumentationFromFile(TDA_Doc *docu, htmlFile *html, char *iFile);

int createDoc(TDA_Doc **docu, Logger *log)
{
	(*docu) = malloc(sizeof(TDA_Doc));

    (*docu)->inputFile = NULL;
    (*docu)->outputFile = NULL;
    (*docu)->logFile = log;
    (*docu)->listado = NULL;
    (*docu)->slist = malloc(sizeof(straight_list_t));
    return RES_OK;
}

int extractDocumentation(TDA_Doc *docu, char *inputDir, char *outputFile) {
    DIR *dir;
    struct dirent *dp;
    char **buffer;
    htmlFile *html;
    int i, n = 0;

    if (inputDir[strlen(inputDir)] != '/')
    	strcat(inputDir, "/");

    dir = opendir(inputDir);

    if (dir) {
        buffer = malloc(sizeof(char**));
        while (dp = readdir(dir)) {
            if ((strcmp(dp->d_name, ".") != 0) && (strcmp(dp->d_name, "..") != 0)) {
                buffer = realloc(buffer, sizeof(char**) * (n+2));
                buffer[n] = malloc(strlen(inputDir) + strlen(dp->d_name) + 1);
                strcpy(buffer[n], inputDir);
                strcat(buffer[n], dp->d_name);
                n++;
            }
        }
        closedir(dir);
    } else {
        n = -1;
    }

    if ( n >= 0) {
        straight_list_create(docu->slist,sizeof(char**), &slistCopy, &sListDestroy);
        straight_list_move(docu->slist,straight_list_first);

        createHtmlFile(&html, outputFile);
        for (i = 0; i < n; i++) {
            extractDocumentationFromFile(docu, html, buffer[i]);
            free(buffer[i]);
        }
        free(buffer);
        closeHtmlFile(&html);
        return RES_OK;
    } else {
        return RES_ERROR;
    }
}

int extractDocumentationFromFile(TDA_Doc *docu, htmlFile *html, char *iFile) {
    FILE *inputFile;
    char **comms = NULL;
    int i = 0, j = 0;
    int n = 0;
    int count = 0;
    int cinit = 0;
    int cend = 0;
    int cfound = 0;
    int lenElem = 0;

    char linea[MAX_LINE];
    char *stoken, *etoken;

    char **index;

    inputFile = fopen(iFile, "r");
    if (!inputFile) return RES_ERROR;

    comms = malloc((sizeof(char*)*MAX_LINE));
    if (!comms) return RES_ERROR;

    index = malloc(sizeof(char*));
    if (!index) return RES_ERROR;

    CreateList(&(docu->listado), sizeof(char**));

    while (!feof(inputFile)) {
        if (fgets(linea, MAX_LINE-1, inputFile) != NULL) {
            stoken = strstr(linea, KW_INIT);
            etoken = strstr(linea, KW_END);

            if ((stoken) && (!etoken)) {
                cinit = 1;
            } else if ((!stoken) && (etoken)) {
                cend = 1;
                if (cinit)
                    cfound = 1;
            }

            if ((cinit) && (!cend)) {
                if (checkForKW(linea) == 0) {
                    comms[count] = malloc(strlen(linea) + 1);
                    strncpy(comms[count], linea, strlen(linea) + 1);
                    count++;
                }
            } else if (cend) {
                if (count > 0) {
                    if (EmptyList(docu->listado))
                        InsertE(&(docu->listado), M_First, comms);
                    else
                        InsertE(&(docu->listado), M_Next, comms);

                    cinit = 0;
                    cend = 0;
                }
            }
        }
    }

    fclose(inputFile);

    MoveC(&(docu->listado), M_First);

    do {
        GetC((docu->listado), comms);

        for (i = 0; i < count; i++) {
            if (comms[i] == NULL) {
                return -4;
            } else {
                parseStringToHtml(html, comms[i]);

                index[0] = malloc(strlen(comms[i]) + 1);
                index[1] = malloc(strlen(iFile) + 1);
                strcpy(index[0], comms[i]);
                strcpy(index[1], iFile);

                if(straight_list_is_empty(docu->slist))
                    straight_list_insert(docu->slist, straight_list_first, index);
                else
                    straight_list_insert(docu->slist, straight_list_next, index);
                free(index[0]);
                free(index[1]);
            }
        }
    } while (MoveC(&(docu->listado), M_Next) != 0);

    for (i = 0; i < count; i++)
        free(comms[i]);

    free(comms);

    free(index);

    ClearList(&(docu->listado));

    return RES_OK;
}

int createIndex(TDA_Doc *docu, char *indexFile)
{
    FILE* index;
    char htmlLine[MAX_LINE];
    void **data;
    char **buffer;

    /*doy formato y agrego al archivo de indices*/
    index = fopen(indexFile,"w");
    if(!index)
    {
        loge(docu->logFile,MSG_ERROR_INDEX_FILE);
        return RES_ERROR;
    }

    fwrite(HTML_INDEX_HEADER,sizeof(char),strlen(HTML_INDEX_HEADER),index); /*agrego header del indice*/

    /*Me muevo por la lista del indice desde el principio*/

    straight_list_move(docu->slist,straight_list_first);

    data = malloc((sizeof(char**)*2));
    data[0] = malloc((sizeof(char*)*20));
    data[1] = malloc((sizeof(char*)*20));

    do{
        straight_list_get(docu->slist,data);

        buffer = malloc((sizeof(char*)*2));
        buffer = ((char**)data);

        sprintf(htmlLine,"<li><a href=doc.html#%s>%s</a></li>",buffer[1],buffer[0]);
        fwrite(htmlLine,sizeof(char),strlen(htmlLine),index);

    }while(straight_list_move(docu->slist,straight_list_next)!=FALSE);

    fwrite(HTML_INDEX_FOOTER,sizeof(char),strlen(HTML_INDEX_FOOTER),index);/*agrego footer del indice*/

    fclose(index);

    return RES_OK;
}

int destroyDoc(TDA_Doc **docu)
{
	ClearList(&((*docu)->listado));
	free(*docu);
	return RES_OK;
}
