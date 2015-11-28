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
/*
@funcion checkForKW
@descr Esta funcion valida si en una cadena de texto existe uno de los tags que identifican a los keywords.
@autor Ignacio
@fecha 14/10/2015
@version "1.0"
@param linea cadena de texto a validar
@pre ninguna
@pos devuelve RES_OK si encuentra uno de los keywords en la cadena
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
@funcion straight_list_copy_keyword
@descr Hace la copia de una estructura de tipo t_keyword.
@autor Ignacio
@fecha 14/11/2015
@version "1.0"
@param src referencia del dato que se quiere copiar
@param dst referencia en dónde se quiere hacer la copia
@pre src debe estar inicializado y dst debe tener recursos asignados
@pos realiza la copia del dato en src en la memoria asosciada a dst. Devuelve RES_OK si pudo hacer la copia; RES_ERROR si src es NULL y RES_MEM_ERROR si no pudo alocar memoria para la copia.
*/
int straight_list_copy_keyword(void* dst, const void* src)
{
    t_keyword* dst_aux = (t_keyword*) dst;
    t_keyword* src_aux = (t_keyword*) src;

    if(!src_aux) return RES_ERROR;

    if(src_aux->name)
    {
        dst_aux->name = (char*)malloc(sizeof(char)*strlen(src_aux->name)+1);
        if(!dst_aux->name)
        {
            return RES_MEM_ERROR;
        }
    }

    dst_aux->tag = (char*)malloc(sizeof(char)*strlen(src_aux->tag)+1);
    if(!dst_aux->tag)
    {
        free(dst_aux->name);
        return RES_MEM_ERROR;
    }

    if(src_aux->value)
    {
        dst_aux->value = (char*)malloc(sizeof(char)*strlen(src_aux->value)+1);
        if(!dst_aux->value)
        {
            free(dst_aux->name);
            free(dst_aux->tag);
            return RES_MEM_ERROR;
        }
    }

    if(src_aux->name) strcpy(dst_aux->name, src_aux->name);
    else dst_aux->name = NULL;

    strcpy(dst_aux->tag, src_aux->tag);

    if(src_aux->value) strcpy(dst_aux->value, src_aux->value);
    else dst_aux->value = NULL;

    dst = (void*) dst_aux;
    return RES_OK;
}
/*
@funcion straight_list_delete_keyword
@descr Libera los recursos de un dato de tipo t_keyword.
@autor Ignacio
@fecha 14/11/2015
@version "1.0"
@param data referencia al dato alocado
@pre data no debe ser NULL y debe estar inicializado.
@pos libera la memoria asociada al name, tag y value de la estructura.
*/
void straight_list_delete_keyword(void* data)
{
    t_keyword* data_aux = (t_keyword*) data;
    free(data_aux->name);
    free(data_aux->tag);
    free(data_aux->value);
}
/*
@funcion straight_list_delete_listado
@descr Libera los recursos de un dato straight_list.
@autor Ignacio
@fecha 14/11/2015
@version "1.0"
@param data referencia a la lista
@pre data debe estar creada.
@pos libera los recursos de la lista y esta queda destruida.
*/
void straight_list_delete_listado(void* data)
{
    straight_list_t* aux = (straight_list_t*)data;
    straight_list_destroy(aux);
}
/*
@funcion straight_list_copy_listado
@descr Copia una lista.
@autor Ignacio
@fecha 14/11/2015
@version "1.0"
@param dst referencia de en dónde se quiere la copia de la lista
@param src lista original a copiar
@pre la lista src debe existir y dst debe tener recursos asignados.
@pos copia toda lista src a dst.
*/
int straight_list_copy_listado(void* dst, const void* src)
{
    straight_list_t* aux;
    straight_list_t* src_aux = (straight_list_t*)src;
    void* data_aux;

    printf("                   entre a la funcion de copia de lista\n");

    if(straight_list_is_empty(src_aux))
        return RES_ERROR;

    data_aux = malloc(sizeof(t_keyword));
    if(!data_aux) return RES_MEM_ERROR;

    aux = (straight_list_t*)dst;

    if(straight_list_move(src_aux,straight_list_first)==TRUE) printf("movi el corriente al principio.\n");

    straight_list_get(src_aux,data_aux);
    printf("keyaux data aux tag: %s\n",(((t_keyword*)data_aux)->tag));
    straight_list_insert(aux,straight_list_first,data_aux);

    straight_list_delete_keyword(data_aux);

    while(straight_list_move(src_aux,straight_list_next)!=FALSE)
    {
        straight_list_get(src_aux,data_aux);
        printf("keyaux data aux tag: %s\n",(((t_keyword*)data_aux)->tag));
        straight_list_insert(aux,straight_list_next,data_aux);
        straight_list_delete_keyword(data_aux);
    }

    free(data_aux);

    return RES_OK;
}
/*
@funcion set_keyword
@descr Iniciliza un dato de tipo t_keyword.
@autor Ignacio
@fecha 14/11/2015
@version "1.0"
@param kw referencia a una estructura t_keyword
@param data cadena de texto con los datos para cargar la estructura
@pre kw creada.
@pos inicializa los campos de laestructura con la información en data.
*/
int set_keyword(t_keyword* kw,char* data)
{
    char* tag;
    char* name;
    char* value;
    char* token;
    int i = 0;

    if(checkForKW(data)==RES_OK)
    {
        token = strtok(data," ");
        tag = token;

        while(token!=NULL)
        {
            if(strcmp(tag,KW_FUNCTION)==0)
            {
                token = strtok(NULL,"");
                name = token;
                value = '\0';
                break;
            }
            else if(strcmp(tag,KW_RETURN)==0||strcmp(tag,KW_PARAM)==0)
            {
                if(i==0)
                {
                    token = strtok(NULL," ");
                    name = token;
                    i++;
                }
                if(i==1)
                {
                    token = strtok(NULL,"");
                    value = token;
                    break;
                }
            }
            else
            {
                token = strtok(NULL,"");
                value = token;
                name = '\0';
                break;
            }
        }
    }
    else
        return RES_ERROR;

    kw->tag = (char*)malloc(sizeof(char)*strlen(tag)+1);
    if(!kw->tag)
        return RES_MEM_ERROR;
    strcpy(kw->tag,tag);
    kw->tag[strlen(kw->tag)] = '\0';

    printf("tag %s\n",kw->tag);

    if(name)
    {
        kw->name = (char*)malloc(sizeof(char)*strlen(name)+1);
        if(!kw->name)
        {
            free(kw->tag);
            return RES_MEM_ERROR;
        }
        strcpy(kw->name,name);
        name[strlen(name)] = '\0';
        printf("name %s\n",kw->name);
    }
    else
        kw->name = NULL;

    if(value)
    {
        kw->value = (char*)malloc(sizeof(char)*strlen(value)+1);
        if(!kw->value)
        {
            free(kw->name);
            free(kw->tag);
            return RES_MEM_ERROR;
        }
        strcpy(kw->value,value);
        value[strlen(value)] = '\0';
        printf("value %s\n",kw->value);
    }
    else
        kw->value = NULL;

    return RES_OK;
}
/******************************************************************************************************************/
/*
@funcion search_site
@descr Busca dentro de una lista el lugar en dónde debería ir el dato data.
@autor Ignacio
@fecha 14/11/2015
@version "1.0"
@param lp referencia a la lista en donde se busca.
@param data dato de búsqueda
@param *mov movimiento dentro de una lista
@pre lp creada y data creada.
@pos devuelve en *mov el movimiento que tiene que hacer lp.
*/
int search_site(straight_list_t *lp, const void* data, straight_list_movement_t *mov) {

	void *current_data;
	t_keyword* cdata;
	t_keyword* rdata;
	if(straight_list_is_empty(lp))
    {
        *mov = straight_list_first;
        return RES_OK;
    }
    current_data = malloc(sizeof(t_keyword));
    if(!current_data) return RES_MEM_ERROR;

	straight_list_get(lp,current_data);

	cdata = (t_keyword*)current_data;
	rdata = (t_keyword*)data;

   	if (strcmp( cdata->name, rdata->name) > 0)
	{
		straight_list_move(lp,straight_list_first);
		straight_list_delete_keyword(current_data);
		straight_list_get(lp,current_data);
	}
    while(strcmp(rdata->name,cdata->name)>0 && straight_list_move(lp,straight_list_next))
	{
  		straight_list_delete_keyword(current_data);
		straight_list_get(lp,current_data);
		cdata = (t_keyword*)current_data;
	}
	if(strcmp(rdata->name,cdata->name)<0)
    {
        *mov = straight_list_previous;
    }
	else
    {
		*mov = straight_list_next;
	}
	return RES_OK;
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
	int search;

    search = search_site(lp,data,&mov);
    if(search!=RES_OK) return RES_ERROR;
	insert = straight_list_insert(lp,mov,data);
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


/*
@funcion extractDocumentationFromFile
@descr Lee el archivo de entrada y va almacenando en la estructura la información de los comentarios puestos en este. Agrega en el archivo de salida la información.
@autor Ignacio
@fecha 14/11/2015
@version "1.0"
@param docu referencia al documentador
@param html archivo de salida de datos
@param iFile archivo de entrada de datos
@pre docu creado.
@pos Recorre iFile linea por linea. Cuando identifica que encontró un comentario, agrega en el archivo de salida la información de cada comentario. Crea dos listas, una con la información y otra de indice. Devuelve RES_OK si pudo operar sin problemas y RES_MEM_ERROR si tuvo problemas de memoria. RES_ERROR en el caso de que no pudo abrir alguno de los archivos.
*/
int extractDocumentationFromFile(TDA_Doc *docu, htmlFile *html, char *iFile) {
    FILE *inputFile;
    straight_list_t* listado_aux;
    t_keyword* keyword;
    int cinit = 0;
    int cend = 0;
    int got_key = 0;
    char linea[MAX_LINE];
    char *stoken, *etoken;
    straight_list_movement_t mov = straight_list_first;
    straight_list_movement_t mov_aux = straight_list_first;

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
                /* armo el listado auxiliar*/
                if (checkForKW(linea) == 0)
                {
                /* encontramos una keyword */
                    set_keyword(keyword,linea);
                /* guardo en la lista auxiliar */
                    if(!straight_list_is_empty(listado_aux))
                        mov_aux = straight_list_next;
                    else
                        mov_aux = straight_list_first;

                    if(!straight_list_insert(listado_aux, mov_aux,keyword)) {
                        logError(docu->logFile,MSG_ERROR_MEMORY);
                        straight_list_destroy(listado_aux);
                        straight_list_destroy(docu->listado);
                        straight_list_destroy(docu->indice);
                        straight_list_delete_keyword(keyword);
                        free(listado_aux);
                        free(keyword);
                        fclose(inputFile);
                        return RES_ERROR;
                    }
                    straight_list_delete_keyword(keyword);

                }
            }
            else if (cinit && cend)
            {
                logInfo(docu->logFile,"Termino el comentario de la función, ahora inserto en la lista docu->listado de manera ordenada las funciones.");
                /* en la función de búsqueda habrá que tener en cuenta qué tomar para la comparación */
                if(!straight_list_is_empty(docu->listado))
                    mov = straight_list_next;
                else
                    mov = straight_list_first;
                if(!straight_list_insert(docu->listado,mov,listado_aux)) {
                    /* tuvo problemas de memoria para insertar */
                    logError(docu->logFile,MSG_ERROR_MEMORY);
                    straight_list_destroy(listado_aux);
                    straight_list_destroy(docu->listado);
                    straight_list_destroy(docu->indice);
                    straight_list_delete_keyword(keyword);
                    free(listado_aux);
                    free(keyword);
                    fclose(inputFile);
                    return RES_ERROR;
                }
                logInfo(docu->logFile,"Mando a insertar en el archivo html de salida.");

                /*TODO un get del struct con el nombre para pasar al parser*/

                parseStringToHtml(html,*keyword); /*lo mando al html*/
                /* reinicializo las variables auxiliares*/
                logInfo(docu->logFile,"Reinicializo las variables auxiliares.");
                cinit = 0; cend = 0;
                /* como ya inserté los tags/keywords de una funcion, destruto la lista auxiliar */
                straight_list_destroy(listado_aux);
            }
            else if ((!cinit && cend)||(cinit && stoken)) {
                /* está mal hecho algún comentario, libero los recursos y salgo */
                logError(docu->logFile,MSG_ERROR_BAD_COMMENT);
                logError(docu->logFile,stoken);
                straight_list_delete(listado_aux);
                straight_list_delete(docu->indice);
                straight_list_delete(docu->listado);
                straight_list_delete_keyword(keyword);
                free(listado_aux);
                free(keyword);
                fclose(inputFile);
                return RES_ERROR;
            }
        }
    }

    /* libero los recursos de listado_aux y cierro el archivo de entrada ya que no lo necesito más */
    logInfo(docu->logFile,"Libero los recursos de listado_aux y cierro el archivo de entra ya que no lo necesito más.");
    straight_list_delete(listado_aux);
    straight_list_delete_keyword(keyword);
    fclose(inputFile);

    /* ahora armo la lista del indice ordenada*/
    logInfo(docu->logFile,"Empiezo a armar la lista del indice pero ordenada.");
    straight_list_move(docu->listado,straight_list_first);

    do {
        logInfo(docu->logFile,"Tomo un elemento lista del documentador y lo guardo en listado_aux.");
        straight_list_get(docu->listado,listado_aux);
        /* recorro el elemento lista*/
        straight_list_move(listado_aux,straight_list_first);
        do{
            straight_list_get(listado_aux,keyword);
            if(strcmp(keyword->tag,KW_FUNCTION)==0)
            {
                /* encontré el tag de funcion */
                /* inserto en la lista del indice de docu */
                logInfo(docu->logFile,"Encontré un tag de función y lo inserto en el docu->indice.");
                if(!straight_list_order_insert(docu->indice,keyword)) {
                    /* tuvo problemas de memoria para insertar */
                    logError(docu->logFile,MSG_ERROR_MEMORY);
                    straight_list_destroy(listado_aux);
                    straight_list_destroy(docu->listado);
                    straight_list_destroy(docu->indice);
                    straight_list_delete_keyword(keyword);
                    free(listado_aux);
                    free(keyword);
                    fclose(inputFile);
                    return RES_MEM_ERROR;
                }
                got_key = 1;
            }
            straight_list_delete_keyword(keyword);
        } while (straight_list_move(listado_aux,straight_list_next)!=FALSE||got_key==0);
        got_key = 0;
        straight_list_destroy(listado_aux);
    } while (straight_list_move(docu->listado,straight_list_next)!=FALSE);

    free(keyword);
    free(listado_aux);

    return RES_OK;
}

int createIndex(TDA_Doc *docu, char *indexFile)
{
    FILE* index;
    char htmlLine[MAX_LINE];
    t_keyword* keyword;

    keyword = (t_keyword*)malloc(sizeof(t_keyword));
    if(!keyword)
    {
        logError(docu->logFile,MSG_ERROR_MEMORY);
        return RES_ERROR;
    }
    index = fopen(indexFile,"w");
    if(!index)
    {
        free(keyword);
        logError(docu->logFile,MSG_ERROR_INDEX_FILE);
        return RES_ERROR;
    }
    logInfo(docu->logFile,"Abro el archivo indice.");
    fwrite(HTML_INDEX_HEADER,sizeof(char),strlen(HTML_INDEX_HEADER),index);
    logInfo(docu->logFile,"Agrego header del indice.");
    logInfo(docu->logFile,"Empiezo a recorrer la lista de indice.");

    straight_list_move(docu->indice,straight_list_first);
    do{
        straight_list_get(docu->indice,keyword);
        sprintf(htmlLine,"<li><a href=doc.html#%s>%s</a></li>",keyword->name,keyword->name);
        fwrite(htmlLine,sizeof(char),strlen(htmlLine),index);
        straight_list_delete_keyword(keyword);
    }while(straight_list_move(docu->indice,straight_list_next)!=FALSE);
    logInfo(docu->logFile,"Termino de recorrer la lista indice.");
    fwrite(HTML_INDEX_FOOTER,sizeof(char),strlen(HTML_INDEX_FOOTER),index);
    logInfo(docu->logFile,"Agrego el footer al archivo indice.");
    free(keyword);
    fclose(index);

    return RES_OK;
}

int destroyDoc(TDA_Doc **docu)
{
	free(*docu);
	return RES_OK;
}
