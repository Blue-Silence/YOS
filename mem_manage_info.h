#include <stdint.h>

#ifndef MEM_MANAGE_INFO_H
#define MEM_MANAGE_INFO_H


#define page_size 4096
#define block_size 16 //minimum block acc by kmalloc

typedef page_num_t uint32_t
typedef ptr_t uint32_t
typedef size_t uint32_t
typedef length_t size_t


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
#endif



