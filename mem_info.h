#include <stdint.h>

#ifndef MEM_INFO_H
#define MEM_INFO_H


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

extern mem_node_usable * mmap_nodes_usable[];
extern mem_node * mmap_nodes_other[];

#endif



#ifndef MEM_CHUNK_NUM
#define MEM_CHUNK_NUM

extern int mem_chunk_usable;
extern int mem_chunk_other;

#endif