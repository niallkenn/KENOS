#include "syscall.h"

extern int sys_write(uint32_t file_descriptor, const char* string, uint32_t length) {
    if (file_descriptor == 1 || file_descriptor == 2) {
        volatile uint16_t* vga = (volatile uint16_t*)0xB8000;
        for (uint32_t i = 0; i < length; i++) {
            vga[i] = (0x0F << 8) | string[i];
        }
        //vga[0] = (0x0F << 8) | string[0];
        return length;
    }   

    return -1;
}
