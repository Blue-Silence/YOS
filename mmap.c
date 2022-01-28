#include <stdint.h>
#include "mem_info_type.h"

extern uint8_t temp[];

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
            nodes_u[num_u].base_addr=mmap_addr[i].base_addr;
            nodes_u[num_u].length=mmap_addr[i].length;
            num_u++;
        }
        else 
        {   nodes_o[num_o]=mmap_addr[i];
            num_o++;
        }
    }
}
