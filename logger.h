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

int createLog(Logger **log, char *path);

int Log(Logger *log, char *level, char *msg);

int loge(Logger *log, char *msg);

int logi(Logger *log, char *msg);

int getLogPath(Logger *log, char **buffer);

int closeLog(Logger **log);

#endif
