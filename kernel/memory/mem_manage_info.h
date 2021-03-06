#include <stdint.h>

#ifndef MEM_MANAGE_INFO_H
#define MEM_MANAGE_INFO_H

#include "mem_manage.h"

#define page_size 4096
#define block_size 16 //minimum block acc by kmalloc

#define level2_table_size 1024 //



typedef uint32_t level_one_entry_num_t;

typedef struct block_header_t{
    length_t length; //length
    struct block_header_t * next; //point to next
} __attribute__((packed)) block_header_t;

typedef struct mem_chunk_head_t{
    ptr_t base_addr; //chunk start addr
    page_num_t length; //length by page numbers
    ptr_t last_addr;
    page_num_t available;
    level_one_entry_num_t entry_num;//entry num for the level 1 table
    struct mem_chunk_head_t * next;
} __attribute__((packed)) mem_chunk_head_t;//followed by level 1 table with entry_num entries

typedef struct mem_table_level1_entry_t{
    mem_table_level2_entry_t * level2_table;
    flag_t flag;
} __attribute__((packed)) mem_table_level1_entry_t;
/*flag:
    0:set if there're pages left;
    rest are reserved;
*/

#endif



