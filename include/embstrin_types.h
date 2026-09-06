#ifndef EMBSTRIN_TYPES_H
#define EMBSTRIN_TYPES_H


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



typedef uint32_t program_id_t;
typedef uint32_t instruction_count_t;
typedef uint32_t task_id_t;
typedef int32_t task_state_t;
typedef uint32_t device_id_t;
typedef uint32_t device_type_t;
typedef uint32_t queue_size_t;
typedef uint32_t mem_size_t;        //this includes all types of memory: SRAM, flash, etc 
typedef uint32_t ttl_t;



typedef struct
{
    program_id_t program_id;
    uint8_t *instructions;
    instruction_count_t instruction_count;
    instruction_count_t instruction_buf_size;
    mem_size_t SRAM_req;
    mem_size_t flash_req;
} EMBSTRIN_Program;

typedef struct
{
    EMBSTRIN_Program **programs;
    queue_size_t count;
    queue_size_t queue_size;
} EMBSTRIN_ProgQueue;

typedef struct
{
    task_id_t task_id;
    task_id_t parent_id;
    uint32_t priority;
    ttl_t ttl;
    task_state_t task_state;
    EMBSTRIN_Program *program;
} EMBSTRIN_Task; 

typedef struct EMBSTRIN_Device EMBSTRIN_Device;

struct EMBSTRIN_Device
{
    device_id_t device_id;
    device_type_t device_type;
    EMBSTRIN_Device **neighbours;
    uint32_t neighbours_count;
    bool device_state; // true if free, false if busy
    EMBSTRIN_Task *curr_task;
    EMBSTRIN_Task **task_queue;
    queue_size_t task_queue_len;
    queue_size_t task_queue_max;
};

typedef struct
{
    EMBSTRIN_Device **devices;
    queue_size_t device_count;
    queue_size_t device_capacity;
    EMBSTRIN_ProgQueue pending_queue;
} EMBSTRIN_Host;

#endif 
