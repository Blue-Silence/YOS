
project(kernel)

add_library(kernel OBJECT

            memory/mem_manage.c
            )

target_include_directories(kernel PRIVATE
                          "${PROJECT_SOURCE_DIR}/../HAL"
                          include
                          memory
                          )


