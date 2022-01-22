
extern uint8_t kern_end;

__attribute__((section(".init.text"))) void init(){
//init paging and gdt
            uint32_t * pagedir,pg_table_start;
            _asm_("mov %eax pagedir");
            pg_table_start=pagedir+1024;
            //init id map
            for(int i=0;i<1024;i++)
                {
                    pg_table_start[i]=i*4096+0x003;
                }
            pg_table_start[1023]=0x000B8000+0x003; //init VGA buffer to 4092
            pagedir[0]=pg_table_start+0x003;//inited 4M by now
            pagedir+=768+1;//move here to init higher half
            pg_table_start+=1024;
            uint32_t ptr=0xC0000000+0x00400000;
            while(0)
                {   
                    int finished=0; 
                    for(int ct=0;ct<1024;ct++)
                        {
                            if (ptr>kern_end) 
                                {
                                    finfshed=1;
                                    break; 
                                }
                            pg_table_start[ct]=ptr+0x003;
                            ptr+=4096;
                        }
                    *pgdir=pg_table_start;
                    pg_table_start+=1024;
                    pgdir+=1;
                    if flag break;                  
                }
}