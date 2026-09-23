#include "unistd.h"
#include "syscall.h"

int write(uint32_t file_descriptor, const char* string, uint32_t length) {
    int ret;
    asm volatile (
        "int $0x80"
        : "=a" (ret)                       // Output: Return value in EAX goes to 'ret'
        : "a" (SYS_WRITE), "b" (file_descriptor), "c" (string), "d" (length)  // Inputs: EAX=4, EBX=1 (stdout), ECX=str, EDX=len
        : "memory"                         // Clobber: The string memory is read/modified
    );
    return ret;
}

void yield(void) {
    asm volatile(
        "int $0x80"
        :
        : "a" (SYS_YIELD)
        : "memory"
    );
}

int getpid(void) {
    int ret;
    asm volatile(
        "int $0x80"
        : "=a" (ret)
        : "a" (SYS_GETPID)
        : "memory"
    );
    return ret;
}

void exit(void) {
    asm volatile(
        "int $0x80"
        :
        : "a" (SYS_EXIT)
    );
}