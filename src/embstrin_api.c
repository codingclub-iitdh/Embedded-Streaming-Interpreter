#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "embstrin_types.h"
#include "embstrin_api.h"

// ---- Program management ----

EMBSTRIN_Program *EMBSTRIN_INFRA_API_create_program(uint8_t *instructions,
                                                    instruction_count_t buf_size,
                                                    instruction_count_t instruction_count,
                                                    mem_size_t SRAM_req,
                                                    mem_size_t flash_req)
{
    EMBSTRIN_Program *program = (EMBSTRIN_Program *)malloc(sizeof(EMBSTRIN_Program));
    if (program == NULL)
        return NULL;

    program->instructions = (uint8_t *)malloc(buf_size);
    if (program->instructions == NULL)
    {
        free(program);
        return NULL;
    }
    memcpy(program->instructions, instructions, buf_size);

    program->instruction_buf_size = buf_size;
    program->instruction_count = instruction_count;
    program->SRAM_req = SRAM_req;
    program->flash_req = flash_req;

    return program;
}

void EMBSTRIN_INFRA_API_destroy_program(EMBSTRIN_Program *program)
{
    if (program == NULL)
        return;
    free(program->instructions);
    free(program);
}

/*------------ Queue Manangement --------------*/

EMBSTRIN_ProgQueue *EMBSTRIN_INFRA_API_create_queue(queue_size_t capacity)
{
    EMBSTRIN_ProgQueue *queue = (EMBSTRIN_ProgQueue *)malloc(sizeof(EMBSTRIN_ProgQueue));
    if (!queue)
    {
        return NULL;
    }

    queue->programs = (EMBSTRIN_Program **)malloc(sizeof(EMBSTRIN_Program *) * capacity);
    if (!queue->programs)
    {
        free(queue);
        return NULL;
    }
    queue->count = 0;
    queue->queue_size = capacity;

    return queue;
}

void EMBSTRIN_INFRA_API_destroy_queue(EMBSTRIN_ProgQueue *queue)
{
    if (queue == NULL)
    {
        return;
    }
    free(queue->programs);
    free(queue);
}

int32_t EMBSTRIN_INFRA_API_add_program_to_queue(EMBSTRIN_ProgQueue *queue,
                                                EMBSTRIN_Program *program)
{

    if (queue == NULL || program == NULL)
    {
        return EMBSTRIN_ERROR;
    }
    if (queue->count >= queue->queue_size)
    {
        return EMBSTRIN_ERROR;
    }

    queue->programs[(queue->count)++] = program;
    return EMBSTRIN_TRUE;
}

EMBSTRIN_Program *EMBSTRIN_INFRA_API_pop_program_from_queue(EMBSTRIN_ProgQueue *queue)
{
    if (queue == NULL || queue->count == 0)
        return NULL;

    EMBSTRIN_Program *program = queue->programs[0];

    for (queue_size_t i = 0; i < queue->count - 1; i++)
    {
        queue->programs[i] = queue->programs[i + 1];
    }

    queue->count--;
    return program;
}

int32_t EMBSTRIN_INFRA_API_queue_is_empty(EMBSTRIN_ProgQueue *queue)
{
    if (!queue)
    {
        return EMBSTRIN_ERROR;
    }
    if (!queue->count)
    {
        return EMBSTRIN_TRUE;
    }
    return EMBSTRIN_FALSE;
}

int32_t EMBSTRIN_INFRA_API_queue_is_full(EMBSTRIN_ProgQueue *queue)
{
    if (!queue)
    {
        return EMBSTRIN_ERROR;
    }
    if (queue->count == queue->queue_size)
    {
        return EMBSTRIN_TRUE;
    }
    return EMBSTRIN_FALSE;
}

uint32_t EMBSTRIN_INFRA_API_queue_count(EMBSTRIN_ProgQueue *queue)
{
    if (!queue)
    {
        return EMBSTRIN_ERROR;
    }
    return queue->count;
}

// ---- Task management ----
EMBSTRIN_Task *EMBSTRIN_INFRA_API_create_task(EMBSTRIN_Program *program,
                                              uint32_t priority,
                                              task_id_t parent_id,
                                              ttl_t ttl)
{
    EMBSTRIN_Task *task = (EMBSTRIN_Task *)malloc(sizeof(EMBSTRIN_Task));
    if (!task)
    {
        return NULL;
    }
    static task_id_t next_task_id = 1;
    task->task_id = next_task_id++;
    task->program = program;
    task->priority = priority;
    task->parent_id = parent_id;
    task->ttl = ttl;
    task->task_state = EMBSTRIN_SUBMITTED;
    return task;
}

EMBSTRIN_Task *EMBSTRIN_INFRA_API_get_task(EMBSTRIN_Device *device,
                                           uint32_t task_id)
{
    if (device == NULL || device->task_queue == NULL)
    {
        return NULL;
    }
    for (uint32_t i = 0; i < device->task_queue_len; i++)
    {
        if (device->task_queue[i]->task_id == task_id)
        {
            return device->task_queue[i];
        }
    }
    return NULL;
}

int32_t EMBSTRIN_INFRA_API_update_task_state(EMBSTRIN_Task *task, task_state_t new_state)
{
    if (!task)
        return EMBSTRIN_ERROR;

    if (new_state < EMBSTRIN_BLOCKED || new_state > EMBSTRIN_COMPLETED)
        return EMBSTRIN_ERROR;

    task->task_state = new_state;
    return EMBSTRIN_TRUE;
}

void EMBSTRIN_INFRA_API_destroy_task(EMBSTRIN_Task *task)
{
    if (!task)
    {
        return;
    }
    free(task);
}

// ---- Device management ----
EMBSTRIN_Device *EMBSTRIN_INFRA_API_register_device(EMBSTRIN_Host *host,
                                                    device_id_t device_id,
                                                    device_type_t device_type,
                                                    queue_size_t task_queue_max)
{
    if (host->device_count >= host->device_capacity)
    {
        return NULL;
    }
    EMBSTRIN_Device *device = (EMBSTRIN_Device *)malloc(sizeof(EMBSTRIN_Device));
    if (!device)
        return NULL;
    device->device_id = device_id;
    device->device_type = device_type;
    device->neighbours = NULL;
    device->neighbours_count = 0;
    device->device_state = true;
    device->curr_task = NULL;
    device->task_queue_max = task_queue_max;
    device->task_queue = (EMBSTRIN_Task **)malloc(sizeof(EMBSTRIN_Task *) * device->task_queue_max);
    device->task_queue_len = 0;
    host->devices[host->device_count++] = device;
    return device;
}

int32_t EMBSTRIN_INFRA_API_remove_device(EMBSTRIN_Host *host,
                                         device_id_t device_id)
{
    if (host == NULL)
    {
        return EMBSTRIN_ERROR;
    }
    bool found = false;
    int32_t found_index = -1;
    for (uint32_t i = 0; i < host->device_count; i++)
    {
        if (host->devices[i]->device_id == device_id)
        {
            found_index = i;
            break;
        }
    }
    if (found_index == -1)
        return EMBSTRIN_ERROR;

    for (uint32_t i = found_index; i < host->device_count - 1; i++)
    {
        host->devices[i] = host->devices[i + 1];
    }
    host->devices[host->device_count - 1] = NULL;
    host->device_count--;
    return EMBSTRIN_TRUE;
}

EMBSTRIN_Device *EMBSTRIN_INFRA_API_get_device(EMBSTRIN_Host *host,
                                               device_id_t device_id)
{
    if (host == NULL)
    {
        return NULL;
    }
    for (uint32_t i = 0; i < host->device_count; i++)
    {
        if (host->devices[i]->device_id == device_id)
        {
            return host->devices[i];
        }
    }
    return NULL;
}

int32_t EMBSTRIN_INFRA_API_assign_task(EMBSTRIN_Device *device,
                                       EMBSTRIN_Task *task)
{
    if (!device || !task)
    {
        return EMBSTRIN_ERROR;
    }
    if (device->task_queue_len >= device->task_queue_max)
    {
        return EMBSTRIN_FALSE;
    }
    device->task_queue[device->task_queue_len++] = task;
    return EMBSTRIN_TRUE;
}

int32_t EMBSTRIN_INFRA_API_set_device_state(EMBSTRIN_Device *device,
                                            bool state)
{
    if (!device)
    {
        return EMBSTRIN_ERROR;
    }
    device->device_state = state;
    return EMBSTRIN_TRUE;
}

int32_t EMBSTRIN_INFRA_API_add_neighbour(EMBSTRIN_Device *device,
                                         EMBSTRIN_Device *neighbour)
{
    if (!device || !neighbour)
    {
        return EMBSTRIN_ERROR;
    }
    device->neighbours = (EMBSTRIN_Device **)realloc(
        device->neighbours,
        sizeof(EMBSTRIN_Device *) * (device->neighbours_count + 1));
    if (!device->neighbours)
        return EMBSTRIN_ERROR;
    device->neighbours[device->neighbours_count++] = neighbour;
    return EMBSTRIN_TRUE;
}

int32_t EMBSTRIN_INFRA_API_remove_neighbour(EMBSTRIN_Device *device,
                                            device_id_t neighbour_id)
{
    if (!device)
    {
        return EMBSTRIN_ERROR;
    }
    bool found = false;
    for (uint32_t i = 0; i < device->neighbours_count - 1; i++)
    {
        if (device->neighbours[i]->device_id == neighbour_id)
        {
            found = true;
        }
        if (found && i+1 < device->neighbours_count )
        {
            device->neighbours[i] = device->neighbours[i + 1];
        }
    }
    if (!found)
        return EMBSTRIN_ERROR;
    device->neighbours_count--;
    return EMBSTRIN_TRUE;
}

// ---- Host management ----
EMBSTRIN_Host *EMBSTRIN_INFRA_API_create_host(queue_size_t device_capacity,
                                              queue_size_t queue_capacity)
{
    EMBSTRIN_Host *host = (EMBSTRIN_Host *)malloc(sizeof(EMBSTRIN_Host));
    if (!host)
        return NULL;
    host->device_capacity = device_capacity;
    host->devices = (EMBSTRIN_Device **)malloc(sizeof(EMBSTRIN_Device *) * device_capacity);
    if (!host->devices)
    {
        free(host);
        return NULL;
    }
    host->pending_queue.programs = (EMBSTRIN_Program **)malloc(sizeof(EMBSTRIN_Program *) * queue_capacity);
    if (!host->pending_queue.programs)
    {
        free(host->devices);
        free(host);
        return NULL;
    }
    host->pending_queue.count = 0;
    host->pending_queue.queue_size = queue_capacity;
    host->device_count = 0;
    host->device_capacity = device_capacity;
    return host;
}

void EMBSTRIN_INFRA_API_destroy_host(EMBSTRIN_Host *host)
{
    if (!host)
        return;

    free(host->pending_queue.programs);
    free(host->devices);
    free(host);
}
