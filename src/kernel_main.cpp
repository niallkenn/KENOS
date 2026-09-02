extern "C" void kernel_main() {
    while (1) {
        asm volatile("hlt");
    }
}