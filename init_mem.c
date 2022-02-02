#include <stddef.h>
#include "mem_manage_info.h"
#include "mem_info.h"

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


void memInfoLt_and_heap_init(){
    size_t space_needed=mem_chunk_usable*sizeof(mem_chunk_head_t);

    for(int i=0;i<mem_chunk_usable;i++)
    {   
        level_one_entry_num_t num=(mmap_nodes_usable[i].length/page_size)/level2_table_size;
        if ((mmap_nodes_usable[i].length/page_size)%level2_table_size!=0)
            num++;
        space_needed+=( sizeof(mem_table_level1_entry_t) + num*level2_table_size*sizeof(mem_table_level2_entry_t) );
    }

    page_num_t page_needed=space_needed/page_size+1 +4;
//                                                 put new pagetable.4 should be enough.
    heap_start=heap_end=(((ptr_t) _kernel_end)/page_size+1)*page_size; //init heap with 0 space

    for(int i=0;i<mem_chunk_usable;i++)
    {   
        ptr_t base_addr=mmap_nodes_usable[i].base_addr;
        page_num_t len=mmap_nodes_usable[i].length/page_size;

        if (len>page_needed)
            {

                for(int i=0;(page_num_t) i<page_needed;i++)
                {
                    if ( page_reg(boot_page_directory,heap_end,(base_addr+(len-i-1)*page_size),0x003) == 0)
                    {
                        i++;
                        if ((page_num_t) i!=page_needed)
                        {
                            page_reg(boot_page_directory,heap_end,(ptr_t) (base_addr+(len-i-1)*page_size),0x003);
                            heap_end+=page_size;
                        }
                    }
                    else 
                        heap_end+=page_size; //setup heap
                }
            }
        
        (((mem_free_node_temp *) (mmap_nodes_usable+i)))->length=len;
        (((mem_free_node_temp *) (mmap_nodes_usable+i)))->base_addr=base_addr;
    }
    
    ((block_header_t * ) heap_start)->length=sizeof(block_header_t);
    ((block_header_t * ) heap_start)->next=(block_header_t * ) heap_end; //set the first empty node

    mem_chunk_head_t * next=NULL;
    for(int i=mem_chunk_usable-1;i>=0;i--)
    {   

        level_one_entry_num_t num=((((mem_free_node_temp *) (mmap_nodes_usable+i)))->length)/level2_table_size;
        if (((((mem_free_node_temp *) (mmap_nodes_usable+i)))->length)%level2_table_size!=0)
            num++;//level1 table entry num
        page_num_t len=((mem_free_node_temp *) (mmap_nodes_usable+i))->length;

        mem_chunk_head=(mem_chunk_head_t * )kmalloc(sizeof(mem_chunk_head_t)+num*(sizeof(mem_table_level1_entry_t)));


        mem_chunk_head->base_addr=(((mem_free_node_temp *) (mmap_nodes_usable+i)))->base_addr;
        mem_chunk_head->length=len;
        mem_chunk_head->last_addr=(mem_chunk_head->base_addr)+(len-1)*page_size;
        mem_chunk_head->available=len;
        mem_chunk_head->entry_num=num;
        mem_chunk_head->next=next;

        init_level1_table(mem_chunk_head);

        next=mem_chunk_head;
    }

    init_cover_chunk(0x00400000,0x00400000-page_size);
    init_cover_chunk(0x00400000+0xC0000000,heap_end-page_size);
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
    }
}
