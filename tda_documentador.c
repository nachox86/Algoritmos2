#include <string.h>
#include "tda_documentador.h"
#include "logger_tda.h"
#include "htmlParser.h"
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
#define RES_MEM_ERROR  -1

/*******************************************************************************************************************/
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

int straight_list_copy_keyword(void* dst, const void* src)
{
    t_keyword* dst_aux;
    t_keyword* src_aux = (t_keyword*) src;

    dst_aux = (t_keyword*)malloc(sizeof(t_keyword));
    if(!dst_aux)
        return RES_MEM_ERROR;

    dst_aux->name = (char*)malloc(sizeof(char)*strlen(src_aux->name)+1);
    if(!dst_aux->name)
    {
        free(dst_aux);
        return RES_MEM_ERROR;
    }
    dst_aux->tag = (char*)malloc(sizeof(char)*strlen(src_aux->tag)+1);
    if(!dst_aux->tag)
    {
        free(dst_aux->name);
        free(dst_aux);
        return RES_MEM_ERROR;
    }
    dst_aux->value = (char*)malloc(sizeof(char)*strlen(src_aux->value)+1);
    if(!dst_aux->value)
    {
        free(dst_aux->name);
        free(dst_aux->tag);
        free(dst_aux);
        return RES_MEM_ERROR;
    }
    strcpy(dst_aux->name, src_aux->name);
    strcpy(dst_aux->tag, src_aux->tag);
    strcpy(dst_aux->value, src_aux->value);
    dst = (void*)dst_aux;
    return RES_OK;
}

void straight_list_delete_keyword(void* data)
{
    t_keyword* data_aux = (t_keyword*) data;
    free(data_aux->name);
    free(data_aux->tag);
    free(data_aux->value);
    free(data);
}

void straight_list_delete_listado(void* data)
{
    straight_list_destroy((straight_list_t*)data);
    free(data);
}

int straight_list_copy_listado(void* dst, const void* src)
{
    straight_list_t* aux;
    straight_list_t* src_aux = (straight_list_t*) src;
    void* data_aux;

    if(!straight_list_is_empty(src))
        return RES_OK;

    aux = (straight_list_t*)malloc(sizeof(straight_list_t));
    if(!aux)
        return RES_MEM_ERROR;

    if(straight_list_create(aux, sizeof(t_keyword), straight_list_copy_keyword, straight_list_delete_keyword)!=RES_OK)
    {
        free(aux);
        return RES_ERROR;
    }

    straight_list_move(src_aux,straight_list_first);
    straight_list_get(src_aux,data_aux);
    straight_list_insert(aux,straight_list_first,data_aux);

    while(!straight_list_move(src_aux,straight_list_next))
    {
        straight_list_get(src_aux,data_aux);
        straight_list_insert(aux,straight_list_next,data_aux);
    }

    dst = (void*)aux;

    return RES_OK;
}

int set_keyword(t_keyword* kw,char* data)
{
    char* tag;
    char* name_or_value;
    char* name;
    char* value;
    int size_nov = 2; /*size of name_or_value*/

    tag = strtok(data," ");
    name_or_value = strtok(NULL,"");
    if(name_or_value!=NULL)
    {
        name = strtok(name_or_value," ");
        if(name!=NULL)
            value = strtok(NULL,"");
    }

    kw->tag = (char*)malloc(sizeof(char)*strlen(tag)+1); /*pido memoria para el tag*/
    if(!kw->tag)
        return RES_MEM_ERROR;

    if((strcmp(tag,KW_FUNCTION)==0||strcmp(tag,KW_PARAM)==0||strcmp(tag,KW_RETURN)==0))
    {
        kw->name = (char*)malloc(sizeof(char)*strlen(name)+1);
        if(!kw->name)
        {
            free(kw->tag);
            return RES_MEM_ERROR;
        }
        if (value)
        kw->value = (char*)malloc(sizeof(char)*strlen(value)+1);
        if(!kw->value)
        {
            free(kw->name);
            free(kw->tag);
            return RES_MEM_ERROR;
        }
        strcpy(kw->name,name);
        strcpy(kw->value,value);
    }
    else
    {
        kw->name = (char*)malloc(sizeof(char)*2);
        if(!kw->name)
        {
            free(kw->tag);
            return RES_MEM_ERROR;
        }
        if(name_or_value!=NULL)
            size_nov = strlen(name_or_value)+1;
        kw->value = (char*)malloc(sizeof(char)*size_nov);
        if(!kw->value)
        {
            free(kw->name);
            free(kw->tag);
            return RES_MEM_ERROR;
        }
        strcpy(kw->name,"\0\0");
        if(name_or_value!=NULL)
            strcpy(kw->value,name_or_value);
        else
            strcpy(kw->value,"\0\0");
    }
    return RES_OK;
}

/******************************************************************************************************************/
int search_site(straight_list_t *lp, const void* data, straight_list_movement_t *mov) {
	void *current_data;
	t_keyword* cdata;
	t_keyword* rdata;

	if(straight_list_is_empty(lp))
		return FALSE;

	straight_list_get(lp,current_data);

	cdata = (t_keyword*)current_data;
	rdata = (t_keyword*)data;

   	if (strcmp( cdata->name, rdata->name) > 0)
	{
		straight_list_move(lp,straight_list_first);
		straight_list_get(lp,current_data);
	}
    while(strcmp(rdata->name,cdata->name)>0 && straight_list_move(lp,straight_list_next))
	{
		straight_list_get(lp,current_data);
		cdata = (t_keyword*)current_data;
	}
	if(strcmp(rdata->name,cdata->name)<0)
	{
		*mov = straight_list_first;
	}
	else
		*mov = straight_list_next;

	return TRUE;
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
int straight_list_order_insert(straight_list_t *lp, void* data)
{
    straight_list_movement_t mov = straight_list_first;
    int insert;
	int search = search_site(lp,data,&mov);
    if(search!=RES_OK) return RES_ERROR;
	insert = straight_list_insert(lp,mov,data);
    if(insert!=RES_OK) return RES_ERROR;
	return insert;
}
/***************************************************************************************************************/

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
       /* straight_list_create(docu->listado,sizeof(straight_list_t), straight_list_copy_listado, straight_list_delete_listado);
        straight_list_move(docu->listado,straight_list_first);*/

        createHtmlFile(html, outputFile);
        for (i = 0; i < n; i++) {
            extractDocumentationFromFile(docu, html, buffer[i]);
            free(buffer[i]);
        }
        free(buffer);
        closeHtmlFile(html);
        return RES_OK;
    } else {
        return RES_ERROR;
    }
}



int extractDocumentationFromFile(TDA_Doc *docu, htmlFile *html, char *iFile) {
    FILE *inputFile;
    straight_list_t* listado_aux;
    t_keyword* keyword;
    int cinit = 0;
    int cend = 0;
    char linea[MAX_LINE];
    char *stoken, *etoken;

    logInfo(docu->logFile,"Abro archivo de entrada.");
    inputFile = fopen(iFile, "r");
    if (!inputFile) {
        logError(docu->logFile,MSG_ERROR_IN_FILE);
        return RES_ERROR;
    }

    logInfo(docu->logFile,"Creo (pido recursos) las listas listado e indice del documentador.");
    if(straight_list_create(docu->listado, sizeof(straight_list_t), straight_list_copy_listado, straight_list_delete_listado)!=RES_OK) {
        logError(docu->logFile,MSG_ERROR_CREATE_LIST);
        fclose(inputFile);
        return RES_ERROR;
    }
    if(straight_list_create(docu->indice, sizeof(t_keyword), straight_list_copy_keyword, straight_list_delete_keyword)!=RES_OK) {
        logError(docu->logFile,MSG_ERROR_CREATE_LIST);
        straight_list_delete(docu->listado);
        fclose(inputFile);
        return RES_ERROR;
    }

    logInfo(docu->logFile,"Pido recursos para lista auxiliar.");
    listado_aux = (straight_list_t*)malloc(sizeof(straight_list_t));
    if(!listado_aux) {
        logError(docu->logFile,MSG_ERROR_CREATE_LIST);
        straight_list_delete(docu->indice);
        straight_list_delete(docu->listado);
        fclose(inputFile);
        return RES_ERROR;
    }

    logInfo(docu->logFile,"Pido recursos para t_keyword auxiliar.");
    keyword = (t_keyword*)malloc(sizeof(t_keyword));
    if(!keyword) {
        logError(docu->logFile,MSG_ERROR_CREATE_LIST);
        /*straight_list_delete(listado_aux);*/
        straight_list_delete(docu->indice);
        straight_list_delete(docu->listado);
        free(listado_aux);
        fclose(inputFile);
        return RES_ERROR;
    }

    logInfo(docu->logFile,"Empiezo a recorrer el archivo.");
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
                logInfo(docu->logFile,"Terminó un comentario.");
            }

            if (cinit && !cend)
            {
                /* mientras que esté dentro del comentario */
                if (checkForKW(linea) == 0)
                {
                /* encontramos una keyword */
                    set_keyword(keyword,linea);
                /* guardo en la lista auxiliar */
                    if(!straight_list_is_empty(listado_aux))
                    {
                        if(!straight_list_insert(listado_aux, straight_list_next,keyword)) {
                            logError(docu->logFile,MSG_ERROR_MEMORY);
                            straight_list_destroy(listado_aux);
                            free(listado_aux);
                            straight_list_destroy(docu->listado);
                            straight_list_destroy(docu->indice);
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
                        if(!straight_list_insert(listado_aux, straight_list_first,keyword)) {
                            logError(docu->logFile,MSG_ERROR_MEMORY);
                            straight_list_destroy(listado_aux);
                            free(listado_aux);
                            straight_list_destroy(docu->listado);
                            straight_list_destroy(docu->indice);
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
                logInfo(docu->logFile,"Termino el comentario de la función, ahora inserto en la lista docu->listado de manera ordenada las funciones.");
                /* en la función de búsqueda habrá que tener en cuenta qué tomar para la comparación */
                if(!straight_list_order_insert(docu->listado,listado_aux)) {
                    /* tuvo problemas de memoria para insertar */
                    logError(docu->logFile,MSG_ERROR_MEMORY);
                    straight_list_destroy(listado_aux);
                    free(listado_aux);
                    straight_list_destroy(docu->listado);
                    straight_list_destroy(docu->indice);
                    free(keyword->name);
                    free(keyword->tag);
                    free(keyword->value);
                    free(keyword);
                    fclose(inputFile);
                    return RES_ERROR;
                }
                logInfo(docu->logFile,"Mando a insertar en el archivo html de salida.");
                parseStringToHtml(html,*keyword); /*lo mando al html*/
                /* reinicializo las variables auxiliares*/
                logInfo(docu->logFile,"Reinicializo las variables auxiliares.");
                cinit = 0; cend = 0;
                /* como ya inserté los tags/keywords de una funcion, destruto la lista auxiliar */
                straight_list_destroy(listado_aux);
                free(listado_aux);
                /* y pido recursos nuevamente para los comentarios de la siguiente función */
                logInfo(docu->logFile,"Recupero recursos de la lista auxiliar y del keyword auxiliar, y los solicito para la próxima.");
                listado_aux = (straight_list_t*)malloc(sizeof(straight_list_t));
                if(!listado_aux) {
                    logError(docu->logFile,MSG_ERROR_CREATE_LIST);
                    straight_list_delete(docu->indice);
                    straight_list_delete(docu->listado);
                    free(keyword->name);
                    free(keyword->tag);
                    free(keyword->value);
                    free(keyword);
                    fclose(inputFile);
                    return RES_ERROR;
                }
                keyword = (t_keyword*)malloc(sizeof(t_keyword));
                if(!keyword) {
                    logError(docu->logFile,MSG_ERROR_CREATE_LIST);
                    free(listado_aux);
                    straight_list_delete(docu->indice);
                    straight_list_delete(docu->listado);
                    fclose(inputFile);
                    return RES_ERROR;
                }
            }
            else if (!cinit && cend)
            {
                /* está mal hecho algún comentario, libero los recursos y salgo */
                logError(docu->logFile,MSG_ERROR_BAD_COMMENT);
                straight_list_delete(listado_aux);
                free(listado_aux);
                straight_list_delete(docu->indice);
                straight_list_delete(docu->listado);
                free(keyword->name);
                free(keyword->tag);
                free(keyword->value);
                free(keyword);
                fclose(inputFile);
                return RES_ERROR;
            }
        }
    }

    /* libero los recursos de listado_aux y cierro el archivo de entrada ya que no lo necesito más */
    logInfo(docu->logFile,"Libero los recursos de listado_aux y cierro el archivo de entra ya que no lo necesito más.");
    straight_list_delete(listado_aux);
    free(listado_aux);
    fclose(inputFile);

    /* ahora armo la lista del indice ordenada*/
    logInfo(docu->logFile,"Empiezo a armar la lista del indice pero ordenada.");
    straight_list_move(docu->listado,straight_list_first);

    do {
        logInfo(docu->logFile,"Tomo un elemento lista del documentador y lo guardo en listado_aux.");
        listado_aux = (straight_list_t*)malloc(sizeof(straight_list_t));
        if(!listado_aux)
        {
            logError(docu->logFile,MSG_ERROR_MEMORY);
            straight_list_destroy(docu->listado);
            straight_list_destroy(docu->indice);
            return RES_MEM_ERROR;
        }
        straight_list_get(docu->listado,listado_aux);

        /* recorro el elemento lista*/
        straight_list_move(listado_aux,straight_list_first);
        do{
            keyword = (t_keyword*)malloc(sizeof(t_keyword));
            if(!keyword) {
                logError(docu->logFile,MSG_ERROR_MEMORY);
                straight_list_destroy(listado_aux);
                straight_list_destroy(docu->listado);
                straight_list_destroy(docu->indice);
                free(listado_aux);
                return RES_MEM_ERROR;
            }
            straight_list_get(listado_aux,keyword);
            if(strcmp(keyword->tag,KW_FUNCTION)==0)
            {
                /* encontré el tag de function */
                /* inserto en la lista del indice de docu */
                logInfo(docu->logFile,"Encontré un tag de función y lo inserto en el docu->indice.");
                if(!straight_list_order_insert(docu->indice,keyword)) {
                    /* tuvo problemas de memoria para insertar */
                    logError(docu->logFile,MSG_ERROR_MEMORY);
                    straight_list_destroy(listado_aux);
                    straight_list_destroy(docu->listado);
                    straight_list_destroy(docu->indice);
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
    t_keyword* keyword;

    /*keyword = (t_keyword*)malloc(sizeof(t_keyword*));
    if(!keyword)
    {
        logError(docu->logFile,MSG_ERROR_MEMORY);
        return RES_ERROR;
    }*/

    /*doy formato y agrego al archivo de indices*/
    index = fopen(indexFile,"w");
    if(!index)
    {
        /*free(keyword);*/
        logError(docu->logFile,MSG_ERROR_INDEX_FILE);
        return RES_ERROR;
    }

    fwrite(HTML_INDEX_HEADER,sizeof(char),strlen(HTML_INDEX_HEADER),index); /*agrego header del indice*/

    /*Me muevo por la lista del indice desde el principio*/

    straight_list_move(docu->indice,straight_list_first);

    do{
        straight_list_get(docu->indice,keyword);

        sprintf(htmlLine,"<li><a href=doc.html#%s>%s</a></li>",keyword->name,keyword->name);
        fwrite(htmlLine,sizeof(char),strlen(htmlLine),index);

        free(keyword->name);
        free(keyword->tag);
        free(keyword->value);
        free(keyword);
        /*keyword = (t_keyword*)malloc(sizeof(t_keyword));
        if(!keyword)
        {
            logError(docu->logFile,MSG_ERROR_MEMORY);
            fclose(index);
            return RES_ERROR;
        }*/

    }while(straight_list_move(docu->indice,straight_list_next)!=FALSE);

    fwrite(HTML_INDEX_FOOTER,sizeof(char),strlen(HTML_INDEX_FOOTER),index);/*agrego footer del indice*/

    fclose(index);

    return RES_OK;
}

int destroyDoc(TDA_Doc **docu)
{
	free(*docu);
	return RES_OK;
}
