#include <stdio.h>
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
#define MSG_ERROR_CREATE_LIST "Ocurrió un error al crear una lista."
#define MSG_ERROR_BAD_COMMENT "Se detectó un comentario mal escrito."
#define RES_ERROR 1
#define RES_MEM_ERROR  -1

typedef struct {
    char* tag;
    char* name;
    char* value;
} t_keyword;

/*Funca*/
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
/*Funca*/
int straight_list_copy_keyword(void* dst, const void* src)
{
    t_keyword* dst_aux = (t_keyword*) dst;
    t_keyword* src_aux = (t_keyword*) src;

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
    dst = (void*) dst_aux;
    return RES_OK;
}
/*Funca*/
void straight_list_delete_keyword(void* data)
{
    t_keyword* data_aux = (t_keyword*) data;
    free(data_aux->name);
    free(data_aux->tag);
    free(data_aux->value);
    /*free(data);*/
}

void straight_list_delete_listado(void* data)
{
    straight_list_destroy((straight_list_t*)data);
    /*free(data);*/
}

int straight_list_copy_listado(void* dst, const void* src)
{
    straight_list_t* aux;
    straight_list_t* src_aux;
    void* data_aux;

    if(!straight_list_is_empty(src))
        return RES_OK;

    printf("entre\n");
    src_aux = (straight_list_t*) src;

    data_aux = malloc(sizeof(t_keyword));
    if(!data_aux) return RES_MEM_ERROR;
    /*aux = (straight_list_t*)malloc(sizeof(straight_list_t));
    if(!aux)
        return RES_MEM_ERROR;*/

    /*if(straight_list_create(aux, sizeof(t_keyword), straight_list_copy_keyword, straight_list_delete_keyword)!=RES_OK)
    {
        free(aux);
        return RES_ERROR;
    }*/

    aux = (straight_list_t*)dst;

    straight_list_move(src_aux,straight_list_first);
    straight_list_get(src_aux,data_aux);
    straight_list_insert(aux,straight_list_first,data_aux);

    while(!straight_list_move(src_aux,straight_list_next))
    {
        straight_list_get(src_aux,data_aux);
        straight_list_insert(aux,straight_list_next,data_aux);
    }

    /*dst = (void*)aux;*/

    return RES_OK;
}

/*FUNCA*/
int set_keyword(t_keyword* kw,char* data)
{
    char* tag;
    char* name_or_value;
    char* name;
    char* value;
    int size_nov = 2; /*size of name_or_value*/

    tag = strtok(data," ");
    printf("%s\n",tag);
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
    strcpy(kw->tag,tag);

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
        kw->name = (char*)malloc(sizeof(char)*size_nov);
        if(!kw->name)
        {
            free(kw->tag);
            return RES_MEM_ERROR;
        }
        strcpy(kw->name,"\0\0");
        if(name_or_value!=NULL)
            size_nov = strlen(name_or_value)+1;
        kw->value = (char*)malloc(sizeof(char)*size_nov);
        if(!kw->value)
        {
            free(kw->name);
            free(kw->tag);
            return RES_MEM_ERROR;
        }
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


int main(int argc,char** argv){

/****************funciones de keyword*****************************/
 /*   t_keyword key1;
    t_keyword* key2;
    int set;

    key1.name = (char*)malloc(sizeof(char)*20);
    key1.name = "nombre_de_funcion";

    key1.tag = (char*)malloc(sizeof(char)*20);
    key1.tag = "nombre_de_tag";

    key1.value = (char*)malloc(sizeof(char)*20);
    key1.value = "nombre_de_value";

    key2=(t_keyword*)malloc(sizeof(t_keyword));

    set = straight_list_copy_keyword(key2,&key1);

    if(set==RES_OK){
        printf("key1 name: %s\nkey1 tag: %s\nkey1 value: %s\n",key1.name,key1.tag,key1.value);
        printf("key2 name: %s\nkey2 tag: %s\nkey2 value: %s\n",key2->name,key2->tag,key2->value);
    }

    straight_list_delete_keyword(key2);*/


/******************funciones de lista********************/

    straight_list_t* lista;
    straight_list_t* copia;
    t_keyword* key1;
    t_keyword* key2;
    t_keyword* key3;
    t_keyword* keyaux;
    char linea1[50] = "@function funcion1";
    char linea2[50] = "@descr esta es la primer funcion";
    char linea3[50] = "@author ignacio";

    keyaux = (t_keyword*)malloc(sizeof(t_keyword));
    if(!keyaux)
    {
        printf("ERROR.\n");
        return RES_MEM_ERROR;
    }

    key1 = (t_keyword*)malloc(sizeof(t_keyword));
    if(!key1)
    {
        free(keyaux);
        printf("ERROR.\n");
        return RES_MEM_ERROR;
    }

    key2 = (t_keyword*)malloc(sizeof(t_keyword));
    if(!key2)
    {
        free(key1);
        free(keyaux);
        printf("ERROR.\n");
        return RES_MEM_ERROR;
    }

    key3 = (t_keyword*)malloc(sizeof(t_keyword));
    if(!key3){
        free(key2);
        free(key1);
        free(keyaux);
        printf("ERROR.\n");
        return RES_MEM_ERROR;
    }

    lista=(straight_list_t*)malloc(sizeof(straight_list_t));
    if(!lista){
        free(key3);
        free(key2);
        free(key1);
        printf("ERROR.\n");
        free(keyaux);
        return RES_MEM_ERROR;
    }

    copia=(straight_list_t*)malloc(sizeof(straight_list_t));
    if(!copia){
        free(lista);
        free(key3);
        free(key2);
        free(key1);
        printf("ERROR.\n");
        free(keyaux);
        return RES_MEM_ERROR;
    }

    printf("Creo las listas.\n");
    straight_list_create(lista, sizeof(t_keyword), straight_list_copy_keyword, straight_list_delete_keyword);

    printf("Inicializo los structs.\n");
    set_keyword(key1,linea1);
    set_keyword(key2,linea2);
    set_keyword(key3,linea3);

    printf("key1 name: %s\nkey1 tag: %s\nkey1 value: %s\n",key1->name,key1->tag,key1->value);
    printf("key2 name: %s\nkey2 tag: %s\nkey2 value: %s\n",key2->name,key2->tag,key2->value);
    printf("key3 name: %s\nkey3 tag: %s\nkey3 value: %s\n",key3->name,key3->tag,key3->value);

    straight_list_insert(lista,straight_list_first,key1);
    straight_list_insert(lista,straight_list_next,key2);
    straight_list_insert(lista,straight_list_next,key3);

    printf("Ya inserté en la lista original.\n");

    straight_list_move(lista,straight_list_first);
    do{
        straight_list_get(lista,keyaux);
        printf("keyaux name: %s\nkeyaux tag: %s\nkeyaux value: %s\n",keyaux->name,keyaux->tag,keyaux->value);
    }while(straight_list_move(lista,straight_list_next));

    printf("ahora copia la lista\n");

    straight_list_create(copia, sizeof(t_keyword), straight_list_copy_keyword, straight_list_delete_keyword);
    straight_list_copy_listado(copia,lista);

    straight_list_move(copia,straight_list_first);
    straight_list_get(copia,keyaux);

    printf("%s\n",keyaux->tag);

    printf("ahora me fijo el contenido de la copia\n");
    do{
        straight_list_get(copia,keyaux);
        printf("keyaux name: %s\nkeyaux tag: %s\nkeyaux value: %s\n",keyaux->name,keyaux->tag,keyaux->value);
    }while(straight_list_move(copia,straight_list_next));

    printf("ahora se elimina la lista copia");
    straight_list_delete_listado(copia);
    printf("se elimina la lista original");
    straight_list_delete_listado(&lista);

    return RES_OK;
}
