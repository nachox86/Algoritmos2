#include <string.h>
#include "tda_documentador.h"
#include "logger-tda.h"
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
#define MSG_ERROR_CREATE_LIST "Ocurrió un error al crear una lista."
#define MSG_ERROR_BAD_COMMENT "Se detectó un comentario mal escrito."
/* TODO: Check this declaration (done in situ for debug purposes */
#define RES_MEM_ERROR   -1

typedef struct {
    char* tag;
    char* name;
    char* value;
} t_keyword;

int extractDocumentationFromFile(TDA_Doc *docu, htmlFile *html, char *iFile);

int createDoc(TDA_Doc **docu, Logger *log)
{
	(*docu) = malloc(sizeof(TDA_Doc));
	if(!(*docu))
    {
        return RES_MEM_ERROR;
    }
    (*docu)->indice = (straight_list_t*)malloc(sizeof(straight_list_t));
    if(!((*docu)->indice))
    {
        free(*docu);
        return RES_MEM_ERROR;
    }
    (*docu)->listado = (straight_list_t*)malloc(sizeof(straight_list_t));
    if(!((*docu)->listado))
    {
        free((*docu)->indice);
        free((*docu));
        return RES_MEM_ERROR;
    }
    (*docu)->inputFile = NULL;
    (*docu)->outputFile = NULL;
    (*docu)->logFile = log;

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


int set_keyword(t_keyword* kw,char* data)
{
    /* tengo que parsear */
    int ldata = strlen(data);
    int ltag;
    int lname;
    int lvalue;
    int i;
    char* token;
    char* switched;
    char* switched2;
/* TODO solo llenar name si es el key de function*/
    token = (char*)malloc(ldata*sizeof(char)+1);
    if(!token)
        return RES_MEM_ERROR;

    strcpy(token,data);
    ltag = strlen(strstr(token," ")); /*el tag es la primer palabra*/
    lvalue = ldata - ltag; /* lvalue abarca lo que hay después del tag*/
    kw->tag = (char*)malloc(sizeof(char)*ltag+1);
    if(!kw->tag)
    {
        free(token);
        return RES_MEM_ERROR;
    }
    memcpy(kw->tag,token,ltag);
    kw->tag[ltag] = '\0';

    switched = (char*)malloc(ldata*sizeof(char)+1);
    if(!switched)
    {
        free(kw->tag);
        free(token);
        return RES_MEM_ERROR;
    }

    for(i=0;i<ldata;i++)
        switched[i] = token[ldata-i-1];

    switched[ldata] = '\0'

    switched2 = (char*)malloc(ldata*sizeof(char)+1);
    if(!switched2)
    {
        free(switched);
        free(kw->tag);
        free(token);
        return RES_MEM_ERROR;
    }

    memcpy(switched2,switched,lvalue);
    kw->tag[lvalue] = '\0';

    free(token);
    token = (char*)malloc(lvalue*sizeof(char)+1);
    if(!token)
    {
        free(switched2);
        free(switched);
        free(kw->tag);
        return RES_MEM_ERROR;
    }

    ldata = strlen(switched2);
    for(i=0;i<ldata;i++)
        token[i] = switched2[ldata-i-1];

    token[ldata] = '\0';

    lname = strlen(strstr(token," "));
    lvalue = ldata - lname;
    kw->name = (char*)malloc(sizeof(char)*lname+1);
    if(!kw->name)
    {
        free(token);
        free(switched2);
        free(switched);
        free(kw->tag);
        return RES_MEM_ERROR;
    }
    memcpy(kw->name,token,lname);
    kw->name[lname] = '\0';

    kw->value = (char*)malloc(sizeof(char)*lvalue+1);
    if(!kw->value)
    {
        free(token);
        free(switched2);
        free(switched);
        free(kw->name);
        free(kw->tag);
        return RES_MEM_ERROR;
    }
    for(i=0;i<lvalue;i++)
        kw->value[i] = token[lname+i];
    kw->value[lvalue] = '\0';

    free(token);
    free(switched2);
    free(switched);
}

int extractDocumentationFromFile(TDA_Doc *docu, htmlFile *html, char *iFile) {
    FILE *inputFile;
    int i = 0, j = 0;
    int n = 0;
    int count = 0;
    int cinit = 0;
    int cend = 0;
    int lenElem = 0;

    char linea[MAX_LINE];
    char *stoken, *etoken;

    char **index;

    straight_list_t* listado_aux;
    t_keyword* keyword;

    docu = (TDA_Doc*)malloc(sizeof(TDA_Doc));
    if(!docu)
    {
        logError(docu->log,MSG_ERROR_MEMORY);
        fclose(inputFile);
        return RES_MEM_ERROR;
    }

    logInfo(docu->log,"Abro archivo de entrada.");
    inputFile = fopen(iFile, "r");
    if (!inputFile)
    {
        logError(docu->log,MSG_ERROR_IN_FILE);
        return RES_ERROR;
    }

    logInfo(docu->log,"Creo (pido recursos) las listas listado e indice del documentador.");
    if(straight_list_create(docu->listado, sizeof(straight_list_t), copy_elem_listado, destroy_elem_listado)!=RES_OK)
    {
        logError(docu->log,MSG_ERROR_CREATE_LIST);
        free(docu);
        fclose(inputFile);
        return RES_ERROR;
    }
    if(straight_list_create(docu->indice, sizeof(t_keyword), copy_elem_indice, destroy_elem_indice)!=RES_OK)
    {
        logError(docu->log,MSG_ERROR_CREATE_LIST);
        straight_list_delete(docu->listado);
        free(docu);
        fclose(inputFile);
        return RES_ERROR;
    }

    logInfo(docu->log,"Pido recursos para lista auxiliar.");
    listado_aux = (straight_list_t*)malloc(sizeof(straight_list_t));
    if(!listado_aux)
    {
        logError(docu->log,MSG_ERROR_CREATE_LIST);
        straight_list_delete(docu->indice);
        straight_list_delete(docu->listado);
        free(docu);
        fclose(inputFile);
        return RES_ERROR;
    }

    logInfo(docu->log,"Pido recursos para t_keyword auxiliar.");
    keyword = (t_keyword*)malloc(sizeof(t_keyword));
    if(!keyword)
    {
        logError(docu->log,MSG_ERROR_CREATE_LIST);
        straight_list_delete(listado_aux);
        straight_list_delete(docu->indice);
        straight_list_delete(docu->listado);
        free(listado_aux);
        free(docu);
        fclose(inputFile);
        return RES_ERROR;
    }

    logInfo(docu->log,"Empiezo a recorrer el archivo.");
    while (!feof(inputFile)) {
        /* TODO: get line dinamico */
        if (fgets(linea, MAX_LINE-1, inputFile) != NULL) {
            stoken = strstr(linea, KW_INIT);
            etoken = strstr(linea, KW_END);

            if ((stoken) && (!etoken)) {
            /* empieza un comentario */
                cinit = 1;
            } else if ((!stoken) && (etoken)) {
            /* termina un comentario */
                cend = 1;
                logInfo(docu->log,"Terminó un comentario.");
            }

            if (cinit && !cend)
            {
                /* mientras que esté dentro del comentario */
                if (checkForKW(linea) == 0)
                {
                /* encontramos una keyword */
                    /* TODO set keyword <- linea */
                /* guardo en la lista auxiliar */
                    if(!straight_list_is_empty(listado_aux))
                    {
                        if(!straight_list_insert(listado_aux, straight_list_next,keyword))
                        {
                            logError(docu->log,MSG_ERROR_MEMORY);
                            straight_list_destroy(listado_aux);
                            free(listado_aux);
                            straight_list_destroy(docu->listado);
                            straight_list_destroy(docu->indice);
                            free(docu);
                            free(keyword->name);
                            free(keyword->tag);
                            free(keyword->value);
                            free(keyword);
                            fclose(inputFile);
                            return RES_ERROR;
                        }
                    }
                    else
                    {
                        if(!straight_list_insert(listado_aux, straight_list_first,keyword))
                        {
                            logError(docu->log,MSG_ERROR_MEMORY);
                            straight_list_destroy(listado_aux);
                            free(listado_aux);
                            straight_list_destroy(docu->listado);
                            straight_list_destroy(docu->indice);
                            free(docu);
                            free(keyword->name);
                            free(keyword->tag);
                            free(keyword->value);
                            free(keyword);
                            fclose(inputFile);
                            return RES_ERROR;
                        }
                    }
                }
            }
            else if (cinit && cend)
            {
                logInfo(docu->log,"Termino el comentario de la función, ahora inserto en la lista docu->listado de manera ordenada las funciones.");
                /* en la función de búsqueda habrá que tener en cuenta qué tomar para la comparación */
                if(!iListOrderInsert(docu->listado,listado_aux))
                {
                    /* tuvo problemas de memoria para insertar */
                    logError(docu->log,MSG_ERROR_MEMORY);
                    straight_list_destroy(listado_aux);
                    straight_list_destroy(docu->listado);
                    straight_list_destroy(docu->indice);
                    free(docu);
                    free(keyword->name);
                    free(keyword->tag);
                    free(keyword->value);
                    free(keyword);
                    fclose(inputFile);
                    return RES_ERROR;
                }
                /* reinicializo las variables auxiliares*/
                logInfo(docu->log,"Reinicializo las variables auxiliares.");
                cinit = 0;
                cend = 0;
                cfound = 0;
                /* como ya inserté los tags/keywords de una funcion, destruto la lista auxiliar */
                straight_list_destroy(listado_aux);
                /* y pido recursos nuevamente para los comentarios de la siguiente función */
                logInfo(docu->log,"Recupero recursos de la lista auxiliar y del keyword auxiliar, y los solicito para la próxima.");
                listado_aux = (straight_list_t*)malloc(sizeof(straight_list_t));
                if(!listado_aux)
                {
                    logError(docu->log,MSG_ERROR_CREATE_LIST);
                    straight_list_delete(docu->indice);
                    straight_list_delete(docu->listado);
                    free(docu);
                    free(keyword->name);
                    free(keyword->tag);
                    free(keyword->value);
                    free(keyword);
                    fclose(inputFile);
                    return RES_ERROR;
                }
                keyword = (t_keyword*)malloc(sizeof(t_keyword));
                if(!keyword)
                {
                    logError(docu->log,MSG_ERROR_CREATE_LIST);
                    free(listado_aux);
                    straight_list_delete(docu->indice);
                    straight_list_delete(docu->listado);
                    free(docu);
                    fclose(inputFile);
                    return RES_ERROR;
                }
            }
            else if (!cinit && cend)
            {
                /* está mal hecho algún comentario, libero los recursos y salgo */
                logError(docu->log,MSG_ERROR_BAD_COMMENT);
                straight_list_delete(listado_aux);
                free(listado_aux);
                straight_list_delete(docu->indice);
                straight_list_delete(docu->listado);
                free(docu);
                free(keyword->name);
                free(keyword->tag);
                free(keyword->value);
                free(keyword);
                fclose(inputFile);
                return RES_ERROR;
            }
        }
    }

    /* libero los recursos de listado_aux y cierro el archivo de entra ya que no lo necesito más */
    logInfo(docu->log,"Libero los recursos de listado_aux y cierro el archivo de entra ya que no lo necesito más.");
    straight_list_delete(listado_aux);
    fclose(inputFile);

    /* ahora armo la lista del indice ordenada*/
    logInfo(docu->log,"Empiezo a armar la lista del indice pero ordenada.");
    straight_list_move(docu->listado,straight_list_first);

    do {
        logInfo(docu->log,"Tomo un elemento lista del documentador y lo guardo en listado_aux.");
        listado_aux = (straight_list_t*)malloc(sizeof(straight_list_t));
        if(!listado_aux)
        {
            logError(docu->log,MSG_ERROR_MEMORY);
            straight_list_destroy(docu->listado);
            straight_list_destroy(docu->indice);
            free(docu);
            return RES_MEM_ERROR;
        }
        straight_list_get(docu->listado,listado_aux);

        /* recorro el elemento lista*/
        straight_list_move(listado_aux,straight_list_first);
        do{
            keyword = (t_keyword*)malloc(sizeof(t_keyword));
            if(!keyword)
            {
                logError(docu->log,MSG_ERROR_MEMORY);
                straight_list_destroy(listado_aux);
                straight_list_destroy(docu->listado);
                straight_list_destroy(docu->indice);
                free(docu);
                free(listado_aux);
                return RES_MEM_ERROR;
            }
            straight_list_get(listado_aux,keyword);
            if(strcmp(keyword->tag,KW_FUNCTION)==0)
            {
                /* encontré el tag de function */
                /* inserto en la lista del indice de docu */
                logInfo(docu->log,"Encontré un tag de función y lo inserto en el docu->indice.");
                if(!iListOrderInsert(docu->indice,keyword))
                {
                    /* tuvo problemas de memoria para insertar */
                    logError(docu->log,MSG_ERROR_MEMORY);
                    straight_list_destroy(listado_aux);
                    straight_list_destroy(docu->listado);
                    straight_list_destroy(docu->indice);
                    free(docu);
                    free(listado_aux);
                    free(keyword->name);
                    free(keyword->tag);
                    free(keyword->value);
                    free(keyword);
                    fclose(inputFile);
                    return RES_MEM_ERROR;
                }
                /* TODO destroy t_keyword */
                free(keyword->name);
                free(keyword->tag);
                free(keyword->value);
                free(keyword);
            }
        } while (straight_list_move(listado_aux,straight_list_next)!=FALSE);
        straight_list_destroy(listado_aux);
    } while (straight_list_move(docu->listado,straight_list_next)!=FALSE);


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
        logError(docu->logFile,MSG_ERROR_INDEX_FILE);
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
