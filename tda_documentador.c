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
    t_keyword* dst_aux = (t_keyword*) dst;
    t_keyword* src_aux = (t_keyword*) src;

    memcpy(dst, src, (sizeof(t_keyword) * MAX_KW));

    return RES_OK;
}
void straight_list_delete_keyword(void* data)
{
    t_keyword* data_aux = (t_keyword*) data;
    free(data_aux->name);
    free(data_aux->tag);
    free(data_aux->value);
    /* free(data); */
}

void straight_list_delete_listado(void* data)
{
    straight_list_destroy((straight_list_t*)data);
    free(data);
}

int straight_list_copy_listado(void* dst, const void* src)
{
    straight_list_t* aux = (straight_list_t*) dst;
    straight_list_t* src_aux = (straight_list_t*) src;
    void* data_aux;

    if(!straight_list_is_empty(src))
        return RES_OK;

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
    char* tag = NULL;
    char* name = NULL;
    char* value = NULL;

    tag = strtok(data," ");
    if (strcmp(tag, KW_PARAM) == 0) {
        name = strtok(NULL, " ");
        value = strtok(NULL, "");
    } else {
        name = strtok(NULL, "");
    }

    if (tag) {
        kw->tag = (char*)malloc(sizeof(char)*strlen(tag)+1); /*pido memoria para el tag*/
        strcpy(kw->tag, tag);
    }
    if (name) {
        kw->name = malloc(sizeof(char)*strlen(name)+1);
        strcpy(kw->name, name);
    }
    if (value) {
        kw->value = malloc(sizeof(char)*strlen(value)+1);
        strcpy(kw->value, value);
    }
    return 1;

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
    htmlFile *html = malloc(sizeof(htmlFile));
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
    t_keyword kw[MAX_KW];
    int cinit = 0, cend = 0, kwi = 0, i;
    char linea[MAX_LINE];
    char *stoken = NULL, *etoken = NULL;
    t_keyword temp[MAX_KW];

    for (i = 0; i < MAX_KW; i++) {
        kw[i].name = NULL;
        kw[i].tag = NULL;
        kw[i].value = NULL;
    }

    inputFile = fopen(iFile, "r");
    straight_list_create(docu->listado, (sizeof(t_keyword) * MAX_KW), straight_list_copy_keyword, straight_list_delete_keyword);

    while (!feof(inputFile)) {
        if (fgets(linea, MAX_LINE-1, inputFile) != NULL) {
            stoken = strstr(linea, KW_INIT);
            etoken = strstr(linea, KW_END);

            if (stoken)
                cinit = 1;
            if (etoken)
                cend = 1;

            if ((cinit) && (!cend)) {
                if (checkForKW(linea) == 0) {
                    set_keyword(&kw[kwi], linea);
                    kwi++;
                }
            } else if (cend) {
                if (kwi > 0) {
                    if (straight_list_is_empty(docu->listado))
                        straight_list_insert(docu->listado, straight_list_first, kw);
                    else
                        straight_list_insert(docu->listado, straight_list_next, kw);
                    cinit = cend = 0;
                    for (i = 0; i <= kwi; i++) {
                        kw[i].name = NULL;
                        kw[i].tag = NULL;
                        kw[i].value = NULL;
                    }
                    kwi = 0;
                }
            }
        }
    }

    if (!straight_list_is_empty(docu->listado)) {
        straight_list_move(docu->listado, straight_list_first);

        do {
            straight_list_get(docu->listado, &temp);
            i = 0;
            while ((temp[i].tag) && (checkForKW(temp[i].tag) == 0)) {
                parseStringToHtml(html, temp[i]);
                i++;
            }
        } while (straight_list_move(docu->listado, straight_list_next) != 0);
    }
    straight_list_clear(docu->listado);
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
