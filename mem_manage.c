#include <stddef.h>
#include <stdbool.h>
#include "mem_info.h"
#include "mem_manage_info.h"



extern ptr_t boot_page_directory[];

mem_chunk_head_t * mem_chunk_head;

ptr_t heap_start;
ptr_t heap_end;



ptr_t page_reg(ptr_t * table,ptr_t v_addr,ptr_t p_addr,uint16_t flag);
//register virtual addr with physical addr;return 0 if page is assigned to topest table entry;return previous p_addr otherwise.

page_get_t get_page(mem_chunk_head_t * p); //get physical page
int free_page(ptr_t p); //free physical page

ptr_t kmalloc(size_t size);
int kfree(ptr_t free_ptr);

ptr_t map_physical(ptr_t p_addr);
int map_physical_free(ptr_t p_addr);

mem_table_level2_entry_t set_physical_page_info(mem_table_level2_entry_t x,ptr_t p);
//set level2 tabel entry for physical page p to x


ptr_t page_reg(ptr_t * table,ptr_t v_addr,ptr_t p_addr,uint16_t flag){

    int index1=v_addr/(page_size*1024);
    ptr_t * table2=(ptr_t * ) ((table[index1])&0xFFFFF000);
    if (table[index1]==(ptr_t) NULL)
    {
        table[index1]=p_addr+flag;
        return 0;
    }

    int index2=(v_addr%(page_size*1024))/page_size;
    ptr_t * table2_mapped=(ptr_t * ) map_physical((ptr_t) table2);
    ptr_t re=table2_mapped[index2];
    table2_mapped[index2]=p_addr+flag;
    map_physical_free((ptr_t) table2_mapped);

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
        if (head_ptr==(block_header_t * ) heap_end)
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
    boot_page_table1[1022]=(ptr_t) NULL;
    return 0;
}

mem_table_level2_entry_t set_physical_page_info(mem_table_level2_entry_t x,ptr_t p){
    mem_chunk_head_t * h=mem_chunk_head;
    while(1)
    {
        if (h->last_addr>=p)
            break;
        h=h->next;
    }

    page_num_t i=(p-(h->base_addr))/page_size;

    level_one_entry_num_t n=i/level2_table_size;
    page_num_t index=i%level2_table_size;
    mem_table_level1_entry_t * table1=(mem_table_level1_entry_t * ) ((ptr_t) h +sizeof(mem_chunk_head_t));
    mem_table_level2_entry_t y=(table1[n].level2_table)[index];
    (table1[n].level2_table)[index]=x;
    return y;
}


page_get_t get_page_h(mem_table_level1_entry_t * p);
page_get_t get_page(mem_chunk_head_t * p){
    page_get_t x={.available_bit=false};
    
    if (p==NULL)
        return x;
    if (p->available==0)
        return x;
    mem_table_level1_entry_t * table1=(mem_table_level1_entry_t * ) (((ptr_t) p)+sizeof(mem_chunk_head_t));
    for(int i=0;i<(p->entry_num);i++)
    {
        x=get_page_h(table1+i);
        if (x.available_bit==true)
        {
            p->available--;
            x.p+=(p->base_addr+i*level2_table_size*page_size);
            return x;
        }    
    }
    p->available=0;
    return get_page(p->next);
}

page_get_t get_page_h(mem_table_level1_entry_t * p){
    page_get_t x={.available_bit=false};
    if (((p->flag)&(1<<0)) == 0)
        return x;
    for(int i=0;i<level2_table_size;i++)
    {
        flag_t f=(p->level2_table)[i].flag;
        if (((f&(1<<0)) == 0))
        {
            (p->flag)=(p->flag)-1;
            return x;

        }
            
        if ((f&(1<<1)) == 0)
        {
            x.available_bit=true;
            x.p=page_size*i;
            return x;
        }
    }

    (p->flag)=(p->flag)-1;
    return x;
}
int free_page(ptr_t p){
    mem_table_level2_entry_t x={.addr=NULL,.pid=0,.flag=1};
    set_physical_page_info(x,p);
    return 0;
}

