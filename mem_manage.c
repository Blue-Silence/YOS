#include "mem_info.h"
#include "mem_manage_info.h"

typedef struct mem_free_node_temp{
    ptr_t base_addr; //chunk start addr
    page_num_t length; //length by page numbers
} __attribute__((packed)) mem_free_node_temp; //make sure this is no larger than mem_node_usable 


mem_free_node * mem_free_node_head;

ptr_t heap_start;
ptr_t heap_end;




void memInfoLt_init(int freenode_num,mem_node * mem_nodes){
    int page_needed=freenode_num*sizeof(mem_free_node)/page_size+1;
    ptr_t lt_pos;
    int index;

    for(int i=0;i<freenode_num,i++)
    {   
        page_num_t len=mmap_nodes_usable[i].length/page_size;
        if (len>page_needed)
            {
                (((mem_free_node_temp *) (mmap_nodes_usable+i)))->length=len-page_needed;
                lt_pos=(ptr_t) mmap_nodes_usable[i].base_addr+page_size*len;
            }
        else 
            (((mem_free_node_temp *) (mmap_nodes_usable+i)))->length=len;

        (((mem_free_node_temp *) (mmap_nodes_usable+i)))->base_addr=mmap_nodes_usable[i].base_addr;
    }
    
    mem_free_node_head=(mem_free_node * )lt_pos;
    for(int i=0;i<freenode_num,i++)
    {   
        ((mem_free_node * )lt_pos)[i].base_addr=(((mem_free_node_temp *) (mmap_nodes_usable+i)))->base_addr;
        ((mem_free_node * )lt_pos)[i].length=(((mem_free_node_temp *) (mmap_nodes_usable+i)))->length;
        ((mem_free_node * )lt_pos)[i].next=lt_pos+i+1;
    }
}

ptr_t getFreePage(int num,mem_free_node * head){
    if (head==0) return 0;

}




//ptr_t kmalloc_in_pages(heap_page_t * page,uint32_t block_num)

/*ptr_t kmalloc(size_t size,heapsize){
    //Due to lack of info,use bitmap on the head of each page

    uint32_t block_num=(size+sizeof(block_header_t))/block_size;
    if ((size+sizeof(block_header_t))%block_size!=0)
        block_num++;

    heap_page_t * heap=(heap_page_t * ) heap_start;

    ptr_t result=0;

    for(i=0;i<heap_size;i++)
    {
        result=kmalloc_in_pages(heap+i,block_num);
        if (reslut!=0) 
            break;
    }

    return result;
    
}

 ptr_t kmalloc_in_pages(heap_page_t * page,uint32_t block_num){
    page->
} */

ptr_t kmalloc(size_t size,length_t heapsize){
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
        if (((uint32_t (head_ptr->next))-(uint32_t (head_ptr)))-(head_ptr->length)>=acc_size)
        {
            new_head=((block_header_t * ) ((ptr_t) head_ptr)+(head_ptr->length));
            new_head->next = head_ptr->next;
            new_head->length = acc_size;
            (head_ptr->next)=new_head;
            break;
        }

        head_ptr=head_ptr->next;
    }

    return ((ptr_t) new_head)+sizeof(block_header_t);
}