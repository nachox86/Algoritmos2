#include "logger.h"

int createLog(Logger log, char *path) {
    log.file = fopen(path, "a+t");
    if (!log.file) {
        free(log.file);
        log.open = LOG_CLOSED;
        return ERR_FILE_NOT_FOUND;
    } else {
        log.open = LOG_OPEN;
    }
    return SUCCESS;
}

int Log(Logger log, char *level, char *msg) {
    if (log.open) {
        fprintf(log.file, "%s %s\n", level, msg);
        return SUCCESS;
    }
    return ERROR;
}

int loge(Logger log, char *msg) {
    return Log(log, MSG_ERROR, msg);
}

int logi(Logger log, char *msg) {
    return Log(log, MSG_INFO, msg);
}

int closeLog(Logger log) {
    fclose(log.file);
    log.open = LOG_CLOSED;
    return SUCCESS;
}
