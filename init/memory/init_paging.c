#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

extern uint8_t _kernel_end[];
extern uint8_t boot_page_directory[];
extern uint8_t boot_page_table1[];
extern uint8_t gdt[];
extern uint8_t gdtr[];

void init_paging(){
//init paging and gdt
            uint32_t * pagedir=((uint32_t) boot_page_directory)-0xC0000000;
            uint32_t * pg_table_start=pagedir+1024;


            //init id map
            for(int i=0;i<1024;i++)
                {
                    pg_table_start[i]=i*4096+0x003;
                }
            pg_table_start[1023]=0x000B8000+0x003; //init VGA buffer to 4096*1023
            pagedir[0]=((uint32_t) pg_table_start)+0x003;//inited 4M by now



            pagedir+=768+1;//move here to init higher half
            pg_table_start+=1024;


            uint32_t ptr=0x00400000;
            while(1)
                {
                    int finished=0;
                    for(int ct=0;ct<1024;ct++)
                        {
                            if (0xC0000000+ptr>(uint32_t) _kernel_end)
                                {
                                    finished=1;
                                    break;
                                }
                            pg_table_start[ct]=ptr+0x003;
                            ptr+=4096;
                        }
                    *pagedir=((uint32_t) pg_table_start)+0x003;
                    pg_table_start+=1024;
                    pagedir+=1;
                    if (finished) break;
                }

}
