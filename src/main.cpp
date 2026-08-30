#include "definitions.h"
#include "keyboard.h"
#include "shell.h"
#include "idt.h"
#include "terminal.h"
#include "pic.h"
#include "interrupts.h"
#include "pit.h"
#include "memorymap.h"
#include "frameallocator.h"
#include "paging.h"
#include "heapallocator.h"
#include "kstring.h"
#include "kvector.h"
#include "ide.h"
#include "panic.h"
#include "fat16.h"

extern "C" void main() {
    Idt::initialize();

    Pic::initialize();

    uint32_t pit_frequency = 100;
    Pit::initialize(pit_frequency);

    MemoryMap::initialize();

    FrameAllocator::initialise();

    Paging::initialise();

    HeapAllocator::initialise();
    
    Terminal terminal(5, 5, 315, 195, LIGHT_GREY, BLACK);

    if (!IDE::initialise()) terminal.write("IDE: drive not found\n");
    
    FAT16::format();

    if (!FAT16::initialise()) terminal.write("FAT16: filesystem not found\n");

    FAT16::createFile("file1.");
    FAT16::createFile("file2.c");
    FAT16::createFile("file3.txt");

    kVector<DirectoryEntryName> names = FAT16::listRootDirectory();

    for (size_t i = 0; i < names.size(); i++) {
        terminal.write(names[i].name);
        terminal.new_line();
    }
    
    Shell shell(terminal);
    
    Keyboard::set_shell(&shell);

    enable_interrupts();

    while (1) {
        asm volatile("hlt");
    }
}
