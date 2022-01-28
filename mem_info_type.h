#include <stdint.h>

#ifndef MEM_INFO_TYPE_H
#define MEM_INFO_TYPE_H

extern uint8_t mmap_nodes_usable[];
extern uint8_t mmap_nodes_other[];

int num_u=0;
int num_o=0;

typedef struct mem_node{
    uint32_t size;
    uint64_t base_addr;
    uint64_t length;
    uint32_t type;
} __attribute__((packed)) mem_node;

typedef struct mem_node_usable{
    uint64_t base_addr;
    uint64_t length;
} __attribute__((packed)) mem_node_usable;

#endif