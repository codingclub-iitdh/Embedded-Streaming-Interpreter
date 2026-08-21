#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>
#include "embstrin_logging.h"


static EMBSTRIN_LogConfig g_config = {
    .min_level = EMBSTRIN_LOG_INFO,
    .flush_freq_hz = 10};

static const char* level_to_string(EMBSTRIN_LogLevel level)
{
    switch (level) {
        case EMBSTRIN_LOG_DEBUG: return "DEBUG";
        case EMBSTRIN_LOG_INFO:  return "INFO ";
        case EMBSTRIN_LOG_WARN:  return "WARN ";
        case EMBSTRIN_LOG_ERROR: return "ERROR";
        default:                 return "UNKNOWN";
    }
}

static uint64_t get_host_time_ms(void)
{
    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);
    uint64_t time = ((uint64_t)ft.dwHighDateTime << 32) | ft.dwLowDateTime;
    // convert from 100-nanosecond intervals to milliseconds
    return time / 10000;
}


void EMBSTRIN_LOG_init(EMBSTRIN_LogConfig *config)
{
    if (!config)
    {
        return;
    }
    g_config = *config;
}

void EMBSTRIN_LOG_set_level(EMBSTRIN_LogLevel level)
{
    g_config.min_level = level;
}

void EMBSTRIN_LOG_host(EMBSTRIN_LogLevel level, const char *message)
{
    if (level < g_config.min_level)
        return;

    uint64_t host_time = get_host_time_ms();
    uint64_t ms = host_time % 1000;
    uint64_t secs = (host_time / 1000) % 60;
    uint64_t mins = (host_time / 60000) % 60;
    uint64_t hrs = (host_time / 3600000) % 24;

    // format: [HOST | HH:MM:SS.mmm] LEVEL - message
    printf("[HOST | %02llu:%02llu:%02llu.%03llu] %s - %s\n", hrs, mins, secs, ms, level_to_string(level), message);
}
void EMBSTRIN_LOG_device(uint32_t device_id, uint64_t device_time,
                          EMBSTRIN_LogLevel level, const char* message)
{
    if (level < g_config.min_level)
        return;

    uint64_t host_time = get_host_time_ms();
    uint64_t h_ms   = host_time % 1000;
    uint64_t h_secs = (host_time / 1000) % 60;
    uint64_t h_mins = (host_time / 60000) % 60;
    uint64_t h_hrs  = (host_time / 3600000) % 24;

    
    uint64_t d_ms   = device_time % 1000;
    uint64_t d_secs = (device_time / 1000) % 60;
    uint64_t d_mins = (device_time / 60000) % 60;
    uint64_t d_hrs  = (device_time / 3600000) % 24;

    printf("[DEV:%u | host=%02llu:%02llu:%02llu.%03llu dev=%02llu:%02llu:%02llu.%03llu] %s - %s\n",
            device_id,
            h_hrs, h_mins, h_secs, h_ms,
            d_hrs, d_mins, d_secs, d_ms,
            level_to_string(level), message);
}
void EMBSTRIN_LOG_flush(void)
{
    fflush(stdout);
}
