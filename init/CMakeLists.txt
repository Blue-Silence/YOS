
project(init C ASM)

add_library(init OBJECT

            gdt/init_gdt.c
            gdt/gdt_flush.s

            interrupt/init_idt.c
            interrupt/interrupt_handlers.c
            interrupt/isr_wrapper.s

            memory/init_mem.c
            memory/init_paging.c
            memory/mmap.c
            )

target_include_directories(init PRIVATE
                          "${PROJECT_SOURCE_DIR}/../HAL"
                          "${PROJECT_SOURCE_DIR}/../kernel/include"

                          gdt

                          interrupt

                          memory
                          "${PROJECT_SOURCE_DIR}/../kernel/memory"
                          )





