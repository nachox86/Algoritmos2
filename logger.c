#include "logger.h"

int createLog(Logger log, char *path) {
    log.file = fopen(path, "a+t");
    log.open = log.file == NULL ? LOG_CLOSED : LOG_OPEN;
    return log.open;
}

int Log(Logger log, char *level, char *msg) {
    if (log.open) {
        fprintf(log.file, "%s %s\n", level, msg);
        return 1;
    }
    return 0;
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
    return 1;
}
