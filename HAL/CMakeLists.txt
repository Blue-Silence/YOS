
project(HAL C ASM)

add_library(HAL OBJECT tlb_flush.s)

target_include_directories(HAL PRIVATE
                          "${PROJECT_SOURCE_DIR}"
                          )
