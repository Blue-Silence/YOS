#include <stdint.h>

typedef struct gate_descriptor{
    uint16_t offset_low;
    uint16_t segment_selector;
    uint8_t reserved;
    uint8_t type_0_dpl_p;
    uint16_t offset_high;
} __attribute__((packed)) gate_descriptor_t;

typedef struct idt_ptr{
    uint16_t limit;     // 限长
    uint32_t base;      // 基址
} __attribute__((packed)) idt_ptr_t;

gate_descriptor_t idt[256];
idt_ptr_t idt_ptr;

void isr0();        // 0 #DE 除 0 异常
void isr1();        // 1 #DB 调试异常
void isr2();        // 2 NMI
void isr3();        // 3 BP 断点异常
void isr4();        // 4 #OF 溢出
void isr5();        // 5 #BR 对数组的引用超出边界
void isr6();        // 6 #UD 无效或未定义的操作码
void isr7();        // 7 #NM 设备不可用(无数学协处理器)
void isr8();        // 8 #DF 双重故障(有错误代码)
void isr9();        // 9 协处理器跨段操作
void isr10();       // 10 #TS 无效TSS(有错误代码)
void isr11();       // 11 #NP 段不存在(有错误代码)
void isr12();       // 12 #SS 栈错误(有错误代码)
void isr13();       // 13 #GP 常规保护(有错误代码)
void isr14();       // 14 #PF 页故障(有错误代码)
void isr15();       // 15 CPU 保留
void isr16();       // 16 #MF 浮点处理单元错误
void isr17();       // 17 #AC 对齐检查
void isr18();       // 18 #MC 机器检查
void isr19();       // 19 #XM SIMD(单指令多数据)浮点异常

// 20 ~ 31 Intel 保留
void isr20();
void isr21();
void isr22();
void isr23();
void isr24();
void isr25();
void isr26();
void isr27();
void isr28();
void isr29();
void isr30();
void isr31();

// 32 ~ 255 用户自定义异常
void isr255();


void init_idt(){
    idt_ptr.limit = sizeof(gate_descriptor_t) * 256 - 1;
    idt_ptr.base  = (uint32_t)idt;
    
    //bzero((uint8_t *)idt, sizeof(gate_descriptor_t) * 256);

    // 0-32:  用于 CPU 的中断处理
    idt_set_gate( 0, (uint32_t)isr0,  0x08, 0x8E);
    idt_set_gate( 1, (uint32_t)isr1,  0x08, 0x8E);
    idt_set_gate( 2, (uint32_t)isr2,  0x08, 0x8E);
    idt_set_gate( 3, (uint32_t)isr3,  0x08, 0x8E);
    idt_set_gate( 4, (uint32_t)isr4,  0x08, 0x8E);
    idt_set_gate( 5, (uint32_t)isr5,  0x08, 0x8E);
    idt_set_gate( 6, (uint32_t)isr6,  0x08, 0x8E);
    idt_set_gate( 7, (uint32_t)isr7,  0x08, 0x8E);
    idt_set_gate( 8, (uint32_t)isr8,  0x08, 0x8E);
    idt_set_gate( 9, (uint32_t)isr9,  0x08, 0x8E);
    idt_set_gate(10, (uint32_t)isr10, 0x08, 0x8E);
    idt_set_gate(11, (uint32_t)isr11, 0x08, 0x8E);
    idt_set_gate(12, (uint32_t)isr12, 0x08, 0x8E);
    idt_set_gate(13, (uint32_t)isr13, 0x08, 0x8E);
    idt_set_gate(14, (uint32_t)isr14, 0x08, 0x8E);
    idt_set_gate(15, (uint32_t)isr15, 0x08, 0x8E);
    idt_set_gate(16, (uint32_t)isr16, 0x08, 0x8E);
    idt_set_gate(17, (uint32_t)isr17, 0x08, 0x8E);
    idt_set_gate(18, (uint32_t)isr18, 0x08, 0x8E);
    idt_set_gate(19, (uint32_t)isr19, 0x08, 0x8E);
    idt_set_gate(20, (uint32_t)isr20, 0x08, 0x8E);
    idt_set_gate(21, (uint32_t)isr21, 0x08, 0x8E);
    idt_set_gate(22, (uint32_t)isr22, 0x08, 0x8E);
    idt_set_gate(23, (uint32_t)isr23, 0x08, 0x8E);
    idt_set_gate(24, (uint32_t)isr24, 0x08, 0x8E);
    idt_set_gate(25, (uint32_t)isr25, 0x08, 0x8E);
    idt_set_gate(26, (uint32_t)isr26, 0x08, 0x8E);
    idt_set_gate(27, (uint32_t)isr27, 0x08, 0x8E);
    idt_set_gate(28, (uint32_t)isr28, 0x08, 0x8E);
    idt_set_gate(29, (uint32_t)isr29, 0x08, 0x8E);
    idt_set_gate(30, (uint32_t)isr30, 0x08, 0x8E);
    idt_set_gate(31, (uint32_t)isr31, 0x08, 0x8E);

    // 255 将来用于实现系统调用
    idt_set_gate(255, (uint32_t)isr255, 0x08, 0x8E);

    // 更新设置中断描述符表
    idt_flush((uint32_t)&idt_ptr);
}

void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags)
{
    idt[num].offset_low = base & 0xFFFF;
    idt[num].offset_high = (base >> 16) & 0xFFFF;

    idt[num].segment_selector= sel;
    idt[num].reserved= 0;

    // 先留下 0x60 这个魔数，以后实现用户态时候
    // 这个与运算可以设置中断门的特权级别为 3
    idt[num].type_0_dpl_p = flags;  // | 0x60
}
