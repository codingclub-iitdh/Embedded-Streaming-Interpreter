#ifndef EMBSTRIN_API_H
#define EMBSTRIN_API_H

#include <stdint.h>
#include <stdbool.h>
#include "embstrin_types.h"   

// ---- Program management ----
EMBSTRIN_Program* EMBSTRIN_INFRA_API_create_program(uint8_t* instructions,
                                                     instruction_count_t buf_size,
                                                     instruction_count_t instruction_count,
                                                     mem_size_t SRAM_req,
                                                     mem_size_t flash_req);

void EMBSTRIN_INFRA_API_destroy_program(EMBSTRIN_Program* program);

// ---- Queue management ----
EMBSTRIN_ProgQueue* EMBSTRIN_INFRA_API_create_queue(queue_size_t capacity);

void EMBSTRIN_INFRA_API_destroy_queue(EMBSTRIN_ProgQueue* queue);

int32_t EMBSTRIN_INFRA_API_add_program_to_queue(EMBSTRIN_ProgQueue* queue,
                                                 EMBSTRIN_Program* program);

EMBSTRIN_Program* EMBSTRIN_INFRA_API_pop_program_from_queue(EMBSTRIN_ProgQueue* queue);

int32_t     EMBSTRIN_INFRA_API_queue_is_empty(EMBSTRIN_ProgQueue* queue);
int32_t     EMBSTRIN_INFRA_API_queue_is_full(EMBSTRIN_ProgQueue* queue);
uint32_t EMBSTRIN_INFRA_API_queue_count(EMBSTRIN_ProgQueue* queue);

// ---- Task management ----
EMBSTRIN_Task* EMBSTRIN_INFRA_API_create_task(EMBSTRIN_Program* program,
                                               uint32_t priority,
                                               task_id_t parent_id,
                                               ttl_t ttl);

EMBSTRIN_Task* EMBSTRIN_INFRA_API_get_task(EMBSTRIN_Device* device,
                                            task_id_t task_id);

int32_t EMBSTRIN_INFRA_API_update_task_state(EMBSTRIN_Task* task,
                                              task_state_t new_state);

void EMBSTRIN_INFRA_API_destroy_task(EMBSTRIN_Task* task);

// ---- Device management ----
EMBSTRIN_Device* EMBSTRIN_INFRA_API_register_device(EMBSTRIN_Host* host,
                                                      device_id_t device_id,
                                                      device_type_t device_type,
                                                      queue_size_t taks_queue_max);

int32_t EMBSTRIN_INFRA_API_remove_device(EMBSTRIN_Host* host,
                                          device_id_t device_id);

EMBSTRIN_Device* EMBSTRIN_INFRA_API_get_device(EMBSTRIN_Host* host,
                                                device_id_t device_id);

int32_t EMBSTRIN_INFRA_API_assign_task(EMBSTRIN_Device* device,
                                        EMBSTRIN_Task* task);

int32_t EMBSTRIN_INFRA_API_set_device_state(EMBSTRIN_Device* device,
                                              bool state);

int32_t EMBSTRIN_INFRA_API_add_neighbour(EMBSTRIN_Device* device,
                                          EMBSTRIN_Device* neighbour);

int32_t EMBSTRIN_INFRA_API_remove_neighbour(EMBSTRIN_Device* device,
                                             device_id_t neighbour_id);

// ---- Host management ----
EMBSTRIN_Host* EMBSTRIN_INFRA_API_create_host(queue_size_t device_capacity,
                                               queue_size_t queue_capacity);

void EMBSTRIN_INFRA_API_destroy_host(EMBSTRIN_Host* host);

#endif
