cmake_minimum_required(VERSION 3.22.0)

set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY") #passing testing
set(CMAKE_C_COMPILER "i686-elf-gcc" -std=gnu99 -ffreestanding -O0 -Wall -Wextra)
set(CMAKE_ASM_COMPILER "i686-elf-as" -s)

project(YOS C ASM)

add_subdirectory(HAL)
add_subdirectory(init)
add_subdirectory(kernel)
# add the executable
add_executable(YOS kernel.c boot.s)

target_link_libraries(YOS HAL init kernel "-T linker.ld -o myos.bin -ffreestanding -O2 -nostdlib -lgcc")

target_include_directories(YOS PUBLIC
                          "${PROJECT_SOURCE_DIR}"
                          )
