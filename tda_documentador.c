#include "tda_documentador.h"
#include "logger.h"
#include "list_tda.h"
#include "tda_nodo.h"
#include "htmlParser.h"
#include "functions_tools.h"
#include <string.h>
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
    (*docu)->slist = malloc(sizeof(straight_list_t));;
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


int extractDocumentationFromFile(TDA_Doc *docu, htmlFile *html, char *iFile)
{
    FILE* inputFile;
    int commentsInit = 0;
    int commentsEnd = 0;
    int commentsFound = 0; /* 0 significa que no encontró, 1 que si*/
    int count = 0;
    int i,j;
    int lenElem = 0;
    char **comms;
    char **temp = NULL;

    inputFile = fopen(iFile,"r");
    if(!inputFile)
    {
        loge(docu->logFile,MSG_ERROR_IN_FILE);
        return RES_ERROR;
    }

    /* asigno recursos para la variable auxiliar */
    comms = (char **) malloc(sizeof(char*)*MAX_KW);
    if(!comms)
    {
        loge(docu->logFile,MSG_ERROR_MEMORY);
        return RES_MEM_ERROR;
    }

    CreateList(&(docu->listado),sizeof(char**));

    /*El archivo se abrió correctamente por lo que ahora paso a recopilar la información*/
    /*Recorro línea por línea*/

    while(!feof(inputFile))
    {
        char linea[MAX_LINE];
        char *token;
        char *token2;

        if(fgets(linea,MAX_LINE,inputFile)!=NULL)
        {
            token = strstr(linea,KW_INIT);
            token2 = strstr(linea,KW_END);
            if(token && !token2)
            {
                /*Encontramos el inicio de los comentarios de la función*/
                commentsInit = 1;
            }
            else if(!token && token2)
            {
                commentsEnd = 1;
                if(commentsInit == 1)
                    commentsFound = 1;
            }
                /* else es un comentario trivial */

            if(commentsFound==1)
            /* a esta altura ya se cerraron los comentarios para una función y los guardé en comms */
            {
                if(count==0)
                {
                    free(comms);
                }
                else
                {
                    /*inserto los comentarios en la lista*/
                    if(EmptyList(docu->listado))
                    {
                        /* está vacía la lista, entonces debe ser el primero*/
                        InsertE(&(docu->listado),M_First,comms);
                    }
                    else
                    {
                        InsertE(&(docu->listado),M_Next,comms);
                    }

                    /* reinicializo las variabls auxiliares locales */
                    /* libero los recursos de comms */
                    /* Not the time, not the place.. */
                    /*
                    for(j=count;j>=0;--j)
                    {
                        free(comms[j]);
                    }
                    */
                    free(comms);
                }
                commentsInit = 0;
                commentsEnd = 0;
                commentsFound = 0;
                count = 0;
                /* asigno recursos para la variable auxiliar */
                comms = (char **) malloc(sizeof(char*)*MAX_KW);
                if(!comms)
                {
                    loge(docu->logFile,MSG_ERROR_MEMORY);
                    return RES_MEM_ERROR;
                }
            }
            else if((commentsInit == 1) && (commentsEnd == 0))
            {
            	if (checkForKW(linea) == 0) {
            		/* Found new KeyWord */
		            comms[count] = (char *) malloc(sizeof(char)*strlen(linea)+1);
		            if(!comms[count])
		            {
		                loge(docu->logFile,MSG_ERROR_MEMORY);
		                /*tengo que liberar los recursos*/
		                if(count==0)
		                {
		                    free(comms);
		                }
		                else
		                {
		                    for(j=count;j>=0;--j)
		                    {
		                        free(comms[j]);
		                    }
		                    free(comms);
		                }
		                return RES_MEM_ERROR;
		            }
		            strcpy(comms[count], linea);
		            count++;
           		} else {
           			/* Assume it's part of the last keyword, separated by \n */
           			if (count > 0) {
		       			count--;
		       			comms[count] = realloc(comms[count], strlen(comms[count]) + strlen(linea) + 1);
		       			strcat(comms[count], linea);
		       			count++;
		       		}
           		}
            }
        }

    }

    free(comms);
    fclose(inputFile);

    /*Ahora lo pasamos al archivo en formato HTML*/
    /*vamos a recorrer la lista, nodo por nodo, tomando cada uno de los comentarios e insertándolos en el archivo de salida*/

    /*Muevo al primero el corriente*/
    MoveC(&(docu->listado),M_First);

    do
    {
        GetC(docu->listado,comms);
        lenElem = strlen(*comms);
        for( i = 0; i < lenElem; i++)
        {
            if((comms[i] == NULL) || (!parseStringToHtml(html,comms[i])))
            {
                loge(docu->logFile,MSG_ERROR_OUT_FILE);
                return RES_ERROR;
            }

            if (strcmp(strtok(comms[i], " "), KW_FUNCTION) == 0) {
                if (!temp)
                    temp = malloc(sizeof(char**)*2);

                temp[0] = realloc(temp[0], strlen(comms[i]));
                temp[1] = realloc(temp[1], strlen(iFile));

                strcpy(temp[0], strtok(NULL, ""));
                strcpy(temp[1], iFile);

                straight_list_order_insert(docu->slist, temp);
            }
        }
    } while(MoveC(&(docu->listado),M_Next)!=FALSE);

    /* Now we free every element of comms */
	for(j = (sizeof(comms)/sizeof(comms[0])); j >= 0; j--) {
        free(comms[j]);
    }
    free(comms);

    if (temp) {
        free(temp[0]);
        free(temp[1]);
        free(temp);
    }

    ClearList((&docu->listado));

    return RES_OK;
}


int createIndex(TDA_Doc *docu, char *indexFile)
{
    FILE* index;
    char htmlLine[MAX_LINE];
    char** parm;

    /*doy formato y agrego al archivo de indices*/
    index = fopen(indexFile,"wa");
    if(!index)
    {
        loge(docu->logFile,MSG_ERROR_INDEX_FILE);
        return RES_ERROR;
    }

    fwrite(HTML_INDEX_HEADER,sizeof(char),strlen(HTML_INDEX_HEADER),index); /*agrego header del indice*/

    /*Me muevo por la lista del indice desde el principio*/
    straight_list_move(docu->slist,straight_list_first);

    do{
        straight_list_get(docu->slist,parm);

        sprintf(htmlLine,"<li><a href=doc.html#%s>%s</a></li>",parm[0],parm[0]);
        fwrite(htmlLine,sizeof(char),strlen(htmlLine),index);

        free(parm[1]);
        free(parm[0]);

    }while(straight_list_move(docu->slist,straight_list_next)!=FALSE);

    fwrite(HTML_INDEX_FOOTER,sizeof(char),strlen(HTML_INDEX_FOOTER),index);/*agrego footer del indice*/

    free(parm);
    fclose(index);

    return RES_OK;
}

int destroyDoc(TDA_Doc **docu)
{
	ClearList(&((*docu)->listado));
	straight_list_delete(&((*docu)->slist));
	free(*docu);
	return RES_OK;
}
