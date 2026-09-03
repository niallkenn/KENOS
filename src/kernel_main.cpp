#include "definitions.hpp"
#include "memorymap.hpp"

extern "C" void kernel_main(uint32_t multibootinfo) {
    MemoryMap::initialize(multibootinfo);

    while (1) {
        asm volatile("hlt");
    }
}