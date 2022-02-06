


.global tlb_flush
tlb_flush:
    movl %cr3, %eax  # 参数存入 eax 寄存器
    movl %eax, %cr3        # 加载到 IDTR
    ret
