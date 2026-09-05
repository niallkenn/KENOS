#include "definitions.hpp"
#include "memorymap.hpp"
#include "frameallocator.hpp"
#include "paging.hpp"
#include "heapallocator.hpp"

extern "C" void kernel_main(uint32_t multibootinfo) {
    // Write 'OK' to VGA text buffer at virtual higher-half address 0xC00B8000
    volatile uint16_t* vga = reinterpret_cast<volatile uint16_t*>(0xC00B8000);
    vga[0] = 0x2F4F; // 'O' white on green
    vga[1] = 0x2F4B; // 'K' white on green

    while (1) {
        asm volatile("hlt");
    }
}