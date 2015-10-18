#ifndef FUNCTION_TOOLS_H_INCLUDED
#define FUNCTION_TOOLS_H_INCLUDED

#include "list_tda.h"
#include "tda_documentador.h"

#define RES_OK 0
#define RES_ERROR 1
#define RES_HELP 2
#define RES_NOT_ENOUGH_ARGS 3
#define RES_WRONG_ARGS 4
#define RES_CORRECT_ARGS 5

#define ARGS_INDEX ->

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

#define INDEX_PREFFIX ".idx.html"
#define INDEX_PREFFIX_NO_EXT "idx."
#define HTML_EXT ".html"

void showHelp();

int checkForKW(char* linea);

int countFunctions(T_List* listed);

int getCommentsCount(char** dato);

int slistCopy(void* dst, const void* src);

void sListDestroy(void* elem);

#endif
