#include "mem_info.h"
#include "mem_manage_info.h"


extern uint8_t _kernel_end[];
extern ptr_t boot_page_directory[];

typedef struct mem_free_node_temp{
    ptr_t base_addr; //chunk start addr
    page_num_t length; //length by page numbers
} __attribute__((packed)) mem_free_node_temp; //make sure this is no larger than mem_node_usable 


//mem_free_node * mem_free_node_head;
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


/*void memInfoLt_and_heap_init(){
    int page_needed=mem_chunk_usable*(sizeof(mem_chunk_head_t)+sizeof(mem_node_pair_t))/page_size+1 +1 ;
    //                                                                                              put new pagetable.One should be enough for 50 chunks.

    ptr_t lt_pos;
    int index;

    heap_start=heap_end=(((ptr_t) _kernel_end)/page_size+1)*page_size; //init heap with 0 space

    for(int i=0;i<mem_chunk_usable,i++)
    {   
        ptr_t base_addr=mmap_nodes_usable[i].base_addr;
        page_num_t len=mmap_nodes_usable[i].length/page_size;

        if (len>page_needed)
            {
                index=i;

                for(int i=0;i<page_needed;i++)
                {
                    if ( page_reg(boot_page_directory,heap_end,(base_addr+(len-i-1)*page_size,0x003)) == 0);
                    {
                        i++;
                        if (i!=page_needed)
                        {
                            page_reg(boot_page_directory,heap_end,(ptr_t) (base_addr+(len-i-1)*page_size,0x003));
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

    mem_chunk_head=(mem_chunk_head_t * )kmalloc(mem_chunk_usable*(sizeof(mem_chunk_head_t)+sizeof(mem_node_pair_t)));
    mem_node_pair_t * pairs=(mem_node_pair_t * ) (mem_chunk_head+mem_chunk_usable)
    for(int i=0;i<mem_chunk_usable,i++)
    {   
        mem_chunk_head[i].base_addr=(((mem_free_node_temp *) (mmap_nodes_usable+i)))->base_addr;
        mem_chunk_head[i].length=(((mem_free_node_temp *) (mmap_nodes_usable+i)))->length;
        mem_chunk_head[i].pair_head=pairs+i;

        pairs[i].avaliable=mem_chunk_head[i].length;
        pairs[i].used=0;
        pairs[i].next=NULL;
    }

    pairs[index].available=available-page_needed;
    pairs[index].used=page_needed;
} */

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
/*
typedef struct getPage_info_t{
    page_num_t length;
    mem_node_pair_t * ptr;
    ptr_t node_start;
}getPage_info_t;


ptr_t getFreePage(mem_chunk_head_t * ptr,int num){
    ptr_t p=NULL;
    for(i==0;i<num;i++)
    {
        if (ptr[i].available==0)
            continue;

        (ptr[i].available)--;

        if ((ptr[i].pair_head->next)->available==0)
        {
            mem_node_pair_t * t=(ptr[i].pair_head->next)->next;
            (ptr[i].pair_head->used+=(ptr[i].pair_head->next)->used;
            kfree(ptr[i].pair_head->next);
            ptr[i].pair_head->next=t;
        }//merge empty chunk

        getPage_info_t t={.length=(ptr[i].pair_head)->available,.ptr=ptr[i].pair_head};
        getPage_info_t x=getFreePage_h((ptr[i].pair_head)->next  ,  t  ,  ptr[i].base_addr+(page_size*((ptr[i].pair_head)->available+(ptr[i].pair_head)->used)));

        x.ptr->used++;
        x.ptr->available--;
        p=x.node_start+(page_size*(x.ptr->available));
        
    }
    return p;
}


getPage_info_t getFreePage_h(mem_node_pair_t * ptr,getPage_info_t x,ptr_t node_start){
    if (ptr==NULL)
        return x;

    if ((ptr->next)->available==0)
    {
        mem_node_pair_t * t=(ptr->next)->next;
        ptr->used+=(ptr->next)->used;
        kfree(ptr->next);
        ptr->next=t;
    }//merge empty chunk

    
    if (ptr->length <= x.length)
    {
        getPage_info_t y={.length=ptr->length,.ptr=ptr,.node_start=node_start};
        return getFreePage_h(ptr->next,y,node_start+(page_size*(ptr->available+ptr->used)));
    }
    else 
        return getFreePage_h(one_ahead->next,x,node_start+(page_size*(ptr->available+ptr->used)));
}

*/









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



void memInfoLt_and_heap_init(){
    size_t space_needed=mem_chunk_usable*sizeof(mem_chunk_head_t);
    ptr_t lt_pos;
    int index;
    for(int i=0;i<mem_chunk_usable,i++)
    {   
        level_one_entry_num_t num=(mmap_nodes_usable[i].length/page_size)/level2_table_size;
        if ((mmap_nodes_usable[i].length/page_size)%level2_table_size!=0)
            num++;
        space_needed+=num*(sizeof(mem_table_level1_entry_t)+level2_table_size*(mem_table_level2_entry_t))
    }

    page_num_t page_needed=space_needed/page_size+1 +4;
//                                                 put new pagetable.4 should be enough.
    heap_start=heap_end=(((ptr_t) _kernel_end)/page_size+1)*page_size; //init heap with 0 space

    for(int i=0;i<mem_chunk_usable,i++)
    {   
        ptr_t base_addr=mmap_nodes_usable[i].base_addr;
        page_num_t len=mmap_nodes_usable[i].length/page_size;

        if (len>page_needed)
            {
                index=i;

                for(int i=0;i<page_needed;i++)
                {
                    if ( page_reg(boot_page_directory,heap_end,(base_addr+(len-i-1)*page_size,0x003)) == 0);
                    {
                        i++;
                        if (i!=page_needed)
                        {
                            page_reg(boot_page_directory,heap_end,(ptr_t) (base_addr+(len-i-1)*page_size,0x003));
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

    mem_chunk_head=(mem_chunk_head_t * )kmalloc(mem_chunk_usable*(sizeof(mem_chunk_head_t)+sizeof(mem_node_pair_t)));
    mem_node_pair_t * pairs=(mem_node_pair_t * ) (mem_chunk_head+mem_chunk_usable)

    mem_chunk_head_t * next=NULL;
    for(int i=mem_chunk_usable-1;i>=0,i--)
    {   

        level_one_entry_num_t num=(((((mem_free_node_temp *) (mmap_nodes_usable+i)))->length)/level2_table_size;
        if (((((mem_free_node_temp *) (mmap_nodes_usable+i)))->length)%level2_table_size!=0)
            num++;//level1 table entry num
        page_num_t len=((mem_free_node_temp *) (mmap_nodes_usable+i))->length;

        mem_chunk_head=(mem_chunk_head_t * )kmalloc(sizeof(mem_chunk_head_t)+num*(sizeof(mem_node_pair_t)));


        mem_chunk_head->base_addr=(((mem_free_node_temp *) (mmap_nodes_usable+i)))->base_addr;
        mem_chunk_head->length=len;
        mem_chunk_head->last_addr=(mem_chunk_head->base_addr)+(len-1)*page_size;
        mem_chunk_head->available=len;
        mem_chunk_head->entry_num=num;
        mem_chunk_head->next=next;

        init_level1_table(mem_chunk_head);

        next=mem_chunk_head;

        if (i==index)
            init_remove_help(mem_chunk_head,page_needed) ;//mark preacc heap page 
    }

    init_cover_chunk(0x00400000,0x00400000-page_size);
    init_cover_chunk(0x00400000+0xC0000000,heap_end-page_size)
} 

void init_level1_table(mem_chunk_head * head){
    page_num_t len=head->length;
    level_one_entry_num_t num=head->entry_num;
    mem_table_level1_entry_t * table1=(mem_table_level1_entry_t * ) (((ptr_t) head)+sizeof(mem_chunk_head_t));
    for(int i=0;i<num;i++)
    {   
        mem_table_level2_entry_t * p=table1[i].level2_table=(mem_table_level2_entry_t * ) kmalloc(level2_table_size*sizeof(mem_table_level2_entry_t));
        for(j=0;j<1023;j++)
        {
            p[j].addr=NULL;
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

ptr_t kgetP(ptr_t v);