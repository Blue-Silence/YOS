#include <stddef.h>
#include "mem_manage_info.h"
#include "init_mem_info.h"

extern uint8_t _kernel_end[];

typedef struct mem_free_node_temp{
    ptr_t base_addr; //chunk start addr
    page_num_t length; //length by page numbers
} __attribute__((packed)) mem_free_node_temp; //make sure this is no larger than mem_node_usable 


extern ptr_t heap_start;
extern ptr_t heap_end;
extern ptr_t boot_page_directory[];
extern mem_chunk_head_t * mem_chunk_head;

void memInfoLt_and_heap_init();

void init_cover_chunk(ptr_t from,ptr_t to);
void init_level1_table(mem_chunk_head_t * head);
ptr_t kgetP(ptr_t v);

void moveforward_end();

void memInfoLt_and_heap_init(){
    int heap_set_flag=0;
    mem_chunk_head=NULL;

    heap_start=heap_end=(((ptr_t) _kernel_end)/page_size+1)*page_size;

    for(int i=mem_chunk_usable-1;i>=0;i--)
    {
        ptr_t base_addr=(ptr_t) mmap_nodes_usable[i].base_addr;
        if ((base_addr%page_size)!=0)
            base_addr=(base_addr/page_size+1)*page_size; //in case the memory is not aligned

        page_num_t len=(page_num_t) ((mmap_nodes_usable[i].base_addr+mmap_nodes_usable[i].length-base_addr)/page_size);
        if (len<2) //a chunk is a least 2 pages long
            continue;

        level_one_entry_num_t num=(level_one_entry_num_t) (len/level2_table_size);
        if (len%level2_table_size!=0)
            num++;

        size_t size_needed=sizeof(mem_chunk_head_t)+num*(sizeof(mem_table_level1_entry_t))+level2_table_size*sizeof(mem_table_level2_entry_t);
        page_num_t page_needed=size_needed/page_size;
        if ((size_needed%page_size)!=0)
            page_needed++;
            
        int x=0;
        if (heap_set_flag==0)
        {
            if (page_reg(boot_page_directory,heap_end,(base_addr+(len-x-1)*page_size),0x003) == 0)
            {
                x++;
                page_reg(boot_page_directory,heap_end,(base_addr+(len-x-1)*page_size),0x003);
            }
            heap_end+=page_size;
            x++;
            ((block_header_t * ) heap_start)->length=sizeof(block_header_t);
            ((block_header_t * ) heap_start)->next=(block_header_t * ) heap_end; //set the first empty node
            heap_set_flag=1;
        }

        for(int i=0;(page_num_t) i<page_needed;i++)
        {
            if (page_reg(boot_page_directory,heap_end,(base_addr+(len-x-1)*page_size),0x003) == 0)
            {
                x++;
                page_reg(boot_page_directory,heap_end,(base_addr+(len-x-1)*page_size),0x003);
            }
            x++;

            moveforward_end();
            heap_end+=page_size;
        }
        
        mem_chunk_head_t * head=(mem_chunk_head_t * )kmalloc(sizeof(mem_chunk_head_t)+num*(sizeof(mem_table_level1_entry_t)));

        head->base_addr=base_addr;
        head->length=len;
        head->last_addr=base_addr+(len-1)*page_size;
        head->available=len;
        head->entry_num=num;
        head->next=mem_chunk_head;

        init_level1_table(head);
        mem_chunk_head=head;
    }

    init_cover_chunk(0,0x00400000-(1+2)*page_size); //Last 2 pages are special.Leave them alone;
    init_cover_chunk(0x00400000+0xC0000000,heap_end-page_size);
}

void moveforward_end(){
    block_header_t *  p=(block_header_t * ) heap_start;
    while(1)
    {
        if ((ptr_t) (p->next)==heap_end)
        {
            p->next=(block_header_t * ) (heap_end+page_size);
            break;
        }
        p=p->next;
    }
}

void init_level1_table(mem_chunk_head_t * head){
    page_num_t len=head->length;
    level_one_entry_num_t num=head->entry_num;
    mem_table_level1_entry_t * table1=(mem_table_level1_entry_t * ) (((ptr_t) head)+sizeof(mem_chunk_head_t));
    for(int i=0;(level_one_entry_num_t) i<num;i++)
    {   
        table1[i].flag=1;
        mem_table_level2_entry_t * p=table1[i].level2_table=(mem_table_level2_entry_t * ) kmalloc(level2_table_size*sizeof(mem_table_level2_entry_t));
        for(int j=0;j<1024;j++)
        {
            p[j].addr=(ptr_t) NULL;
            p[j].pid=0;
            p[j].flag=0;
            if (len>0)
            {
                p[j].flag=1;//exist
                len--;
            }
        }
    }
}

ptr_t kgetP(ptr_t v){
    ptr_t v1=(boot_page_directory[v>>22])&0xFFFFF000;
    ptr_t * p1=(ptr_t * ) map_physical(v1);
    ptr_t p2=p1[(v>>12)&0b1111111111];
    map_physical_free((ptr_t) p1);
    return p2&0xFFFFF000;
}

void init_cover_chunk(ptr_t from,ptr_t to){
    while(1)
    {
        if (from>to)
            break;

        mem_table_level2_entry_t x={.addr=from,.pid=0,.flag=0b11};
        *(find_physical_page_info(kgetP(from)))=x;
        from+=page_size;
    } //Yes I know this is bloody slow.But it only happened during the booting,right?
}
