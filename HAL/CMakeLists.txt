
project(HAL C ASM)

add_library(HAL OBJECT tlb_flush.s common_io.c)

target_include_directories(HAL PRIVATE
                          "${PROJECT_SOURCE_DIR}"
                          )
