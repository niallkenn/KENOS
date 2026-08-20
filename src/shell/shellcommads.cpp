#include "shellcommands.h"
#include "frameallocator.h"
#include "heapallocator.h"
#include "panic.h"

void k_clear(Shell& shell) {
    if (shell.command_buffer.size() > 1) {
        shell.terminal.write("\nclear: invalid options\n");
    }

    shell.terminal.clear();
}

void k_echo(Shell& shell) {
    shell.terminal.new_line();
    for (size_t i = 1; i < shell.command_buffer.size(); i++) {
        shell.terminal.write(shell.command_buffer[i].c_str());
        shell.terminal.put_char(' ');
    }
    shell.terminal.new_line();
}

void k_help(Shell& shell) {
    shell.terminal.new_line();

    shell.terminal.write("\nabout: information about KENOS");
    shell.terminal.write("\ncalc:  evaluate arithmetic expression");
    shell.terminal.write("\nclear: clear the terminal");
    shell.terminal.write("\necho:  print text");
    shell.terminal.write("\nhelp:  list available commands");
    shell.terminal.write("\nmem:   show memory information");

    shell.terminal.new_line();
    shell.terminal.new_line();
}

void k_mem(Shell& shell) {
    uint32_t b = 1;
    uint32_t kib = b * 1024;
    uint32_t mib = kib * 1024;
    uint32_t gib = mib * 1024;

    uint32_t total = FrameAllocator::m_total_frames * 4096;
    uint32_t used = FrameAllocator::m_used_frames * 4096;
    uint32_t free = FrameAllocator::m_free_frames * 4096;

    shell.terminal.write("\n\nPhysical memory:\n\n");
    shell.terminal.write("\tTotal: ");

    if (total < kib) {
        shell.terminal.write_uint(total);
        shell.terminal.write("B, frames:  ");
    } else if (total < mib) {
        shell.terminal.write_uint(total / 1024);
        shell.terminal.write("KiB, frames:  ");
    } else if (total < gib) {
        shell.terminal.write_uint((total / 1024) / 1024);
        shell.terminal.write("MiB, frames:  ");
    } else {
        shell.terminal.write_uint(((total / 1024) / 1024) / 1024);
        shell.terminal.write("GiB, frames:  ");
    }
    shell.terminal.write_uint(total / 4096);

    shell.terminal.write("\n\tUsed: ");
    
    if (used < kib) {
        shell.terminal.write_uint(used);
        shell.terminal.write("B, frames:  ");
    } else if (used < mib) {
        shell.terminal.write_uint(used / 1024);
        shell.terminal.write("KiB, frames:  ");
    } else if (used < gib) {
        shell.terminal.write_uint((used / 1024) / 1024);
        shell.terminal.write("MiB, frames:  ");
    } else {
        shell.terminal.write_uint(((used / 1024) / 1024) / 1024);
        shell.terminal.write("GiB, frames:  ");
    }
    shell.terminal.write_uint(used / 4096);

    shell.terminal.write("\n\tFree: ");

    if (free < kib) {
        shell.terminal.write_uint(free);
        shell.terminal.write("B, frames:  ");
    } else if (free < mib) {
        shell.terminal.write_uint(free / 1024);
        shell.terminal.write("KiB, frames:  ");
    } else if (free < gib) {
        shell.terminal.write_uint((free / 1024) / 1024);
        shell.terminal.write("MiB, frames:  ");
    } else {
        shell.terminal.write_uint(((free / 1024) / 1024) / 1024);
        shell.terminal.write("GiB, frames:  ");
    }
    shell.terminal.write_uint(free / 4096);

    uint32_t hfree = HeapAllocator::m_total_free;
    uint32_t hused = HeapAllocator::m_total_used;
    uint32_t htotal = hfree + hused;

    shell.terminal.write("\n\nKernel heap:\n\n\t");

    shell.terminal.write("Total: ");
    
    if (htotal < kib) {
        shell.terminal.write_uint(htotal);
        shell.terminal.write("B");
    } else if (htotal < mib) {
        shell.terminal.write_uint(htotal / 1024);
        shell.terminal.write("KiB");
    } else if (htotal < gib) {
        shell.terminal.write_uint((htotal / 1024) / 1024);
        shell.terminal.write("MiB");
    } else {
        shell.terminal.write_uint(((htotal / 1024) / 1024) / 1024);
        shell.terminal.write("GiB");
    }

    shell.terminal.write("\n\tUsed: ");
    
    if (hused < kib) {
        shell.terminal.write_uint(hused);
        shell.terminal.write("B");
    } else if (hused < mib) {
        shell.terminal.write_uint(hused / 1024);
        shell.terminal.write("KiB");
    } else if (hused < gib) {
        shell.terminal.write_uint((hused / 1024) / 1024);
        shell.terminal.write("MiB");
    } else {
        shell.terminal.write_uint(((hused / 1024) / 1024) / 1024);
        shell.terminal.write("GiB");
    }

    shell.terminal.write("\n\tFree: ");
    
    if (hfree < kib) {
        shell.terminal.write_uint(hfree);
        shell.terminal.write("B");
    } else if (hfree < mib) {
        shell.terminal.write_uint(hfree / 1024);
        shell.terminal.write("KiB");
    } else if (hfree < gib) {
        shell.terminal.write_uint((hfree) / 1024);
        shell.terminal.write("MiB");
    } else {
        shell.terminal.write_uint(((hfree/ 1024) / 1024) / 1024);
        shell.terminal.write("GiB");
    }


    shell.terminal.new_line();
    shell.terminal.new_line();
}

void k_calc(Shell& shell) {
    shell.terminal.new_line();
    if (shell.command_buffer.size() != 4) {
        shell.terminal.write("calc: invalid input");
    } else {
        if (shell.command_buffer[1].isNumber() && shell.command_buffer[3].isNumber()) {
            int result = __INT_MAX__;
            if (shell.command_buffer[2].equalTo("+")) {
                result = shell.command_buffer[1].toInt() + shell.command_buffer[3].toInt();
            } else if (shell.command_buffer[2].equalTo("-")) {
                result = shell.command_buffer[1].toInt() - shell.command_buffer[3].toInt();
            } else if (shell.command_buffer[2].equalTo("*")) {
                result = shell.command_buffer[1].toInt() * shell.command_buffer[3].toInt();
            } else if (shell.command_buffer[2].equalTo("/")) {
                result = shell.command_buffer[1].toInt() / shell.command_buffer[3].toInt();
            } else {
                shell.terminal.write("calc: invalid input");
            }

            if (result != __INT_MAX__) {
                if (result < 0) {
                    shell.terminal.put_char('-');
                    shell.terminal.write_uint(-result);
                } else shell.terminal.write_uint(result);
            }
        } else {
            shell.terminal.write("calc: invalid input");
        }
    }

    shell.terminal.new_line();
}

void k_about(Shell& shell) {
    shell.terminal.write("\n\n\t\tKENOS v1.0\n\n");
    shell.terminal.write("\t\tA 32-bit x86 operating system\n");
    shell.terminal.write("\t\tBuilt from the ground up\n");
    shell.terminal.write("\t\tWith nothing but shit code\n\n");
}

void k_command_error(Shell& shell) {
    shell.terminal.write("\ncommand not found: ");
    shell.terminal.write(shell.command_buffer[0].c_str());
    shell.terminal.new_line();
}

void k_alloc(Shell& shell) {
    void* a = kmalloc(shell.command_buffer[1].toInt());
    (void)a;
    shell.terminal.new_line();
}

void k_panic() {
    panic("Panic intentionally called");
}
