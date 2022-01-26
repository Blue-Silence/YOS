#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

extern uint8_t _kernel_end[];
extern uint8_t boot_page_directory[];
extern uint8_t boot_page_table1[];
extern uint8_t gdt[];

void init(){
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

void setGDT(){
            uint32_t * gdtp=gdt;
            
            gdtp[0]=0;
            gdtp[1]=0;

            gdtp[2]=00000000110011111001101000000000b;
            gdtp[3]=00000000000000001111111111111111b;//os,code
            
            gdtp[4]=00000000110011111001001000000000b;
            gdtp[5]=00000000000000001111111111111111b;//os,data

            gdtp[6]=00000000110011111111101000000000b;
            gdtp[7]=00000000000000001111111111111111b;//user,code

            gdtp[8]=00000000110011111111001000000000b;
            gdtp[9]=00000000000000001111111111111111b;//os,data
}