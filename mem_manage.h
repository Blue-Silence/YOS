#include <stdint.h>

#ifndef MEM_MANAGE_INFO_H
#define MEM_MANAGE_INFO_H


typedef struct page_get_t{
    flag_t available_bit; 
    ptr_t p;
} __attribute__((packed)) page_get_t;

typedef struct mem_table_level2_entry_t{
    ptr_t addr; //virtual addr of this page
    pid_t pid; //owner of this page 
    flag_t flag; 
} __attribute__((packed)) mem_table_level2_entry_t;
/*flag:
    0:set if the page does exist;
    1:set if the page was acced;
    2:set if it is a shared page;
    rest are reserved;
*/

ptr_t page_reg(ptr_t * table,ptr_t v_addr,ptr_t p_addr,uint16_t flag);
//register virtual addr with physical addr;return 0 if page is assigned to topest table entry;return previous p_addr otherwise.
page_get_t get_page(); //get physical page
int free_page(ptr_t p); //free physical page

ptr_t kmalloc(size_t size);
int kfree(ptr_t free_ptr);

ptr_t map_physical(ptr_t p_addr);
int map_physical_free(ptr_t p_addr);

mem_table_level2_entry_t set_physical_page_info(mem_table_level2_entry_t x,ptr_t p);
//set level2 tabel entry for physical page p to x

#endif