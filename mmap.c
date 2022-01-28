struct mem_ptr{
    uint32_t mmap_length;
    uint32_t mmap_addr;
} __attribute__((packed));

typedef struct mem_node{
    uint32_t size;
    uint64_t base_addr;
    uint64_t length;
    uint32_t type;
} __attribute__((packed)) mem_node;

typedef struct mem_node_usable{
    uint64_t base_addr;
    uint64_t length;
} __attribute__((packed)) mem_node_usable;

extern uint8_t mmap_nodes_usable[];
extern uint8_t mmap_nodes_other[];
extern uint8_t temp[];

int num_u=0;
int num_o=0;

void mmap(){
    uint32_t * infoAddr=temp;
    uint32_t mmap_length=infoAddr[11];
    mem_node * mmap_addr=infoAddr[12];

    uint32_t num=mmap_length/sizeof(mem_node);
    
    mem_node_usable * nodes_u=((uint32_t) mmap_nodes_usable)-0xC0000000;
    mem_node * nodes_o=((uint32_t) mmap_nodes_other)-0xC0000000;

    for(int i=iu=io=0;i<num;i++)
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