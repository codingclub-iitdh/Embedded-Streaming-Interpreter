#include <stdint.h>
#include <stdbool.h>

#define EMBSTRIN_BLOCKED -2
#define EMBSTRIN_ERROR -1
#define EMBSTRIN_FALSE 0
#define EMBSTRIN_TRUE 1
#define EMBSTRIN_SUBMITTED 2
#define EMBSTRIN_PENDING 3
#define EMBSTRIN_RUNNING 4
#define EMBSTRIN_COMPLETED 5

typedef struct
{
    uint32_t program_id;
    uint8_t *instructions;
    uint32_t instruction_count;
    uint32_t instruction_buf_size;
    uint32_t SRAM_req;
    uint32_t flash_req;
} EMBSTRIN_Program;

typedef struct
{
    EMBSTRIN_Program **programs;
    uint32_t count;
    uint32_t queue_size;
} EMBSTRIN_ProgQueue;

typedef struct
{
    uint32_t task_id;
    uint32_t parent_id;
    uint32_t priority;
    uint32_t ttl;
    int32_t task_state;
    EMBSTRIN_Program *program;
} EMBSTRIN_Task; 

typedef struct EMBSTRIN_Device EMBSTRIN_Device;

struct EMBSTRIN_Device
{
    uint32_t device_id;
    uint32_t device_type;
    EMBSTRIN_Device **neighbours;
    uint32_t neighbours_count;
    bool device_state; // true if free, false if busy
    EMBSTRIN_Task *curr_task;
    EMBSTRIN_Task **task_queue;
    uint32_t task_queue_len;
    uint32_t task_queue_max;
};

typedef struct
{
    EMBSTRIN_Device **devices;
    uint32_t device_count;
    uint32_t device_capacity;
    EMBSTRIN_ProgQueue pending_queue;
} EMBSTRIN_Host;
