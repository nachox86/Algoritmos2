#ifndef LOGGER_H_INCLUDED
#define LOGGER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

#define LOG_OPEN    1
#define LOG_CLOSED  0

#define ERROR   0
#define SUCCESS 1

#define ERR_FILE_NOT_FOUND  -1

#define MSG_ERROR   "Error: "
#define MSG_INFO    "Info: "

typedef struct {
    int open;
    FILE *file;
    char *path;
} Logger;

/*
    createLogger: Crea e inicializa el Logger
    PARAMS:     log: logger a ser inicializado
                path: directorio donde crear el log
    En caso de error devuelve ERROR (0), SUCCESS (1) en caso contrario.
*/

int createLog(Logger **log, char *path);

/*
    loge: Escribe errores en el log
    PARAMS:     log: logger en el cual escribir el error
                msg: mensaje a guardar
    PRE:        log debe haber sido inicializado previamente
    En caso de error devuelve ERROR (0), SUCCESS (1) en caso contrario.
*/

int loge(Logger *log, char *msg);

/*
    logi: Escribe informacion en el log
    PARAMS:     log: logger en el cual escribir la informacion
                msg: mensaje a guardar
    PRE:        log debe haber sido inicializado previamente
    En caso de error devuelve ERROR (0), SUCCESS (1) en caso contrario.
*/

int logi(Logger *log, char *msg);

/*
    getLogPath: Guarda el path del logger en el buffer
    PARAMS:     log: logger del cual obtener la direccion
                buffer: buffer en el cual guardar la direccion
    PRE:        log debe haber sido inicializado previamente
    En caso de error devuelve ERROR (0), SUCCESS (1) en caso contrario.
*/

int getLogPath(Logger *log, char **buffer);

/*
    closeLog: Cierra y destruye el logger
    PARAMS:     log: logger a destruir
    PRE:        log debe haber sido inicializado previamente
    En caso de error devuelve ERROR (0), SUCCESS (1) en caso contrario.
*/

int closeLog(Logger **log);

#endif
