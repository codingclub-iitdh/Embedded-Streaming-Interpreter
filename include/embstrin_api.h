#include <stdint.h>
#include <stdbool.h>
#include "embstrin_types.h"   

// ---- Program management ----
EMBSTRIN_Program* EMBSTRIN_INFRA_API_create_program(uint8_t* instructions,
                                                     uint32_t buf_size,
                                                     uint32_t instruction_count,
                                                     uint32_t SRAM_req,
                                                     uint32_t flash_req);

void EMBSTRIN_INFRA_API_destroy_program(EMBSTRIN_Program* program);

// ---- Queue management ----
EMBSTRIN_ProgQueue* EMBSTRIN_INFRA_API_create_queue(uint32_t capacity);

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
                                               uint32_t parent_id,
                                               uint32_t ttl);

EMBSTRIN_Task* EMBSTRIN_INFRA_API_get_task(EMBSTRIN_Device* device,
                                            uint32_t task_id);

int32_t EMBSTRIN_INFRA_API_update_task_state(EMBSTRIN_Task* task,
                                              int32_t new_state);

void EMBSTRIN_INFRA_API_destroy_task(EMBSTRIN_Task* task);

// ---- Device management ----
EMBSTRIN_Device* EMBSTRIN_INFRA_API_register_device(EMBSTRIN_Host* host,
                                                      uint32_t device_id,
                                                      uint32_t device_type,
                                                      uint32_t task_queue_max);

int32_t EMBSTRIN_INFRA_API_remove_device(EMBSTRIN_Host* host,
                                          uint32_t device_id);

EMBSTRIN_Device* EMBSTRIN_INFRA_API_get_device(EMBSTRIN_Host* host,
                                                uint32_t device_id);

int32_t EMBSTRIN_INFRA_API_assign_task(EMBSTRIN_Device* device,
                                        EMBSTRIN_Task* task);

int32_t EMBSTRIN_INFRA_API_set_device_state(EMBSTRIN_Device* device,
                                              bool state);

int32_t EMBSTRIN_INFRA_API_add_neighbour(EMBSTRIN_Device* device,
                                          EMBSTRIN_Device* neighbour);

int32_t EMBSTRIN_INFRA_API_remove_neighbour(EMBSTRIN_Device* device,
                                             uint32_t neighbour_id);

// ---- Host management ----
EMBSTRIN_Host* EMBSTRIN_INFRA_API_create_host(uint32_t device_capacity,
                                               uint32_t queue_capacity);

void EMBSTRIN_INFRA_API_destroy_host(EMBSTRIN_Host* host);
