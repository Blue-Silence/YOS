
.macro isr_common n
    pusha                    # Push%es edi, %esi, ebp, %esp, %e%bx, edx, ecx, e%ax
    mov %ds,%ax
    push %eax                # 保存数据段描述符
    
    mov $0x10, %ax            # 加载内核数据段描述符表
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss
    
    push %esp        # 此时的 %esp 寄存器的值等价于 pt_re%gs 结构体的指针
    call isr_handler\n         # 在 C 语言代码里
    add $4, %esp      # 清除压入的参数
    
    pop %ebx                 # 恢复原来的数据段描述符
    mov %bx, %ds 
    mov %bx, %es
    mov %bx, %fs
    mov %bx, %gs
    mov %bx, %ss
    
    popa                     # Pops edi, %esi, ebp, %esp, %e%bx, edx, ecx, e%ax
    add $8, %esp              # 清理栈里的 error code 和 ISR
    iret
.endm

.macro ISR_NOERRCODE n
.global isr\n
isr\n:
    cli                  # 首先关闭中断
    push $0              # push 无效的中断错误代码
    push $\n             # push 中断号
    isr_common \n
.endm

# 用于有错误代码的中断
%macro ISR_ERRCODE 1
.global isr\n
isr\n:
    cli                  # 关闭中断
    push $\n              # push 中断号
    isr_common \n
.endm

# 定义中断处理函数
ISR_NOERRCODE  0    # 0 #DE 除 0 异常
ISR_NOERRCODE  1    # 1 #DB 调试异常
ISR_NOERRCODE  2    # 2 NMI
ISR_NOERRCODE  3    # 3 BP 断点异常 
ISR_NOERRCODE  4    # 4 #OF 溢出 
ISR_NOERRCODE  5    # 5 #BR 对数组的引用超出边界 
ISR_NOERRCODE  6    # 6 #UD 无效或未定义的操作码 
ISR_NOERRCODE  7    # 7 #NM 设备不可用(无数学协处理器) 
ISR_ERRCODE    8    # 8 #DF 双重故障(有错误代码) 
ISR_NOERRCODE  9    # 9 协处理器跨段操作
ISR_ERRCODE   10    # 10 #TS 无效TSS(有错误代码) 
ISR_ERRCODE   11    # 11 #NP 段不存在(有错误代码) 
ISR_ERRCODE   12    # 12 #SS 栈错误(有错误代码) 
ISR_ERRCODE   13    # 13 #GP 常规保护(有错误代码) 
ISR_ERRCODE   14    # 14 #PF 页故障(有错误代码) 
ISR_NOERRCODE 15    # 15 CPU 保留 
ISR_NOERRCODE 16    # 16 #MF 浮点处理单元错误 
ISR_ERRCODE   17    # 17 #AC 对齐检查 
ISR_NOERRCODE 18    # 18 #MC 机器检查 
ISR_NOERRCODE 19    # 19 #XM SIMD(单指令多数据)浮点异常

# 20 ~ 31 Intel 保留
ISR_NOERRCODE 20
ISR_NOERRCODE 21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_NOERRCODE 30
ISR_NOERRCODE 31
# 32 ～ 255 用户自定义
ISR_NOERRCODE 255


.global idt_flush
idt_flush:
    mov 4(%esp)+4, %eax  # 参数存入 eax 寄存器
    lidt %eax        # 加载到 IDTR
    ret
