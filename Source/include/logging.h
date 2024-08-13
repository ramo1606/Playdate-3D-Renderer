#ifndef LOGGING_H /* Header guard to prevent multiple inclusion */
#define LOGGING_H

#include "pd_api.h"
#include "global.h"

/* Logging macros */
#define LOG_INFO(format, ...) \
    pd->system->logToConsole("[INFO] %s: " format, __func__, ##__VA_ARGS__)

#define LOG_WARNING(format, ...) \
    pd->system->logToConsole("[WARNING] %s: " format, __func__, ##__VA_ARGS__)

#define LOG_ERROR(format, ...) \
    pd->system->logToConsole("[ERROR] %s: " format, __func__, ##__VA_ARGS__)

#endif /* LOGGING_H */