#include "definitions.hpp"
#include "memorymap.hpp"
#include "frameallocator.hpp"
#include "paging.hpp"

extern "C" void kernel_main(uint32_t multibootinfo) {
    MemoryMap::initialise(multibootinfo);

    FrameAllocator::initialise(multibootinfo, *reinterpret_cast<uint32_t*>(multibootinfo));

    void* a = FrameAllocator::allocate();
    void* b = FrameAllocator::allocate();

    FrameAllocator::free(a);
    FrameAllocator::free(b);

    Paging::initialise();

    while (1) {
        asm volatile("hlt");
    }
}