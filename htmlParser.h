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

/*
    createHtmlFile: Inicializa y crea el archivo html donde guardara la documentacion
    PARAMS:     output: htmlFile a inicializar
                path: direccion del archivo donde guardar la documentacion
    En caso de error devuelve ERROR (0), SUCCESS (1) en caso contrario
*/

int createHtmlFile(htmlFile **output, char *path);

/*
    parseStringToHtml: Convierte el string del comentario a formato HTML y lo guarda en el archivo previamente creado
    PARAMS:     output: htmlFile donde guardar el comentario
                line: comentario a procesar y guardar en el archivo HTML
    PRE:        output debe haber sido creado en inicializado previamente.
                line debe ser el comentario con el formato establecido en el enunciado del TP. i.e: "@KeyWord arg1 [arg2] ...".
    En caso de error devuelve ERROR (0), SUCCESS (1) en caso contrario
*/

int parseStringToHtml(htmlFile *output, char *line);

/*
    closeHtmlFile: Cierra y finaliza la escritura del archivo HTML donde se guardaron los comentarios
    PARAMS:     output: htmlFile a cerrar
    PRE:        output debe haber sido previamente creado
    En caso de error devuelve ERROR (0), SUCCESS (1) en caso contrario
*/

int closeHtmlFile(htmlFile **output);


#endif
