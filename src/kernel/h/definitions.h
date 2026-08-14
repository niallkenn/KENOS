#pragma once

#define PRESENT 0x1
#define WRITABLE 0x2

typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;

typedef signed char        int8_t;
typedef signed short       int16_t;
typedef signed int         int32_t;
typedef signed long long   int64_t;

typedef __SIZE_TYPE__ size_t;
typedef int ssize_t;

typedef int ptrdiff_t;
typedef uint32_t uintptr_t;
typedef int32_t intptr_t;

enum VgaColor : uint8_t {
    BLACK         = 0,
    DARK_BLUE     = 1,
    DARK_GREEN    = 2,
    CYAN          = 3,
    DARK_RED      = 4,
    MAGENTA       = 5,
    BROWN         = 6,
    LIGHT_GREY    = 7,
    DARK_GREY     = 8,
    PURPLE        = 9,
    LIGHT_GREEN   = 10,
    LIGHT_BLUE    = 11,
    LIGHT_RED     = 12,
    PINK          = 13,
    YELLOW        = 14,
    WHITE         = 15
};

extern "C" {
    extern uint32_t _kernel_end[];
    extern uint32_t _kernel_start[];
}