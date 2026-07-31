extern "C" void main() {
    *(char*)0xa0000 = 1;
    while (1) {
        asm volatile("hlt");
    }
}
