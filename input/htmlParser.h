#ifndef HTMLPARSER_H_INCLUDED
#define HTMLPARSER_H_INCLUDED

#include <stdio.h>
#include "tda_documentador.h"

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
    char *path;
    FILE *file;
} htmlFile;

/*
    @funcion createHtmlFile
    @descr Inicializa y crea el archivo html donde guardara la documentacion
    @param output htmlFile a inicializar
    @param path direccion del archivo donde guardar la documentacion
    @author mike
    @return SUCCESS o ERROR, dependiendo del caso
*/

int createHtmlFile(htmlFile *output, char *path);

/*
    @funcion parseStringToHtml
    @descr Convierte el string del comentario a formato HTML y lo guarda en el archivo previamente creado
    @author mike
    @param output htmlFile donde guardar el comentario
    @param line comentario a procesar y guardar en el archivo HTML
    @pre output debe haber sido creado en inicializado previamente.
    @pre line debe ser el comentario con el formato establecido en el enunciado del TP. i.e: "@KeyWord arg1 [arg2] ...".
    @return En caso de error devuelve ERROR (0), SUCCESS (1) en caso contrario
*/

int parseStringToHtml(htmlFile *output, t_keyword comment);

/*
    @funcion closeHtmlFile
    @descr Cierra y finaliza la escritura del archivo HTML donde se guardaron los comentarios
    @param output htmlFile a cerrar
    @pre output debe haber sido previamente creado
    @return En caso de error devuelve ERROR (0), SUCCESS (1) en caso contrario
*/

int closeHtmlFile(htmlFile *output);


#endif
