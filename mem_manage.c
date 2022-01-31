#include "mem_info.h"
#include "mem_manage_info.h"



extern ptr_t boot_page_directory[];

mem_chunk_head_t * mem_chunk_head;

ptr_t heap_start;
ptr_t heap_end;


void memInfoLt_and_heap_init();
ptr_t page_reg(ptr_t * table,ptr_t v_addr,ptr_t p_addr,uint32_t flag);
//register virtual addr with physical addr;return 0 if page is assigned to topest table entry;return previous p_addr otherwise.
ptr_t getFreePage(mem_chunk_head_t * ptr,int num);
ptr_t kmalloc(size_t size);
int kfree(ptr_t free_ptr);
ptr_t map_physical(ptr_t p_addr);
int map_physical_free(ptr_t p_addr);


ptr_t page_reg(ptr_t * table,ptr_t v_addr,ptr_t p_addr,uint16_t flag){

    int index1=v_addr/(page_size*1024);
    ptr_t * table2=(ptr_t * ) ((table[index])&FFFFF000)
    if (table[index]==NULL)
    {
        table[index]=p_addr+flag;
        return 0;
    }

    int index2=(v_addr%(page_size*1024))/page_size;
    ptr_t * table2_mapped=map_physical(table2)
    ptr_t re=table2_mapped[index2];
    table2_mapped[index2]=p_addr+flag;
    map_physical_free(table2_mapped);

    return re;

}



ptr_t kmalloc(size_t size){
    uint32_t block_num=(size+sizeof(block_header_t))/block_size;
    if ((size+sizeof(block_header_t))%block_size!=0)
        block_num++;
    size_t acc_size=block_num*block_size;

    block_header_t * head_ptr=(block_header_t * ) heap_start;
    block_header_t * new_head=0;
    while(1)
    {
        if (head_ptr==heap_end)
            break;
        if ((((length_t) (head_ptr->next))-((ptr_t) head_ptr))-(head_ptr->length)>=acc_size)
        {
            new_head=((block_header_t * ) (((ptr_t) head_ptr)+(head_ptr->length)));
            new_head->next = head_ptr->next;
            new_head->length = acc_size;
            (head_ptr->next)=new_head;
            break;
        }

        head_ptr=head_ptr->next;
    }

    return (((ptr_t) new_head)+sizeof(block_header_t));
}

int kfree(ptr_t free_ptr){
    block_header_t * ptr=(block_header_t * ) heap_start;

    while(1)
    {
        if (ptr==(block_header_t * ) heap_end)
            return -1; //has been freed
        if (ptr->next==((block_header_t * ) (((ptr_t) free_ptr)-sizeof(block_header_t))))
        {
            ptr->next=(ptr->next)->next;
            return 0;
        }

        ptr=ptr->next;  
    }
}


/* for now just map 4096*1022 tp p_addr.more work needed */
extern ptr_t boot_page_table1[];
ptr_t map_physical(ptr_t p_addr){

    boot_page_table1[1022]=p_addr+0x003;
    return 4096*1022;
}

int map_physical_free(ptr_t p_addr){
    boot_page_table1[1022]=NULL;
    return 0;
}

