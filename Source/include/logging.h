#ifndef LOGGING_H /* Header guard to prevent multiple inclusion */
#define LOGGING_H

#include "pd_api.h"

/* Extern declaration for the PlaydateAPI singleton */
extern PlaydateAPI* pd;

/* Logging macros */
#define LOG_INFO(format, ...) \
    pd->system->logToConsole(playdate, "[INFO] %s: " format, __func__, ##__VA_ARGS__)

#define LOG_WARNING(format, ...) \
    pd->system->logToConsole(playdate, "[WARNING] %s: " format, __func__, ##__VA_ARGS__)

#define LOG_ERROR(format, ...) \
    pd->system->logToConsole(playdate, "[ERROR] %s: " format, __func__, ##__VA_ARGS__)

#endif /* LOGGING_H */