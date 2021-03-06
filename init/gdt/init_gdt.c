#include <stdint.h>

#define GDT_LENGTH 5 //Entry number

typedef
struct gdt_entry_t {
    uint16_t limit_low; // 段界限 15 ～ 0
    uint16_t base_low; // 段基地址 15 ～ 0
    uint8_t base_middle; // 段基地址 23 ～ 16
    uint8_t access; // 段存在位、描述符特权级、描述符类型、描述符子类别
    uint8_t granularity; // 其他标志、段界限 19 ～ 16
    uint8_t base_high; // 段基地址 31 ～ 24
} __attribute__((packed)) gdt_entry_t;

// GDTR
typedef
struct gdt_ptr_t {
    uint16_t limit; // 全局描述符表限长
    uint32_t base; // 全局描述符表 32 位基地址
} __attribute__((packed)) gdt_ptr_t;

void init_gdt();

extern void gdt_flush(uint32_t);

gdt_entry_t gdt_entries[GDT_LENGTH];


static void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran);



void init_gdt()
{

    gdt_ptr_t gdt_ptr={.limit=sizeof(gdt_entry_t) * GDT_LENGTH - 1,
                       .base = (uint32_t)&gdt_entries};

    gdt_set_gate(0, 0, 0, 0, 0); // NULL
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // os,code
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // os,data
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // user,code
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // user,data


    gdt_flush((uint32_t)&gdt_ptr);
}


static void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
    gdt_entries[num].base_low = (base & 0xFFFF);
    gdt_entries[num].base_middle = (base >> 16) & 0xFF;
    gdt_entries[num].base_high = (base >> 24) & 0xFF;

    gdt_entries[num].limit_low = (limit & 0xFFFF);
    gdt_entries[num].granularity = (limit >> 16) & 0x0F;

    gdt_entries[num].granularity |= gran & 0xF0;
    gdt_entries[num].access = access;
}
