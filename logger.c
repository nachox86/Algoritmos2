#include "logger.h"
#include <string.h>
#include <time.h>

int createLog(Logger **log, char *path) {
    time_t ltime = time(NULL);

    (*log) = malloc(sizeof(Logger));

    (*log)->file = fopen(path, "a");

    if (!(*log)->file) {
        free(*log);
        return ERR_FILE_NOT_FOUND;
    } else {
        fprintf((*log)->file, "%s\n", asctime(localtime(&ltime)));
        (*log)->open = LOG_OPEN;
    }
    return SUCCESS;
}

int Log(Logger *log, char *level, char *msg) {
    if (log->open) {
        fprintf(log->file, "%s %s\n", level, msg);
        return SUCCESS;
    }
    return ERROR;
}

int loge(Logger *log, char *msg) {
    return Log(log, MSG_ERROR, msg);
}

int logi(Logger *log, char *msg) {
    return Log(log, MSG_INFO, msg);
}

int getLogPath(Logger *log, char **buffer) {
    if (log->open) {
        (*buffer) = malloc(sizeof(char) * strlen(log->path) + 1);
        strcpy(*buffer, log->path);
        return SUCCESS;
    }
    return ERROR;
}

int closeLog(Logger **log) {
    fclose((*log)->file);
    free(*log);
    return SUCCESS;
}
