#ifndef EMBSTRIN_LOGGING_H
#define EMBSTRIN_LOGGING_H

#include <stdint.h>
#include <stdbool.h>

// ---- Log levels ----
typedef enum {
    EMBSTRIN_LOG_DEBUG = 0,
    EMBSTRIN_LOG_INFO  = 1,
    EMBSTRIN_LOG_WARN  = 2,
    EMBSTRIN_LOG_ERROR = 3
} EMBSTRIN_LogLevel;

// ---- Logger config ----
typedef struct {
    EMBSTRIN_LogLevel min_level;
    uint32_t          flush_freq_hz;
} EMBSTRIN_LogConfig;

// ---- Log entry ----
typedef struct {
    EMBSTRIN_LogLevel level;
    uint64_t          host_time;
    uint64_t          device_time;
    uint32_t          device_id;
    char              message[256];
} EMBSTRIN_LogEntry;

// ---- Functions ----
void EMBSTRIN_LOG_init(EMBSTRIN_LogConfig* config);
void EMBSTRIN_LOG_set_level(EMBSTRIN_LogLevel level);
void EMBSTRIN_LOG_host(EMBSTRIN_LogLevel level, const char* message);
void EMBSTRIN_LOG_device(uint32_t device_id, uint64_t device_time,
                          EMBSTRIN_LogLevel level, const char* message);
void EMBSTRIN_LOG_flush(void);

#endif  
