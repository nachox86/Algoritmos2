#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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
#define RES_OK 0
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
/*FUNCA*/
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
/****************funciones de keyword*****************************/
int main(int argc,char** argv){
    t_keyword* key1;
    char linea[50] = "sdf sdsd sdfs ";
    int set;

    key1=(t_keyword*)malloc(sizeof(t_keyword));
    if(!key1) return RES_MEM_ERROR;

    set = set_keyword(key1,linea);

    if(set==RES_OK){
        printf("key1 name: %s\nkey1 tag: %s\nkey1 value: %s\n",key1->name,key1->tag,key1->value);
    } else printf("No se pudo setear.\n");

    free(key1->name);
    free(key1->tag);
    free(key1->value);
    free(key1);
    return RES_OK;
}
