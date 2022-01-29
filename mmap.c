#include <stdint.h>
#include "mem_info.h"

#define MEM_CHUNK_NUM

extern uint8_t temp[];

int mem_chunk_usable=0;
int mem_chunk_other=0;

void mmap(){
    uint32_t * infoAddr=(uint32_t *) *((uint32_t *)temp);
    uint32_t mmap_length=infoAddr[11];
    mem_node * mmap_addr=infoAddr[12];

    uint32_t num=mmap_length/sizeof(mem_node);
    
    mem_node_usable * nodes_u=((uint32_t) mmap_nodes_usable)-0xC0000000;
    mem_node * nodes_o=((uint32_t) mmap_nodes_other)-0xC0000000;

    for(int i=0;i<num;i++)
    {
        if (mmap_addr[i].type==1)
        {
            nodes_u[mem_chunk_usable].base_addr=mmap_addr[i].base_addr;
            nodes_u[mem_chunk_usable].length=mmap_addr[i].length;
            mem_chunk_usable++;
        }
        else 
        {   nodes_o[mem_chunk_other]=mmap_addr[i];
            mem_chunk_other++;
        }
    }
}
