#include <stdint.h>

#ifndef MEM_MANAGE_INFO_H
#define MEM_MANAGE_INFO_H


#define page_size 4096
#define block_size 16 //minimum block acc by kmalloc

typedef uint32_t page_num_t;
typedef uint32_t ptr_t;
typedef uint32_t size_t;
typedef size_t length_t;


typedef struct mem_free_node{
    ptr_t base_addr; //chunk start addr
    page_num_t length; //length by page numbers
    mem_free_node * next; //point to next
} __attribute__((packed)) mem_free_node;



/* typedef struct heap_page_t{
    uint8_t bitMap[32]; 
    block_t[256-2]; 
} __attribute__((packed)) heap_page_t;

typedef struct block_t{
    uint8_t none[16] ;//making 16 bytes
} __attribute__((packed)) block_t;

//above two highly depend on page_size and block_size */

typedef struct block_header_t{
    length_t length; //length
    block_header_t * next; //point to next
} __attribute__((packed)) block_header_t;



typedef struct mem_chunk_head_t{
    ptr_t base_addr; //chunk start addr
    page_num_t length; //length by page numbers
    mem_node_pair_t * pair_head;
} __attribute__((packed)) mem_chunk_head_t;

typedef struct mem_node_pair_t{
    page_num_t avaliable; //length by page numbers
    page_num_t used;
    mem_node_pair_t * next; //point to next
} __attribute__((packed)) mem_node_pair_t;

#endif



