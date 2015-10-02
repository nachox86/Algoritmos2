#ifndef HTMLPARSER_H_INCLUDED
#define HTMLPARSER_H_INCLUDED

#include <stdio.h>

#define SUCCESS     1
#define ERROR       0

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
#define KW_POST         "@pos"

typedef struct {
    int init;
    FILE *file;
} htmlFile;

int createHtmlFile(htmlFile **output, char *path);

int parseStringToHtml(htmlFile *output, char *line);

int closeHtmlFile(htmlFile **output);


#endif
